/*
* Copyright (c) 2018, 2019 Vladimir Alemasov
* All rights reserved
*
* This program and the accompanying materials are distributed under 
* the terms of GNU General Public License version 2 
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

// USB UAC class
// This module is based on
// Universal Serial Bus Specification Revision 2.0 April 27, 2000
// Universal Serial Bus Device Class Definition for Audio Devices Release 1.0 March 18, 1998
// USB-2 refers to the Universal Serial Bus Specification, Revision 2.0
// ADC-1 refers to the USB Device Class Definition for Audio Devices, Release 1.0

#include "platform.h"
#include "usbd_core.h"
#include "usb_std.h"
#include "hal-usbd-init.h"
#include "usb-uac.h"
#include "uac-dac-drv.h"

#ifndef USBD_FULL_SPEED
#error "UAC 1.0 does not support high speed."
#endif

//--------------------------------------------
extern const audio_out_drv_t audio_out_drv;

#ifdef USBD_OTGFS
extern const struct usbd_driver usbd_otgfs;
#define usbd_hw usbd_otgfs
#elif defined USBD_OTGHS_FS
extern const struct usbd_driver usbd_otghs;
#define usbd_hw usbd_otghs
#endif

//--------------------------------------------
// An AUDIO FRAME means a block of data the host transmits to device (or vice versa)
// inside the usb isochronous packets.
// An AUDIO SAMPLE is the smallest usable quantum of digital audio.
// An USB FRAME is count by the SOF packets, for FS = 1000/s (each 1ms)
//--------------------------------------------
#define AUDIO_SAMPLE_RATE               I2S_FCLK
#define BITS_PER_AUDIO_SAMPLE           UAC_BITRES
#if BITS_PER_AUDIO_SAMPLE == 16
#define BYTES_PER_AUDIO_SAMPLE          2
#else
#define BYTES_PER_AUDIO_SAMPLE          4
#endif
#define AUDIO_CHANNELS                  I2S_CHANNELS
#define USB_FRAMES_PER_AUDIO_FRAME      1 // bInterval = 1 ms
#define USB_FRAMES_PER_FEEDBACK_FRAME   1 // bInterval = 1 ms
#define SAMPLES_PER_AUDIO_FRAME         (AUDIO_SAMPLE_RATE / 1000) * USB_FRAMES_PER_AUDIO_FRAME
#define BYTES_PER_AUDIO_FRAME           (SAMPLES_PER_AUDIO_FRAME * AUDIO_CHANNELS * BYTES_PER_AUDIO_SAMPLE)
#define MAX_BYTES_PER_AUDIO_FRAME       (BYTES_PER_AUDIO_FRAME + BYTES_PER_AUDIO_SAMPLE * AUDIO_CHANNELS * USB_FRAMES_PER_AUDIO_FRAME)
#define MAX_SAMPLES_PER_AUDIO_FRAME     (MAX_BYTES_PER_AUDIO_FRAME / BYTES_PER_AUDIO_SAMPLE)
#define AUDIO_FRAMES_IN_BUFFER          16
// fMCLK measurement interval in ms as power of 2
#define AUDIO_FMCLK_MEASUREINT_MS_POWER_OF_TWO         5  // 2^5 = 32 ms
// fMCLK measurement interval in USB frames as power of 2
#define AUDIO_FMCLK_MEASUREINT_FR_POWER_OF_TWO         AUDIO_FMCLK_MEASUREINT_MS_POWER_OF_TWO  // 2^5 = 32 frames size of 1 ms

//--------------------------------------------
#define UAC_EP0_SIZE                64
#define UAC_RXD_EP                  0x01
#define UAC_TXD_EP                  0x82
#define UAC_DATA_SZ                 MAX_BYTES_PER_AUDIO_FRAME
#define UAC_FEEDBACK_SZ             4

// Interfaces
#define UAC_CONTROL_INTERFACE       0
#define UAC_STREAMING_INTERFACE     1

// Entity IDs
#define UAC_INPUT_UNIT_ID           1
#define UAC_FEATURE_UNIT_ID         2
#define UAC_OUTPUT_UNIT_ID          3

//--------------------------------------------
#pragma pack(push, 1)
typedef struct uac_config
{
	struct usb_config_descriptor                        config;
	struct usb_iad_descriptor                           iad;
	struct usb_interface_descriptor                     ac;
	struct usb_uac_ac_header_sz1_desc                   ac_hdr;
	struct usb_uac_input_terminal_desc                  ac_itd;
#if AUDIO_CHANNELS == 1
	struct usb_uac_feature_unit_sz2x16_desc             ac_fu;
#else
	struct usb_uac_feature_unit_sz3x16_desc             ac_fu;
#endif
	struct usb_uac_output_terminal_desc                 ac_otd;
	struct usb_interface_descriptor                     as_0;
	struct usb_interface_descriptor                     as_1;
	struct usb_uac_as_desc                              as_hdr;
	struct usb_uac_as_format_type1_sz3_desc             as_format_type1;
	struct usb_uac_std_iso_endpoint_descriptor          eprx_data_std;
	struct usb_uac_cs_iso_endpoint_descriptor           eprx_data_cs;
	struct usb_uac_std_iso_endpoint_descriptor          eptx_fback_std;
} uac_config_t;
#pragma pack(pop)

//--------------------------------------------
static const struct usb_device_descriptor device_desc =
{
	.bLength            = sizeof(struct usb_device_descriptor),
	.bDescriptorType    = USB_DTYPE_DEVICE,
	.bcdUSB             = CPU_TO_LE16(VERSION_BCD(2,0,0)),
	.bDeviceClass       = USB_CLASS_MISC,
	.bDeviceSubClass    = USB_SUBCLASS_COMMON,
	.bDeviceProtocol    = USB_PROTO_IAD,
	.bMaxPacketSize0    = UAC_EP0_SIZE,
	.idVendor           = CPU_TO_LE16(0x0483),
	.idProduct          = CPU_TO_LE16(0x5740),
	.bcdDevice          = CPU_TO_LE16(VERSION_BCD(1,0,0)),
	.iManufacturer      = 1,
	.iProduct           = 2,
	.iSerialNumber      = INTSERIALNO_DESCRIPTOR,
	.bNumConfigurations = 1,
};

//--------------------------------------------
static const uac_config_t config_desc =
{
	.config =
	{
		.bLength                   = sizeof(struct usb_config_descriptor),
		.bDescriptorType           = USB_DTYPE_CONFIGURATION,
		.wTotalLength              = CPU_TO_LE16(sizeof(uac_config_t)),
		.bNumInterfaces            = 2,
		.bConfigurationValue       = 1,
		.iConfiguration            = NO_DESCRIPTOR,
		.bmAttributes              = USB_CFG_ATTR_RESERVED | USB_CFG_ATTR_SELFPOWERED,
		.bMaxPower                 = USB_CFG_POWER_MA(100),
	},
	.iad =
	{
		.bLength                   = sizeof(struct usb_iad_descriptor),
		.bDescriptorType           = USB_DTYPE_INTERFACEASSOC,
		.bFirstInterface           = 0,
		.bInterfaceCount           = 2,
		.bFunctionClass            = USB_CLASS_AUDIO,
		.bFunctionSubClass         = USB_SUBCLASS_NONE,
		.bFunctionProtocol         = USB_PROTO_NONE,
		.iFunction                 = 2,
	},
	.ac =
	{
		.bLength                   = sizeof(struct usb_interface_descriptor),
		.bDescriptorType           = USB_DTYPE_INTERFACE,
		.bInterfaceNumber          = UAC_CONTROL_INTERFACE,
		.bAlternateSetting         = 0,
		.bNumEndpoints             = 0,
		.bInterfaceClass           = USB_CLASS_AUDIO,
		.bInterfaceSubClass        = USB_SUBCLASS_AUDIOCONTROL,
		.bInterfaceProtocol        = USB_PROTO_NONE,
		.iInterface                = NO_DESCRIPTOR,
	},
	.ac_hdr =
	{
		.bLength                   = sizeof(struct usb_uac_ac_header_sz1_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UAC_AC_HEADER,
		.bcdADC                    = CPU_TO_LE16(VERSION_BCD(1,0,0)),
		.wTotalLength              = CPU_TO_LE16(sizeof(struct usb_uac_ac_header_sz1_desc) +
	                                             sizeof(struct usb_uac_input_terminal_desc) +
#if AUDIO_CHANNELS == 1
	                                             sizeof(struct usb_uac_feature_unit_sz2x16_desc) +
#else
	                                             sizeof(struct usb_uac_feature_unit_sz3x16_desc) +
#endif
	                                             sizeof(struct usb_uac_output_terminal_desc)),
		.bInCollection             = 1,
		.baInterfaceNr             = UAC_STREAMING_INTERFACE,
	},
	.ac_itd =
	{
		.bLength                   = sizeof(struct usb_uac_input_terminal_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UAC_AC_INPUT_TERMINAL,
		.bTerminalID               = UAC_INPUT_UNIT_ID,
		.wTerminalType             = CPU_TO_LE16(USB_UAC_TT_USB_STREAMING),
		.bAssocTerminal            = 0,
		.bNrChannels               = AUDIO_CHANNELS,
#if AUDIO_CHANNELS == 1
		.wChannelConfig            = CPU_TO_LE16(0x0004), // C
#else
		.wChannelConfig            = CPU_TO_LE16(0x0003), // L, R
#endif
		.iChannelNames             = 0,
		.iTerminal                 = 0,
	},
	.ac_fu =
	{
#if AUDIO_CHANNELS == 1
		.bLength                   = sizeof(struct usb_uac_feature_unit_sz2x16_desc),
#else
		.bLength                   = sizeof(struct usb_uac_feature_unit_sz3x16_desc),
#endif
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UAC_AC_FEATURE_UNIT,
		.bUnitID                   = UAC_FEATURE_UNIT_ID,
		.bSourceID                 = UAC_INPUT_UNIT_ID,
		.bControlSize              = 2,
		.bmaControls[0]            = CPU_TO_LE16(0x0000), // Master controls
		.bmaControls[1]            = CPU_TO_LE16(USB_UAC_FU_BMAC_MUTE_CONTROL), // Channel 0 controls: Mute
#if AUDIO_CHANNELS == 2
		.bmaControls[2]            = CPU_TO_LE16(USB_UAC_FU_BMAC_MUTE_CONTROL), // Channel 1 controls: Mute
#endif
		.iFeature                  = 0,
	},
	.ac_otd =
	{
		.bLength                   = sizeof(struct usb_uac_output_terminal_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UAC_AC_OUTPUT_TERMINAL,
		.bTerminalID               = UAC_OUTPUT_UNIT_ID,
		.wTerminalType             = CPU_TO_LE16(USB_UAC_OTT_HEADPHONES),
		.bAssocTerminal            = 0,
		.bSourceID                 = UAC_FEATURE_UNIT_ID,
		.iTerminal                 = 0,
	},
	.as_0 =
	{
		.bLength                   = sizeof(struct usb_interface_descriptor),
		.bDescriptorType           = USB_DTYPE_INTERFACE,
		.bInterfaceNumber          = UAC_STREAMING_INTERFACE,
		.bAlternateSetting         = 0,
		.bNumEndpoints             = 0,
		.bInterfaceClass           = USB_CLASS_AUDIO,
		.bInterfaceSubClass        = USB_SUBCLASS_AUDIOSTREAMING,
		.bInterfaceProtocol        = USB_PROTO_NONE,
		.iInterface                = NO_DESCRIPTOR,
	},
	.as_1 =
	{
		.bLength                   = sizeof(struct usb_interface_descriptor),
		.bDescriptorType           = USB_DTYPE_INTERFACE,
		.bInterfaceNumber          = UAC_STREAMING_INTERFACE,
		.bAlternateSetting         = 1,
		.bNumEndpoints             = 2,
		.bInterfaceClass           = USB_CLASS_AUDIO,
		.bInterfaceSubClass        = USB_SUBCLASS_AUDIOSTREAMING,
		.bInterfaceProtocol        = USB_PROTO_NONE,
		.iInterface                = NO_DESCRIPTOR,
	},
	.as_hdr =
	{
		.bLength                   = sizeof(struct usb_uac_as_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UAC_AS_GENERAL,
		.bTerminalLink             = UAC_INPUT_UNIT_ID,
		.bDelay                    = 0,
		.wFormatTag                = CPU_TO_LE16(USB_UAC_DATA_FORMAT_PCM),
	},
	.as_format_type1 =
	{
		.bLength                   = sizeof(struct usb_uac_as_format_type1_sz3_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UAC_AS_FORMAT_TYPE,
		.bFormatType               = USB_UAC_DATA_FORMAT_TYPE_I,
		.bNrChannels               = AUDIO_CHANNELS,
		.bSubframeSize             = BYTES_PER_AUDIO_SAMPLE,
		.bBitResolution            = BITS_PER_AUDIO_SAMPLE,
		.bSamFreqType              = 0x01,
		.tSamFreq[0]               = (uint8_t)(AUDIO_SAMPLE_RATE),
		.tSamFreq[1]               = (uint8_t)(AUDIO_SAMPLE_RATE >> 8),
		.tSamFreq[2]               = (uint8_t)(AUDIO_SAMPLE_RATE >> 16),
	},
	.eprx_data_std =
	{
		.bLength                   = sizeof(struct usb_uac_std_iso_endpoint_descriptor),
		.bDescriptorType           = USB_DTYPE_ENDPOINT,
		.bEndpointAddress          = UAC_RXD_EP,
		.bmAttributes              = USB_EPTYPE_ISOCHRONOUS | USB_EPATTR_ASYNC,
		.wMaxPacketSize            = CPU_TO_LE16(UAC_DATA_SZ),
		.bInterval                 = 1,
		.bRefresh                  = 0,
		.bSynchAddress             = UAC_TXD_EP,
	},
	.eprx_data_cs =
	{
		.bLength                   = sizeof(struct usb_uac_cs_iso_endpoint_descriptor),
		.bDescriptorType           = USB_DTYPE_CS_ENDPOINT,
		.bDescriptorSubType        = USB_DTYPE_UAC_EP_GENERAL,
		.bmAttributes              = 0x00,
		.bLockDelayUnits           = 0,
		.wLockDelay                = CPU_TO_LE16(0x0000),
	},
	.eptx_fback_std =
	{
		.bLength                   = sizeof(struct usb_uac_std_iso_endpoint_descriptor),
		.bDescriptorType           = USB_DTYPE_ENDPOINT,
		.bEndpointAddress          = UAC_TXD_EP,
		.bmAttributes              = USB_EPTYPE_ISOCHRONOUS | USB_EPUSAGE_FEEDBACK,
		.wMaxPacketSize            = CPU_TO_LE16(UAC_FEEDBACK_SZ),
		.bInterval                 = 1,
		.bRefresh                  = AUDIO_FMCLK_MEASUREINT_FR_POWER_OF_TWO,
		.bSynchAddress             = 0,
	},
};

//--------------------------------------------
static const struct usb_string_descriptor lang_desc = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC("Homemcu firmware examples");
static const struct usb_string_descriptor prod_desc_en = USB_STRING_DESC("UAC I2S-DAC demo");
static const struct usb_string_descriptor *const dtable[] =
{
	&lang_desc,
	&manuf_desc_en,
	&prod_desc_en,
};

//--------------------------------------------
typedef struct audio_state
{
	uint8_t mute[AUDIO_CHANNELS + 1];
	bool playback;
	bool feedback;
	bool start_usb;
	bool start_i2s;
	uint8_t cnt_i2s;
	uint8_t cnt_usb;
	uint32_t total_cnt_i2s;
	uint32_t total_cnt_usb;
} audio_state_t;

typedef struct feedback_state
{
	uint32_t mclk_count_measured;
	uint32_t mclk_count_expected;
	uint32_t mclk_count_correction;
	uint8_t feedback_data[UAC_FEEDBACK_SZ];
	uint32_t flagO;
	uint32_t flagE;
	uint32_t flagU;
	uint32_t flagU1;
	uint32_t flagU2;
	uint32_t flagU3;
	uint32_t flagU4;
} feedback_state_t;

//--------------------------------------------
static usbd_device udev;
static uint8_t iface_num;
static uint8_t altset_num;
static audio_state_t audio;
static feedback_state_t feedback;
// Due to use with USB FIFO and/or DMA, the data buffers below must be 32-bit aligned:
#define USB_CTRL_BUFF_SZ 128
static uint32_t ubuf[(USB_CTRL_BUFF_SZ + 3) / sizeof(uint32_t)];
static uint32_t buff_usb[AUDIO_FRAMES_IN_BUFFER][(UAC_DATA_SZ + 3) / sizeof(uint32_t)];
static uint16_t buff_usb_size[AUDIO_FRAMES_IN_BUFFER];
// UAC_CTRL_BUFF_SZ must not be less than the UAC control parameter block of the maximum length
#define UAC_CTRL_BUFF_SZ 16
static uint32_t buff_uac_ctrl[(UAC_CTRL_BUFF_SZ + 3) / sizeof(uint32_t)];

//--------------------------------------------
// FRAMERATE FEEDBACK:
// Application measures number of master clocks between "2^AUDIO_FMCLK_MEASUREINT_FR_POWER_OF_TWO" SOFs
// and calculates feedback data (framerate relative to SOF frequency, in 10.14 format).
// Feedback data is pushed to host every "2^AUDIO_FMCLK_MEASUREINT_MS_POWER_OF_TWO" ms using dedicated isochronous IN endpoint.
//--------------------------------------------
// Feedback (USB-2 5.12.4.2)
// Feedback value (Fv) is expressed in number of samples per (micro)frame.
// The Feedback value (Fv) consists of:
// 1. An integer part that represents the (integer) number of samples per (micro)frame.
// The integer part must have enough bits to represent the maximum number of samples that
// can ever occur in a single (micro)frame. Assuming that the minimum sample size is one byte, then this
// number is limited to 1,023 for full-speed endpoints. Ten bits are therefore sufficient to encode this value.
// For high-speed endpoints, this number is limited to 3*1,024=3,072 and twelve bits are needed.
// 2. A fractional part that represents the �fraction� of a sample that would be needed to match
// the sampling frequency Feedback value to a resolution of 1 Hz or better.
// The fractional part requires at least K bits, where K=10 for full-speed devices (1 ms frames) and
// K=13 for high-speed devices (125 �s microframes).
// So Feedback value (Fv) must be at least:
// 10.10 (10.14 in 3-bytes) format for full-speed devices
// 12.13 (16.16 in 4-bytes) format for high-speed devices
//
// For example, we have the feedback number 0x0B0660, for the 10.14 format it is "0x2C.0x660".
// The part to the left of the point is the whole part 0x2C = 44. The part to the right of the point is the numerator 
// of the fraction "0x660 / 2 ^ 14" = 1632 / 16384 = 0.0996. So the whole feedback is 44.0996 samples per frame.
//
// Feedback value:
// Fv = nFCLK / nSOF, where nFCLK - number of the sound frame periods, nSOF - number of SOF signals
// For 44100Hz frame frequency and full-speed endpoints, the ideal feedback value measured in 1s (nSOF = 1000) is:
// Fv = nFCLK / nSOF = 44100 / 1000 = 44.100
// If we have fSCLK = fFCLK * 32 or/and fMCLK = fFCLK * 256, feedback value can be calculated as:
// Fv = nFCLK / nSOF = nSCLK / (nSOF * 32) = nMCLK / (nSOF * 256)
// For 44100Hz frame frequency and full-speed endpoints, the ideal feedback value measured in 32ms (nSOF = 32) is:
// Fv = nSCLK / (nSOF * 32) = 45158 / (32 * 32) = 44.0996
// Fv = nMCLK / (nSOF * 256) = 361267 / (32 * 256) = 44.0999
// For 44100Hz frame frequency and full-speed endpoints, the ideal feedback value measured in 4ms (nSOF = 4) is:
// Fv = nMCLK / (nSOF * 256) = 45158 / (4 * 256) = 44.0996
//
#if (AUDIO_FMCLK_MEASUREINT_FR_POWER_OF_TWO + 8) <= 14
#define AUDIO_MCLK_FEEDBACK_1014(a) (a << (14 - (AUDIO_FMCLK_MEASUREINT_FR_POWER_OF_TWO + 8)))
#else
#define AUDIO_MCLK_FEEDBACK_1014(a) (a >> ((AUDIO_FMCLK_MEASUREINT_FR_POWER_OF_TWO + 8) - 14))
#endif

//--------------------------------------------
static void mclk_callback(uint32_t mclk)
{
	uint32_t feedback_1014;

	feedback.mclk_count_measured = mclk;
	if (audio.total_cnt_usb > audio.total_cnt_i2s)
	{
		++feedback.flagU;
		if (audio.total_cnt_usb - audio.total_cnt_i2s >= AUDIO_FRAMES_IN_BUFFER - AUDIO_FRAMES_IN_BUFFER / 8)
		{
			feedback.mclk_count_measured -= feedback.mclk_count_correction * 2;
			++feedback.flagU4;
		}
		if (audio.total_cnt_usb - audio.total_cnt_i2s >= AUDIO_FRAMES_IN_BUFFER - AUDIO_FRAMES_IN_BUFFER / 4)
		{
			feedback.mclk_count_measured -= feedback.mclk_count_correction;
			++feedback.flagU3;
		}
		if (audio.total_cnt_usb - audio.total_cnt_i2s <= AUDIO_FRAMES_IN_BUFFER / 4)
		{
			feedback.mclk_count_measured += feedback.mclk_count_correction;
			++feedback.flagU2;
		}
		if (audio.total_cnt_usb - audio.total_cnt_i2s <= AUDIO_FRAMES_IN_BUFFER / 8)
		{
			feedback.mclk_count_measured += feedback.mclk_count_correction * 2;
			++feedback.flagU1;
		}
	}
	else
	{
		++feedback.flagO;
		feedback.mclk_count_measured += feedback.mclk_count_correction * 4;
	}

	feedback_1014 = AUDIO_MCLK_FEEDBACK_1014(feedback.mclk_count_measured);
	feedback.feedback_data[0] = feedback_1014 & 0xFF;
	feedback.feedback_data[1] = (feedback_1014 >> 8) & 0xFF;
	feedback.feedback_data[2] = (feedback_1014 >> 16) & 0xFF;
	feedback.feedback_data[3] = (feedback_1014 >> 24) & 0xFF;

	audio.feedback = true;
}

//--------------------------------------------
static void start_mclk_count(void)
{
	feedback.mclk_count_expected = (uint32_t)(audio_out_drv.get_mclk() / 1000 * (1 << AUDIO_FMCLK_MEASUREINT_MS_POWER_OF_TWO));
	feedback.mclk_count_correction = feedback.mclk_count_expected / 2048;
	audio.feedback = false;
	audio_out_drv.mclk_set_sof((1 << AUDIO_FMCLK_MEASUREINT_FR_POWER_OF_TWO) - 1);
	audio_out_drv.mclk_start_count(mclk_callback);
}

//--------------------------------------------
static void stop_mclk_count(void)
{
	audio_out_drv.mclk_stop_count();
	audio.feedback = false;
}


//--------------------------------------------
static usbd_respond uac_getdesc(usbd_ctlreq *req, void **address, uint16_t *length)
{
	uint8_t dtype = req->wValue >> 8;
	uint8_t dnumber = req->wValue;
	const void* desc;
	uint16_t len = 0;

	switch (dtype)
	{
	case USB_DTYPE_DEVICE:
		desc = &device_desc;
		break;
	case USB_DTYPE_CONFIGURATION:
		desc = &config_desc;
		len = sizeof(config_desc);
		break;
	case USB_DTYPE_STRING:
		if (dnumber < sizeof(dtable) / sizeof(dtable[0]))
		{
			desc = dtable[dnumber];
		}
		else
		{
			return usbd_fail;
		}
		break;
	default:
		return usbd_fail;
	}
	if (len == 0)
	{
		len = ((struct usb_header_descriptor*)desc)->bLength;
	}
	*address = (void*)desc;
	*length = len;
	return usbd_ack;
};

//--------------------------------------------
static usbd_respond uac_feature_control(usbd_device *dev, usbd_ctlreq *req)
{
	uint8_t length;
	// Channel Number (ADC-1 5.2.1)
	uint8_t cn = req->wValue & 0xFF;
	// Control Selector (ADC-1 5.2.1)
	uint8_t cs = req->wValue >> 8;

	switch (cs)
	{
	case USB_UAC_FU_MUTE_CONTROL:
		{
			// Mute Control (ADC-1 5.2.2.4.3.1)
			switch (req->bRequest)
			{
			case USB_UAC_GET_CUR:
				if (cn == 0xFF)
				{
#if AUDIO_CHANNELS == 1
					struct usb_uac_form2_mute_control_parameter_sz2_block *block = (struct usb_uac_form2_mute_control_parameter_sz2_block *)&buff_uac_ctrl[0];
					block->bMute[0] = audio.mute[0];
					block->bMute[1] = audio.mute[1];
					length = sizeof(struct usb_uac_form2_mute_control_parameter_sz2_block);
#else
					struct usb_uac_form2_mute_control_parameter_sz3_block *block = (struct usb_uac_form2_mute_control_parameter_sz3_block *)&buff_uac_ctrl[0];
					block->bMute[0] = audio.mute[0];
					block->bMute[1] = audio.mute[1];
					block->bMute[2] = audio.mute[2];
					length = sizeof(struct usb_uac_form2_mute_control_parameter_sz3_block);
#endif
				}
				else
				{
					struct usb_uac_form1_mute_control_parameter_block *block = (struct usb_uac_form1_mute_control_parameter_block *)&buff_uac_ctrl[0];
					block->bMute = audio.mute[0];
					length = sizeof(struct usb_uac_form1_mute_control_parameter_block);
				}
				dev->status.data_ptr = &buff_uac_ctrl[0];
				dev->status.data_count = length < req->wLength ? length : req->wLength;
				return usbd_ack;
			case USB_UAC_SET_CUR:
				if (cn == 0xFF)
				{
					audio.mute[0] = req->data[0];
					audio.mute[1] = req->data[1];
#if AUDIO_CHANNELS == 2
					audio.mute[2] = req->data[2];
#endif
				}
				else
				{
					audio.mute[cn] = req->data[0];
				}
				return usbd_ack;
			}
		}
	}
	return usbd_fail;
}

//--------------------------------------------
static usbd_respond uac_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback)
{
	if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) != (USB_REQ_INTERFACE | USB_REQ_CLASS))
	{
		if (req->bRequest == USB_STD_GET_INTERFACE)
		{
			if (req->wIndex == 1)
			{
				dev->status.data_ptr = &altset_num;
				dev->status.data_count = 1;
				return usbd_ack;
			}
		}
		if (req->bRequest == USB_STD_SET_INTERFACE)
		{
			altset_num = req->wValue;
			iface_num = req->wIndex;
			if (iface_num == 1 && altset_num == 1)
			{
				if (!audio.playback)
				{
					audio.cnt_i2s = 0;
					audio.cnt_usb = 0;
					audio.total_cnt_i2s = 0;
					audio.total_cnt_usb = 0;
					audio.start_i2s = true;
					audio.start_usb = true;
					start_mclk_count();
					usbd_ep_write(dev, UAC_TXD_EP, (void *)0, 0);
				}
			}
			if (iface_num == 1 && altset_num == 0)
			{
				if (audio.playback)
				{
					stop_mclk_count();
					audio.playback = false;
				}
			}
			return usbd_ack;
		}
		return usbd_fail;
	}

	// Interface (ADC-1 5.2.1)
	uint8_t iface = req->wIndex & 0xFF;
	// Entity Id (ADC-1 5.2.1)
	uint8_t entity_id = req->wIndex >> 8;

	switch (iface)
	{
	case UAC_CONTROL_INTERFACE:
		{
			switch (entity_id)
			{
			case UAC_FEATURE_UNIT_ID:
				return uac_feature_control(dev, req);
			}
		}
	}
	return usbd_fail;
}

//--------------------------------------------
static void uac_callback(usbd_device *dev, uint8_t event, uint8_t ep)
{
	switch (event)
	{
	case usbd_evt_eprx:
		if (!audio.start_usb)
		{
			audio.playback = true;
			++audio.total_cnt_usb;
			++audio.cnt_usb;
			if (audio.cnt_usb == AUDIO_FRAMES_IN_BUFFER)
			{
				audio.cnt_usb = 0;
			}
		}
		audio.start_usb = false;
		buff_usb_size[audio.cnt_usb] = usbd_ep_read(dev, UAC_RXD_EP, &buff_usb[audio.cnt_usb][0], UAC_DATA_SZ);

#if BYTES_PER_AUDIO_SAMPLE == 4
		// The I2S interface converts the 16-bit LSB data received in SPIx_DR to
		// the serial transmission of 16 bits with the MSB (most significant bit) first.
		// But, since all 32 bits are received in LSB, it is necessary to swap 16-bit words.
		uint32_t *buf32;
		buf32 = (uint32_t *)&buff_usb[audio.cnt_usb][0];
		for (uint32_t cnt = 0; cnt < buff_usb_size[audio.cnt_usb] / sizeof(uint32_t); cnt++)
		{
			audio_out_drv.convert(&buf32[cnt], &buf32[cnt]);
		}
#endif

		if (audio.start_i2s && audio.cnt_usb >= AUDIO_FRAMES_IN_BUFFER / 2)
		{
			audio.start_i2s = false;
			audio_out_drv.write_dma_txbuf(&buff_usb[audio.cnt_i2s][0], buff_usb_size[audio.cnt_i2s]);
		}
		break;
	case usbd_evt_eptx:
		if (audio.feedback)
		{
			usbd_ep_write(dev, UAC_TXD_EP, &feedback.feedback_data[0], sizeof(feedback.feedback_data));
			audio.feedback = false;
		}
		else
		{
			usbd_ep_write(dev, UAC_TXD_EP, (void *)0, 0);
		}
		break;
	default:
		break;
	}
}

//--------------------------------------------
static void i2s_tx_complete_callback(void)
{
	if (audio.playback)
	{
		if (audio.total_cnt_i2s < audio.total_cnt_usb)
		{
			++audio.total_cnt_i2s;
			++audio.cnt_i2s;
			if (audio.cnt_i2s == AUDIO_FRAMES_IN_BUFFER)
			{
				audio.cnt_i2s = 0;
			}
		}
		else
		{
			++feedback.flagE;
		}
		audio_out_drv.write_dma_txbuf(&buff_usb[audio.cnt_i2s][0], buff_usb_size[audio.cnt_i2s]);
	}
}

//--------------------------------------------
static usbd_respond uac_setconf(usbd_device *dev, uint8_t cfg)
{
	switch (cfg)
	{
	case 0:
		// deconfiguring device
		usbd_ep_deconfig(dev, UAC_RXD_EP);
		usbd_ep_deconfig(dev, UAC_TXD_EP);
		usbd_reg_endpoint(dev, UAC_RXD_EP, NULL);
		usbd_reg_endpoint(dev, UAC_TXD_EP, NULL);
		return usbd_ack;
	case 1:
        // configuring device
		usbd_ep_config(dev, UAC_RXD_EP, USB_EPTYPE_ISOCHRONOUS, UAC_DATA_SZ);
		usbd_ep_config(dev, UAC_TXD_EP, USB_EPTYPE_ISOCHRONOUS, UAC_FEEDBACK_SZ);
		usbd_reg_endpoint(dev, UAC_RXD_EP, uac_callback);
		usbd_reg_endpoint(dev, UAC_TXD_EP, uac_callback);
		return usbd_ack;
	default:
		return usbd_fail;
	}
}

//--------------------------------------------
void usb_uac_i2s_init(void)
{
	audio_out_drv.init();
	audio_out_drv.init_dma_txbuf(i2s_tx_complete_callback);

	usbd_hw_init(&udev);
	usbd_init(&udev, &usbd_hw, UAC_EP0_SIZE, ubuf, sizeof(ubuf));
	usbd_reg_control(&udev, uac_control);
	usbd_reg_config(&udev, uac_setconf);
	usbd_reg_descr(&udev, uac_getdesc);
}

//--------------------------------------------
void usb_uac_i2s_loop(void)
{
	usbd_enable(&udev, true);
	usbd_connect(&udev, true);
	while (1);
}
