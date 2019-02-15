/*
* Copyright (c) 2018 Vladimir Alemasov
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

#ifndef USB_UAC2_H_
#define USB_UAC2_H_

// USB UAC2 class
// Generic USB audio class definitions
// This module is based on
// Universal Serial Bus Device Class Definition for Audio Devices Release 2.0 May 31, 2006
// ADC-2 refers to the USB Device Class Definition for Audio Devices, Release 2.0
// FMT-2 refers to the Audio Data Formats specification, Release 2.0
// TT-2 refers to the Terminal Types specification, Release 2.0

// Audio Interface Subclass Codes (ADC-2 Table A-5)
#define USB_SUBCLASS_AUDIOCONTROL                      0x01
#define USB_SUBCLASS_AUDIOSTREAMING                    0x02
#define USB_SUBCLASS_MIDISTREAMING                     0x03

// Audio Interface Protocol Codes (ADC-2 Table A-6)
#define USB_PROTO_IP_VERSION_02_00                     0x20

// Audio Class-Specific AC Interface Descriptor Subtypes (ADC-2 Table A-9)
#define USB_DTYPE_UAC2_AC_HEADER                       0x01
#define USB_DTYPE_UAC2_AC_INPUT_TERMINAL               0x02
#define USB_DTYPE_UAC2_AC_OUTPUT_TERMINAL              0x03
#define USB_DTYPE_UAC2_AC_MIXER_UNIT                   0x04
#define USB_DTYPE_UAC2_AC_SELECTOR_UNIT                0x05
#define USB_DTYPE_UAC2_AC_FEATURE_UNIT                 0x06
#define USB_DTYPE_UAC2_AC_EFFECT_UNIT                  0x07
#define USB_DTYPE_UAC2_AC_PROCESSING_UNIT              0x08
#define USB_DTYPE_UAC2_AC_EXTENSION_UNIT               0x09
#define USB_DTYPE_UAC2_AC_CLOCK_SOURCE                 0x0A
#define USB_DTYPE_UAC2_AC_CLOCK_SELECTOR               0x0B
#define USB_DTYPE_UAC2_AC_CLOCK_MULTIPLIER             0x0C
#define USB_DTYPE_UAC2_AC_SAMPLE_RATE_CONVERTER        0x0D

// Audio Function Category Codes (ADC-2 Table A-7)
#define USB_UAC2_AFCC_DESKTOP_SPEAKER                  0x01
#define USB_UAC2_AFCC_HOME_THEATER                     0x02
#define USB_UAC2_AFCC_MICROPHONE                       0x03
#define USB_UAC2_AFCC_HEADSET                          0x04
#define USB_UAC2_AFCC_TELEPHONE                        0x05
#define USB_UAC2_AFCC_CONVERTER                        0x06
#define USB_UAC2_AFCC_VOICE_SOUND_RECORDER             0x07
#define USB_UAC2_AFCC_IO_BOX                           0x08
#define USB_UAC2_AFCC_MUSICAL_INSTRUMENT               0x09
#define USB_UAC2_AFCC_PRO_AUDIO                        0x0A
#define USB_UAC2_AFCC_AUDIO_VIDEO                      0x0B
#define USB_UAC2_AFCC_CONTROL_PANEL                    0x0C
#define USB_UAC2_AFCC_OTHER                            0xFF

// USB Terminal Types (TT-2 Table 2-1)
#define USB_UAC2_TT_USB_STREAMING                      0x0101
#define USB_UAC2_TT_VENDOR_SPECIFIC                    0x01FF
// Input Terminal Types (TT-2 Table 2-2)
#define USB_UAC2_ITT_MICROPHONE                        0x0201
#define USB_UAC2_ITT_DESKTOP_MICROPHONE                0x0202
#define USB_UAC2_ITT_PERSONAL_MICROPHONE               0x0203
#define USB_UAC2_ITT_OMNIDIRECTIONAL_MICROPHONE        0x0204
#define USB_UAC2_ITT_MICROPHONE_ARRAY                  0x0205
#define USB_UAC2_ITT_PROCESSING_MICROPHONE_ARRAY       0x0206
// Output Terminal Types (TT-2 Table 2-3)
#define USB_UAC2_OTT_SPEAKER                           0x0301
#define USB_UAC2_OTT_HEADPHONES                        0x0302
#define USB_UAC2_OTT_HEAD_MOUNTED_DISPLAY_AUDIO        0x0303
#define USB_UAC2_OTT_DESKTOP_SPEAKER                   0x0304
#define USB_UAC2_OTT_ROOM_SPEAKER                      0x0305
#define USB_UAC2_OTT_COMMUNICATION_SPEAKER             0x0306
#define USB_UAC2_OTT_LOW_FREQUENCY_EFFECTS_SPEAKER     0x0307

// Clock Source Control Selectors (ADC-2 Table A-17)
#define USB_UAC2_CS_SAM_FREQ_CONTROL                   0x01
#define USB_UAC2_CS_CLOCK_VALID_CONTROL                0x02
// Clock Source Descriptor bmAttributes bits (ADC-2 Table 4-6)
#define USB_UAC2_CS_BMA_EXTERNAL_CLOCK                 0x00
#define USB_UAC2_CS_BMA_INTERNAL_FIXED_CLOCK           0x01
#define USB_UAC2_CS_BMA_INTERNAL_VARIABLE_CLOCK        0x02
#define USB_UAC2_CS_BMA_INTERNAL_PROGRAMMABLE_CLOCK    0x03
#define USB_UAC2_CS_BMA_CLOCK_SYNCHRONIZED_TO_SOF      0x04
// Clock Source Descriptor bmControls bits (ADC-2 Table 4-6)
// If a certain Control is not present then the bit pair must be set to 0b00.
// If a Control is present but read-only, the bit pair must be set to 0b01. If a Control is also Host
// programmable, the bit pair must be set to 0b11. The value 0b10 is not allowed.
#define USB_UAC2_CS_BMC_SAM_FREQ_CONTROL_RO            0x01
#define USB_UAC2_CS_BMC_SAM_FREQ_CONTROL_HP            0x03
#define USB_UAC2_CS_BMC_CLOCK_VALID_CONTROL_RO         0x04
#define USB_UAC2_CS_BMC_CLOCK_VALID_CONTROL_HP         0x0C

// Feature Unit Control Selectors (ADC-2 Table A-23)
#define USB_UAC2_FU_MUTE_CONTROL                       0x01
#define USB_UAC2_FU_VOLUME_CONTROL                     0x02
#define USB_UAC2_FU_BASS_CONTROL                       0x03
#define USB_UAC2_FU_MID_CONTROL                        0x04
#define USB_UAC2_FU_TREBLE_CONTROL                     0x05
#define USB_UAC2_FU_GRAPHIC_EQUALIZER_CONTROL          0x06
#define USB_UAC2_FU_AUTOMATIC_GAIN_CONTROL             0x07
#define USB_UAC2_FU_DELAY_CONTROL                      0x08
#define USB_UAC2_FU_BASS_BOOST_CONTROL                 0x09
#define USB_UAC2_FU_LOUDNESS_CONTROL                   0x0A
#define USB_UAC2_FU_INPUT_GAIN_CONTROL                 0x0B
#define USB_UAC2_FU_INPUT_GAIN_PAD_CONTROL             0x0C
#define USB_UAC2_FU_PHASE_INVERTER_CONTROL             0x0D
#define USB_UAC2_FU_UNDERFLOW_CONTROL                  0x0E
#define USB_UAC2_FU_OVERFLOW_CONTROL                   0x0F
#define USB_UAC2_FU_LATENCY_CONTROL                    0x10
// Feature Unit Descriptor bmaControls bits (ADC-2 Table 4-13)
// If a certain Control is not present then the bit pair must be set to 0b00.
// If a Control is present but read-only, the bit pair must be set to 0b01. If a Control is also Host
// programmable, the bit pair must be set to 0b11. The value 0b10 is not allowed.
#define USB_UAC2_FU_BMAC_MUTE_CONTROL_RO               0x00000001
#define USB_UAC2_FU_BMAC_MUTE_CONTROL_HP               0x00000003
#define USB_UAC2_FU_BMAC_VOLUME_CONTROL_RO             0x00000004
#define USB_UAC2_FU_BMAC_VOLUME_CONTROL_HP             0x0000000C
#define USB_UAC2_FU_BMAC_BASS_CONTROL_RO               0x00000010
#define USB_UAC2_FU_BMAC_BASS_CONTROL_HP               0x00000030
#define USB_UAC2_FU_BMAC_MID_CONTROL_RO                0x00000040
#define USB_UAC2_FU_BMAC_MID_CONTROL_HP                0x000000C0
#define USB_UAC2_FU_BMAC_TREBLE_CONTROL_RO             0x00000100
#define USB_UAC2_FU_BMAC_TREBLE_CONTROL_HP             0x00000300
#define USB_UAC2_FU_BMAC_GRAPHIC_EQUALIZER_CONTROL_RO  0x00000400
#define USB_UAC2_FU_BMAC_GRAPHIC_EQUALIZER_CONTROL_HP  0x00000C00
#define USB_UAC2_FU_BMAC_AUTOMATIC_GAIN_CONTROL_RO     0x00001000
#define USB_UAC2_FU_BMAC_AUTOMATIC_GAIN_CONTROL_HP     0x00003000
#define USB_UAC2_FU_BMAC_DELAY_CONTROL_RO              0x00004000
#define USB_UAC2_FU_BMAC_DELAY_CONTROL_HP              0x0000C000
#define USB_UAC2_FU_BMAC_BASS_BOOST_CONTROL_RO         0x00010000
#define USB_UAC2_FU_BMAC_BASS_BOOST_CONTROL_HP         0x00030000
#define USB_UAC2_FU_BMAC_LOUDNESS_CONTROL_RO           0x00040000
#define USB_UAC2_FU_BMAC_LOUDNESS_CONTROL_HP           0x000C0000
#define USB_UAC2_FU_BMAC_INPUT_GAIN_CONTROL_RO         0x00100000
#define USB_UAC2_FU_BMAC_INPUT_GAIN_CONTROL_HP         0x00300000
#define USB_UAC2_FU_BMAC_INPUT_GAIN_PAD_CONTROL_RO     0x00400000
#define USB_UAC2_FU_BMAC_INPUT_GAIN_PAD_CONTROL_HP     0x00C00000
#define USB_UAC2_FU_BMAC_PHASE_INVERTER_CONTROL_RO     0x01000000
#define USB_UAC2_FU_BMAC_PHASE_INVERTER_CONTROL_HP     0x03000000
#define USB_UAC2_FU_BMAC_UNDERFLOW_CONTROL_RO          0x04000000
#define USB_UAC2_FU_BMAC_UNDERFLOW_CONTROL_HP          0x0C000000
#define USB_UAC2_FU_BMAC_OVERFLOW_CONTROL_RO           0x10000000
#define USB_UAC2_FU_BMAC_OVERFLOW_CONTROL_HP           0x30000000

// Audio Class-Specific AS Interface Descriptor Subtypes (ADC-2 Table A-10)
#define USB_DTYPE_UAC2_AS_GENERAL                      0x01
#define USB_DTYPE_UAC2_AS_FORMAT_TYPE                  0x02
#define USB_DTYPE_UAC2_AS_ENCODER                      0x03
#define USB_DTYPE_UAC2_AS_DECODER                      0x04

// Format Type Codes (FMT-2 Table A-1)
#define USB_UAC2_DATA_FORMAT_TYPE_I                    0x01
#define USB_UAC2_DATA_FORMAT_TYPE_II                   0x02
#define USB_UAC2_DATA_FORMAT_TYPE_III                  0x03
#define USB_UAC2_DATA_FORMAT_TYPE_IV                   0x04
#define USB_UAC2_DATA_EXT_FORMAT_TYPE_I                0x81
#define USB_UAC2_DATA_EXT_FORMAT_TYPE_II               0x82
#define USB_UAC2_DATA_EXT_FORMAT_TYPE_III              0x83

// Audio Data Format Type I Bit Allocations (FMT-2 Table A-2)
#define USB_UAC2_DATA_FT_I_PCM                         0x0001
#define USB_UAC2_DATA_FT_I_PCM8                        0x0002
#define USB_UAC2_DATA_FT_I_IEEE_FLOAT                  0x0004
#define USB_UAC2_DATA_FT_I_ALAW                        0x0008
#define USB_UAC2_DATA_FT_I_MULAW                       0x0010
#define USB_UAC2_DATA_FT_I_RAW_DATA                    0x8000
// Audio Data Format Type II Bit Allocations (FMT-2 Table A-3)
#define USB_UAC2_DATA_FT_II_MPEG                       0x0001
#define USB_UAC2_DATA_FT_II_AC3                        0x0002
#define USB_UAC2_DATA_FT_II_WMA                        0x0004
#define USB_UAC2_DATA_FT_II_DTS                        0x0008
#define USB_UAC2_DATA_FT_II_RAW_DATA                   0x8000

// Audio Class-Specific Endpoint Descriptor Subtypes (ADC-2 Table A-13)
#define USB_DTYPE_UAC2_EP_GENERAL                      0x01

// Audio Class-Specific Request Codes (ADC-2 Table A-14)
#define USB_UAC2_CUR                                   0x01
#define USB_UAC2_RANGE                                 0x02
#define USB_UAC2_MEM                                   0x03

#pragma pack(push, 1)

// Class-Specific AC Interface Descriptor (ADC-2 Table 4-5)
struct usb_uac2_ac_header_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint16_t bcdADC;
	uint8_t bCategory;
	uint16_t wTotalLength;
	uint8_t bmControls;
};

// Clock Source Descriptor (ADC-2 Table 4-6)
struct usb_uac2_clock_source_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bClockID;
	uint8_t bmAttributes;
	uint8_t bmControls;
	uint8_t bAssocTerminal;
	uint8_t iClockSource;
};

// Input Terminal Descriptor (ADC-2 Table 4-9)
struct usb_uac2_input_terminal_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t bCSourceID;
	uint8_t bNrChannels;
	uint32_t bmChannelConfig;
	uint8_t iChannelNames;
	uint16_t bmControls;
	uint8_t iTerminal;
};

// Feature Unit Descriptor (ADC-2 Table 4-13)
struct usb_uac2_feature_unit_sz2x32_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint32_t bmaControls[2];
	uint8_t iFeature;
};
struct usb_uac2_feature_unit_sz3x32_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint32_t bmaControls[3];
	uint8_t iFeature;
};

// Output Terminal Descriptor (ADC-2 Table 4-10)
struct usb_uac2_output_terminal_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t bSourceID;
	uint8_t bCSourceID;
	uint16_t bmControls;
	uint8_t iTerminal;
};

// Class-Specific AS Interface Descriptor (ADC-2 Table 4-27)
struct usb_uac2_as_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalLink;
	uint8_t bmControls;
	uint8_t bFormatType;
	uint32_t bmFormats;
	uint8_t bNrChannels;
	uint32_t bmChannelConfig;
	uint8_t iChannelNames;
};

// Type I Format Type Descriptor (FMT-2 Table 2-2)
struct usb_uac2_as_format_type1_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bFormatType;
	uint8_t bSubslotSize;
	uint8_t bBitResolution;
};

// Standard AS Isochronous Audio Data Endpoint Descriptor (ADC-2 Table 4-33)
struct usb_uac2_std_iso_endpoint_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
};

// Class-Specific AS Isochronous Audio Data Endpoint Descriptor (ADC-2 Table 4-34)
struct usb_uac2_cs_iso_endpoint_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmAttributes;
	uint8_t bmControls;
	uint8_t bLockDelayUnits;
	uint16_t wLockDelay;
};

// 1-byte Control CUR Parameter Block (ADC-2 Table 5-2)
struct usb_uac2_layout_1_CUR_parameter_block
{
	uint8_t bCUR;
};

// 4-byte Control CUR Parameter Block (ADC-2 Table 5-6)
struct usb_uac2_layout_3_CUR_parameter_block
{
	uint32_t dCUR;
};

// 4-byte Control RANGE Parameter Block (ADC-2 Table 5-7)
struct usb_uac2_layout_3_RANGE_parameter_sz3x32_block
{
	uint16_t wNumSubRanges;
	uint32_t dMIN;
	uint32_t dMAX;
	uint32_t dRES;
};

#pragma pack(pop)

#endif /* USB_UAC2_H_ */

