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

// USB UVC class
// This module is based on
// Universal Serial Bus Specification Revision 2.0 April 27, 2000
// Universal Serial Bus Device Class Definition for Video Devices Revision 1.1 June 1, 2005

#include "platform.h"
#include "usbd_core.h"
#include "usb_std.h"
#include "hal-usbd-init.h"
#include "usb-uvc.h"
#include "imgsensor-drv.h"
#include <string.h>

//--------------------------------------------
extern const imgsensor_drv_t cam_drv;

#if defined USBD_OTGHS
extern const struct usbd_driver usbd_otghs;
#define usbd_hw usbd_otghs
#endif

//--------------------------------------------
#define VF_HEIGHT                   240
#define VF_WIDTH                    320
#define VF_BITS_PER_PIXEL           16
#define VF_SIZE_IN_BITS             (VF_HEIGHT * VF_WIDTH * VF_BITS_PER_PIXEL)
#define VF_SIZE_IN_BYTES            (VF_SIZE_IN_BITS / 8)
#define VF_FPS                      30
#define VF_BITRATE                  (VF_SIZE_IN_BITS * VF_FPS)
#define VF_INTERVAL                 (10000000 / VF_FPS)

//--------------------------------------------
#define UVC_EP0_SIZE                64
#define UVC_TXD_EP                  0x81
#define UVC_DATA_SZ                 1024

//--------------------------------------------
#pragma pack(push, 1)
typedef struct uvc_config
{
	struct usb_config_descriptor                        config;
	struct usb_iad_descriptor                           iad;
	struct usb_interface_descriptor                     vc;
	struct usb_uvc_vc_header_desc                       vc_hdr;
	struct usb_uvc_input_terminal_camera_sz3_desc       vc_itcd;
	struct usb_uvc_output_terminal_desc                 vc_otd;
	struct usb_interface_descriptor                     vs_0;
	struct usb_uvc_vs_input_header_sz1_desc             vs_input_hdr;
	struct usb_uvc_vs_format_uncompressed_desc          vs_format_uncmp;
	struct usb_uvc_vs_frame_uncompressed_cont_desc      vs_frame_uncmp;
	struct usb_uvc_vs_color_matching_desc               vs_color;
	struct usb_interface_descriptor                     vs_1;
	struct usb_endpoint_descriptor                      data_eptx;
} uvc_config_t;
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
	.bMaxPacketSize0    = UVC_EP0_SIZE,
	.idVendor           = CPU_TO_LE16(0x0483),
	.idProduct          = CPU_TO_LE16(0x5740),
	.bcdDevice          = CPU_TO_LE16(VERSION_BCD(1,0,0)),
	.iManufacturer      = 1,
	.iProduct           = 2,
	.iSerialNumber      = INTSERIALNO_DESCRIPTOR,
	.bNumConfigurations = 1,
};

//--------------------------------------------
static const uvc_config_t config_desc =
{
	.config =
	{
		.bLength                   = sizeof(struct usb_config_descriptor),
		.bDescriptorType           = USB_DTYPE_CONFIGURATION,
		.wTotalLength              = CPU_TO_LE16(sizeof(uvc_config_t)),
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
		.bFunctionClass            = USB_CLASS_VIDEO,
		.bFunctionSubClass         = USB_SUBCLASS_VIDEO_INTERFACE_COLLECTION,
		.bFunctionProtocol         = USB_PROTO_NONE,
		.iFunction                 = 2,
	},
	.vc =
	{
		.bLength                   = sizeof(struct usb_interface_descriptor),
		.bDescriptorType           = USB_DTYPE_INTERFACE,
		.bInterfaceNumber          = 0,
		.bAlternateSetting         = 0,
		.bNumEndpoints             = 0,
		.bInterfaceClass           = USB_CLASS_VIDEO,
		.bInterfaceSubClass        = USB_SUBCLASS_VIDEOCONTROL,
		.bInterfaceProtocol        = USB_PROTO_NONE,
		.iInterface                = NO_DESCRIPTOR,
	},
	.vc_hdr =
	{
		.bLength                   = sizeof(struct usb_uvc_vc_header_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UVC_VC_HEADER,
		.bcdUVC                    = CPU_TO_LE16(VERSION_BCD(1,0,0)),
		.wTotalLength              = CPU_TO_LE16(sizeof(struct usb_uvc_vc_header_desc) +
	                                             sizeof(struct usb_uvc_input_terminal_camera_sz3_desc) +
	                                             sizeof(struct usb_uvc_output_terminal_desc)),
		.dwClockFrequency          = CPU_TO_LE32(0x005B8D80), // 6MHz ???????
		.bInCollection             = 1,
		.baInterfaceNr1            = 1,
	},
	.vc_itcd =
	{
		.bLength                   = sizeof(struct usb_uvc_input_terminal_camera_sz3_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UVC_VC_INPUT_TERMINAL,
		.bTerminalID               = 1,
		.wTerminalType             = CPU_TO_LE16(USB_UVC_ITT_CAMERA),
		.bAssocTerminal            = 0,
		.iTerminal                 = 0,
		.wObjectiveFocalLengthMin  = CPU_TO_LE16(0x0000),
		.wObjectiveFocalLengthMax  = CPU_TO_LE16(0x0000),
		.wOcularFocalLength        = CPU_TO_LE16(0x0000),
		.bControlSize              = 3,
		.bmControls[0]             = 0x00,
		.bmControls[1]             = 0x00,
		.bmControls[2]             = 0x00,
	},
	.vc_otd =
	{
		.bLength                   = sizeof(struct usb_uvc_output_terminal_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UVC_VC_OUTPUT_TERMINAL,
		.bTerminalID               = 2,
		.wTerminalType             = CPU_TO_LE16(USB_UVC_TT_STREAMING),
		.bAssocTerminal            = 0,
		.bSourceID                 = 1,
		.iTerminal                 = 0,
	},
	.vs_0 =
	{
		.bLength                   = sizeof(struct usb_interface_descriptor),
		.bDescriptorType           = USB_DTYPE_INTERFACE,
		.bInterfaceNumber          = 1,
		.bAlternateSetting         = 0,
		.bNumEndpoints             = 0,
		.bInterfaceClass           = USB_CLASS_VIDEO,
		.bInterfaceSubClass        = USB_SUBCLASS_VIDEOSTREAMING,
		.bInterfaceProtocol        = USB_PROTO_NONE,
		.iInterface                = NO_DESCRIPTOR,
	},
	.vs_input_hdr =
	{
		.bLength                   = sizeof(struct usb_uvc_vs_input_header_sz1_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UVC_VS_INPUT_HEADER,
		.bNumFormats               = 1,
		.wTotalLength              = CPU_TO_LE16(sizeof(struct usb_uvc_vs_input_header_sz1_desc) +
	                                             sizeof(struct usb_uvc_vs_format_uncompressed_desc) +
	                                             sizeof(struct usb_uvc_vs_frame_uncompressed_cont_desc) +
	                                             sizeof(struct usb_uvc_vs_color_matching_desc)),
		.bEndpointAddress          = UVC_TXD_EP,
		.bmInfo                    = 0x00,
		.bTerminalLink             = 2,
		.bStillCaptureMethod       = 0,
		.bTriggerSupport           = 1,
		.bTriggerUsage             = 0,
		.bControlSize              = 1,
		.bmaControls[0]            = 0,
	},
	.vs_format_uncmp =
	{
		.bLength                   = sizeof(struct usb_uvc_vs_format_uncompressed_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UVC_VS_FORMAT_UNCOMPRESSED,
		.bFormatIndex              = 1,
		.bNumFrameDescriptors      = 1,
		.guidFormat                =
		{
			0x59, 0x55, 0x59, 0x32, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71, // yuy2
		},
		.bBitsPerPixel             = VF_BITS_PER_PIXEL,
		.bDefaultFrameIndex        = 1,
		.bAspectRatioX             = 0,
		.bAspectRatioY             = 0,
		.bmInterlaceFlags          = 0,
		.bCopyProtect              = 0,
	},
	.vs_frame_uncmp =
	{
		.bLength                   = sizeof(struct usb_uvc_vs_frame_uncompressed_cont_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UVC_VS_FRAME_UNCOMPRESSED,
		.bFrameIndex               = 1,
		.bmCapabilities            = 0,
		.wWidth                    = CPU_TO_LE16(VF_WIDTH),
		.wHeight                   = CPU_TO_LE16(VF_HEIGHT),
		.dwMinBitRate              = CPU_TO_LE32(VF_BITRATE),
		.dwMaxBitRate              = CPU_TO_LE32(VF_BITRATE),
		.dwMaxVideoFrameBufferSize = CPU_TO_LE32(VF_SIZE_IN_BYTES),
		.dwDefaultFrameInterval    = CPU_TO_LE32(VF_INTERVAL),
		.bFrameIntervalType        = 0,
		.dwMinFrameInterval        = CPU_TO_LE32(VF_INTERVAL),
		.dwMaxFrameInterval        = CPU_TO_LE32(VF_INTERVAL),
		.dwFrameIntervalStep       = CPU_TO_LE32(0),
	},
	.vs_color =
	{
		.bLength                   = sizeof(struct usb_uvc_vs_color_matching_desc),
		.bDescriptorType           = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType        = USB_DTYPE_UVC_VS_COLORFORMAT,
		.bColorPrimaries           = 1,
		.bTransferCharacteristics  = 1,
		.bMatrixCoefficients       = 4,
	},
	.vs_1 =
	{
		.bLength                   = sizeof(struct usb_interface_descriptor),
		.bDescriptorType           = USB_DTYPE_INTERFACE,
		.bInterfaceNumber          = 1,
		.bAlternateSetting         = 1,
		.bNumEndpoints             = 1,
		.bInterfaceClass           = USB_CLASS_VIDEO,
		.bInterfaceSubClass        = USB_SUBCLASS_VIDEOSTREAMING,
		.bInterfaceProtocol        = USB_PROTO_NONE,
		.iInterface                = NO_DESCRIPTOR,
	},
	.data_eptx =
	{
		.bLength                   = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType           = USB_DTYPE_ENDPOINT,
		.bEndpointAddress          = UVC_TXD_EP,
		.bmAttributes              = USB_EPTYPE_ISOCHRONOUS,
		.wMaxPacketSize            = CPU_TO_LE16(UVC_DATA_SZ),
		.bInterval                 = 1,
	},
};

//--------------------------------------------
static const struct usb_string_descriptor lang_desc = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC("Homemcu firmware examples");
static const struct usb_string_descriptor prod_desc_en = USB_STRING_DESC("UVC camera demo");
static const struct usb_string_descriptor *const dtable[] =
{
	&lang_desc,
	&manuf_desc_en,
	&prod_desc_en,
};

//--------------------------------------------
static struct usb_uvc_vs_control vs_probe_ctrl =
{
	.bmHint                   = 0,
	.bFormatIndex             = 1,
	.bFrameIndex              = 1,
	.dwFrameInterval          = CPU_TO_LE32(VF_INTERVAL),
	.wKeyFrameRate            = CPU_TO_LE16(0),
	.wPFrameRate              = CPU_TO_LE16(0),
	.wCompQuality             = CPU_TO_LE16(0),
	.wCompWindowSize          = CPU_TO_LE16(0),
	.wDelay                   = CPU_TO_LE16(0),
	.dwMaxVideoFrameSize      = CPU_TO_LE32(VF_SIZE_IN_BYTES),
	.dwMaxPayloadTransferSize = CPU_TO_LE32(0),
};
static struct usb_uvc_vs_control vs_commit_ctrl =
{
	.bmHint                   = 0,
	.bFormatIndex             = 1,
	.bFrameIndex              = 1,
	.dwFrameInterval          = CPU_TO_LE32(VF_INTERVAL),
	.wKeyFrameRate            = CPU_TO_LE16(0),
	.wPFrameRate              = CPU_TO_LE16(0),
	.wCompQuality             = CPU_TO_LE16(0),
	.wCompWindowSize          = CPU_TO_LE16(0),
	.wDelay                   = CPU_TO_LE16(0),
	.dwMaxVideoFrameSize      = CPU_TO_LE32(VF_SIZE_IN_BYTES),
	.dwMaxPayloadTransferSize = CPU_TO_LE32(0),
};

//--------------------------------------------
static usbd_device udev;
static uint8_t iface_num;
static uint8_t altset_num;
static volatile bool lock;
// Due to use with USB FIFO and/or DMA, the data buffers below must be 32-bit aligned:
#define USB_CTRL_BUFF_SZ 128
static uint32_t ubuf[(USB_CTRL_BUFF_SZ + 3) / sizeof(uint32_t)];
static uint32_t buff_usb[UVC_DATA_SZ / sizeof(uint32_t)];
static uint32_t framebuf[(VF_SIZE_IN_BYTES + 3) / sizeof(uint32_t)];

//--------------------------------------------
static usbd_respond uvc_getdesc(usbd_ctlreq *req, void **address, uint16_t *length)
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
static void vs_control_decode(const uint8_t *data, struct usb_uvc_vs_control *ctrl)
{
	ctrl->bmHint = le16_to_cpup((uint16_t *)&data[0]);
	ctrl->bFormatIndex = data[2];
	ctrl->bFrameIndex = data[3];
	ctrl->dwFrameInterval = le32_to_cpup((uint32_t *)&data[4]);
	ctrl->wKeyFrameRate = le16_to_cpup((uint16_t *)&data[8]);
	ctrl->wPFrameRate = le16_to_cpup((uint16_t *)&data[10]);
	ctrl->wCompQuality = le16_to_cpup((uint16_t *)&data[12]);
	ctrl->wCompWindowSize = le16_to_cpup((uint16_t *)&data[14]);
	ctrl->wDelay = le16_to_cpup((uint16_t *)&data[16]);
	ctrl->dwMaxVideoFrameSize = le32_to_cpup((uint32_t *)&data[18]);
	ctrl->dwMaxPayloadTransferSize = le32_to_cpup((uint32_t *)&data[22]);
}

//--------------------------------------------
static void vs_control_encode(struct usb_uvc_vs_control *ctrl, uint8_t *data)
{
	*(uint16_t *)&data[0] = cpu_to_le16(ctrl->bmHint);
	data[2] = ctrl->bFormatIndex;
	data[3] = ctrl->bFrameIndex;
	*(uint32_t *)&data[4] = cpu_to_le32(ctrl->dwFrameInterval);
	*(uint16_t *)&data[8] = cpu_to_le16(ctrl->wKeyFrameRate);
	*(uint16_t *)&data[10] = cpu_to_le16(ctrl->wPFrameRate);
	*(uint16_t *)&data[12] = cpu_to_le16(ctrl->wCompQuality);
	*(uint16_t *)&data[14] = cpu_to_le16(ctrl->wCompWindowSize);
	*(uint16_t *)&data[16] = cpu_to_le16(ctrl->wDelay);
	*(uint32_t *)&data[18] = cpu_to_le32(ctrl->dwMaxVideoFrameSize);
	*(uint32_t *)&data[22] = cpu_to_le32(ctrl->dwMaxPayloadTransferSize);
}

//--------------------------------------------
static void vs_data_send(usbd_device *dev)
{
	static uint32_t packets_cnt;
	static uint32_t last_packet_num;
	static uint32_t last_data_size;
	static uint32_t picture_pos;
	static struct usb_uvc_vs_data_uncompressed_header hdr = { 2 , 0 };

	if (packets_cnt == 0)
	{
		hdr.BFH &= ~USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_EOF;
		hdr.BFH ^= USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_FID;
		picture_pos = 0;
		last_packet_num = VF_SIZE_IN_BYTES % (UVC_DATA_SZ - 2) ? VF_SIZE_IN_BYTES / (UVC_DATA_SZ - 2) : VF_SIZE_IN_BYTES / (UVC_DATA_SZ - 2) - 1;
		last_data_size = VF_SIZE_IN_BYTES - ((UVC_DATA_SZ - 2) * last_packet_num);
	}
	memcpy((uint8_t *)buff_usb, &hdr, sizeof(hdr));
	if (packets_cnt < last_packet_num)
	{
		memcpy((uint8_t *)buff_usb + sizeof(hdr), (uint8_t *)framebuf + picture_pos, UVC_DATA_SZ - sizeof(hdr));
		if (usbd_ep_write(dev, UVC_TXD_EP, (uint8_t *)buff_usb, UVC_DATA_SZ) != -1)
		{
			picture_pos += UVC_DATA_SZ - sizeof(hdr);
			packets_cnt++;
		}
	}
	else
	{
		hdr.BFH |= USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_EOF;
		memcpy((uint8_t *)buff_usb + sizeof(hdr), (uint8_t *)framebuf + picture_pos, last_data_size);
		if (usbd_ep_write(dev, UVC_TXD_EP, (uint8_t *)buff_usb, last_data_size + sizeof(hdr)) != -1)
		{
			packets_cnt = 0;
		}
	}
}

//--------------------------------------------
static void uvc_sof_callback(usbd_device *dev, uint8_t event, uint8_t ep)
{
	static uint32_t start_ticks;
	if (!start_ticks)
	{
		start_ticks = get_platform_counter();
	}
	else
	{
		// 15 ms pause
		if (get_platform_counter() - start_ticks > 15)
		{
			// start
			lock = true;
			vs_data_send(dev);
		}
	}
}

//--------------------------------------------
static usbd_respond uvc_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback)
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
				// start to send video data to the isochronous endpoint
				usbd_reg_event(dev, usbd_evt_sof, uvc_sof_callback);
				lock = false;
			}
			if (iface_num == 1 && altset_num == 0)
			{
				// stop to send video data to the isochronous endpoint
				usbd_reg_event(dev, usbd_evt_sof, NULL);
				lock = true;
			}
			return usbd_ack;
		}
		return usbd_fail;
	}
	switch (req->bRequest)
	{
	case USB_UVC_SET_CUR:
		if ((req->wValue >> 8) == USB_UVC_VS_PROBE_CONTROL)
		{
			vs_control_decode(req->data, &vs_probe_ctrl);
		}
		else if ((req->wValue >> 8) == USB_UVC_VS_COMMIT_CONTROL)
		{
			vs_control_decode(req->data, &vs_commit_ctrl);
		}
		return usbd_ack;
	case USB_UVC_GET_CUR:
		if ((req->wValue >> 8) == USB_UVC_VS_PROBE_CONTROL)
		{
			vs_control_encode(&vs_probe_ctrl, (uint8_t *)buff_usb);
		}
		else if ((req->wValue >> 8) == USB_UVC_VS_COMMIT_CONTROL)
		{
			vs_control_encode(&vs_commit_ctrl, (uint8_t *)buff_usb);
		}
		dev->status.data_ptr = buff_usb;
		dev->status.data_count = sizeof(struct usb_uvc_vs_control);
		return usbd_ack;
	case USB_UVC_GET_MIN:
		return usbd_ack;
	case USB_UVC_GET_MAX:
		return usbd_ack;
	case USB_UVC_GET_LEN:
		return usbd_ack;
	case USB_UVC_GET_INFO:
		return usbd_ack;
	case USB_UVC_GET_DEF:
		return usbd_ack;
	default:
		return usbd_fail;
	}
}

//--------------------------------------------
static void uvc_callback(usbd_device *dev, uint8_t event, uint8_t ep)
{
	switch (event)
	{
	case usbd_evt_eptx:
		lock = false;
		break;
	default:
		break;
	}
}

//--------------------------------------------
static usbd_respond uvc_setconf(usbd_device *dev, uint8_t cfg)
{
	switch (cfg)
	{
	case 0:
		// deconfiguring device
		usbd_ep_deconfig(dev, UVC_TXD_EP);
		usbd_reg_endpoint(dev, UVC_TXD_EP, NULL);
		return usbd_ack;
	case 1:
        // configuring device
		usbd_ep_config(dev, UVC_TXD_EP, USB_EPTYPE_ISOCHRONOUS, UVC_DATA_SZ);
		usbd_reg_endpoint(dev, UVC_TXD_EP, uvc_callback);
		return usbd_ack;
	default:
		return usbd_fail;
	}
}

//--------------------------------------------
void usb_uvc_camera_init(void)
{
	cam_drv.init(1);
	cam_drv.init_dma_buf();

	usbd_hw_init(&udev);
	usbd_init(&udev, &usbd_hw, UVC_EP0_SIZE, ubuf, sizeof(ubuf));
	usbd_reg_control(&udev, uvc_control);
	usbd_reg_config(&udev, uvc_setconf);
	usbd_reg_descr(&udev, uvc_getdesc);
}

//--------------------------------------------
void usb_uvc_camera_loop(void)
{
	lock = true;
	usbd_enable(&udev, true);
	usbd_connect(&udev, true);
	while (1)
	{
		if (!lock)
		{
			// camera => (by DMA) => memory frame buffer
			cam_drv.read_dma_buf((uint8_t *)framebuf, VF_SIZE_IN_BYTES);
		}
	}
}
