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

#include "platform.h"
#include "usbd_core.h"
#include "usb_std.h"
#include "usbd-hw-init.h"
#include "usb_cdc.h"
#include <string.h>

//--------------------------------------------
#ifdef USBD_OTGFS
extern const struct usbd_driver usbd_otgfs;
#define usbd_hw usbd_otgfs
#elif defined USBD_OTGHS || defined USBD_OTGHS_FS
extern const struct usbd_driver usbd_otghs;
#define usbd_hw usbd_otghs
#endif

//--------------------------------------------
#define CDC_RXD_EP      0x01
#define CDC_TXD_EP      0x82
#define CDC_NTF_EP      0x83
#define CDC_EP0_SIZE    64
#ifdef USBD_FULL_SPEED
#define CDC_DATA_SZ     64
#else
#define CDC_DATA_SZ     512
#endif
#define CDC_NTF_SZ      64

//--------------------------------------------
#pragma pack(push, 1)
typedef struct cdc_config
{
	struct usb_config_descriptor        config;
	struct usb_interface_descriptor     comm;
	struct usb_cdc_header_desc          cdc_hdr;
	struct usb_cdc_call_mgmt_desc       cdc_mgmt;
	struct usb_cdc_acm_desc             cdc_acm;
	struct usb_cdc_union_desc           cdc_union;
	struct usb_endpoint_descriptor      comm_ep;
	struct usb_interface_descriptor     data;
	struct usb_endpoint_descriptor      data_eprx;
	struct usb_endpoint_descriptor      data_eptx;
} cdc_config_t;
#pragma pack(pop)

//--------------------------------------------
static const struct usb_device_descriptor device_desc =
{
	.bLength            = sizeof(struct usb_device_descriptor),
	.bDescriptorType    = USB_DTYPE_DEVICE,
	.bcdUSB             = CPU_TO_LE16(VERSION_BCD(2,0,0)),
	.bDeviceClass       = USB_CLASS_CDC,
	.bDeviceSubClass    = USB_SUBCLASS_NONE,
	.bDeviceProtocol    = USB_PROTO_NONE,
	.bMaxPacketSize0    = CDC_EP0_SIZE,
	.idVendor           = CPU_TO_LE16(0x0483),
	.idProduct          = CPU_TO_LE16(0x5740),
	.bcdDevice          = CPU_TO_LE16(VERSION_BCD(1,0,0)),
	.iManufacturer      = 1,
	.iProduct           = 2,
	.iSerialNumber      = INTSERIALNO_DESCRIPTOR,
	.bNumConfigurations = 1,
};

//--------------------------------------------
static const struct cdc_config config_desc =
{
	.config =
	{
		.bLength                = sizeof(struct usb_config_descriptor),
		.bDescriptorType        = USB_DTYPE_CONFIGURATION,
		.wTotalLength           = CPU_TO_LE16(sizeof(struct cdc_config)),
		.bNumInterfaces         = 2,
		.bConfigurationValue    = 1,
		.iConfiguration         = NO_DESCRIPTOR,
		.bmAttributes           = USB_CFG_ATTR_RESERVED | USB_CFG_ATTR_SELFPOWERED,
		.bMaxPower              = USB_CFG_POWER_MA(100),
	},
	.comm =
	{
		.bLength                = sizeof(struct usb_interface_descriptor),
		.bDescriptorType        = USB_DTYPE_INTERFACE,
		.bInterfaceNumber       = 0,
		.bAlternateSetting      = 0,
		.bNumEndpoints          = 1,
		.bInterfaceClass        = USB_CLASS_CDC,
		.bInterfaceSubClass     = USB_CDC_SUBCLASS_ACM,
		.bInterfaceProtocol     = USB_CDC_PROTO_V25TER,
		.iInterface             = NO_DESCRIPTOR,
	},
	.cdc_hdr =
	{
		.bFunctionLength        = sizeof(struct usb_cdc_header_desc),
		.bDescriptorType        = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType     = USB_DTYPE_CDC_HEADER,
		.bcdCDC                 = CPU_TO_LE16(VERSION_BCD(1,1,0)),
	},
	.cdc_mgmt =
	{
		.bFunctionLength        = sizeof(struct usb_cdc_call_mgmt_desc),
		.bDescriptorType        = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType     = USB_DTYPE_CDC_CALL_MANAGEMENT,
		.bmCapabilities         = 0,
		.bDataInterface         = 1,
	},
	.cdc_acm =
	{
		.bFunctionLength        = sizeof(struct usb_cdc_acm_desc),
		.bDescriptorType        = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType     = USB_DTYPE_CDC_ACM,
		.bmCapabilities         = 0,
	},
	.cdc_union =
	{
		.bFunctionLength        = sizeof(struct usb_cdc_union_desc),
		.bDescriptorType        = USB_DTYPE_CS_INTERFACE,
		.bDescriptorSubType     = USB_DTYPE_CDC_UNION,
		.bMasterInterface0      = 0,
		.bSlaveInterface0       = 1,
	},
	.comm_ep =
	{
		.bLength                = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType        = USB_DTYPE_ENDPOINT,
		.bEndpointAddress       = CDC_NTF_EP,
		.bmAttributes           = USB_EPTYPE_INTERRUPT,
		.wMaxPacketSize         = CPU_TO_LE16(CDC_NTF_SZ),
		.bInterval              = 0xFF,
	},
	.data =
	{
		.bLength                = sizeof(struct usb_interface_descriptor),
		.bDescriptorType        = USB_DTYPE_INTERFACE,
		.bInterfaceNumber       = 1,
		.bAlternateSetting      = 0,
		.bNumEndpoints          = 2,
		.bInterfaceClass        = USB_CLASS_CDC_DATA,
		.bInterfaceSubClass     = USB_SUBCLASS_NONE,
		.bInterfaceProtocol     = USB_PROTO_NONE,
		.iInterface             = NO_DESCRIPTOR,
	},
	.data_eprx =
	{
		.bLength                = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType        = USB_DTYPE_ENDPOINT,
		.bEndpointAddress       = CDC_RXD_EP,
		.bmAttributes           = USB_EPTYPE_BULK,
		.wMaxPacketSize         = CPU_TO_LE16(CDC_DATA_SZ),
		.bInterval              = 0x01,
	},
	.data_eptx =
	{
		.bLength                = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType        = USB_DTYPE_ENDPOINT,
		.bEndpointAddress       = CDC_TXD_EP,
		.bmAttributes           = USB_EPTYPE_BULK,
		.wMaxPacketSize         = CPU_TO_LE16(CDC_DATA_SZ),
		.bInterval              = 0x01,
	},
};

//--------------------------------------------
static const struct usb_string_descriptor lang_desc = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC("Homemcu firmware examples");
static const struct usb_string_descriptor prod_desc_en = USB_STRING_DESC("CDC loopback demo");
static const struct usb_string_descriptor *const dtable[] =
{
	&lang_desc,
	&manuf_desc_en,
	&prod_desc_en,
};

//--------------------------------------------
static struct usb_cdc_line_coding cdc_line =
{
	.dwDTERate          = CPU_TO_LE32(115200),
	.bCharFormat        = USB_CDC_1_STOP_BITS,
	.bParityType        = USB_CDC_NO_PARITY,
	.bDataBits          = 8,
};

//--------------------------------------------
static usbd_device udev;
static uint32_t fpos;
// Due to use with USB FIFO and/or DMA, the data buffers below must be 32-bit aligned:
#define USB_CTRL_BUFF_SZ 32
static uint32_t ubuf[(USB_CTRL_BUFF_SZ + 3) / sizeof(uint32_t)];
static uint32_t buff[((CDC_DATA_SZ * 2) + 3) / sizeof(uint32_t)];

//--------------------------------------------
static usbd_respond cdc_getdesc(usbd_ctlreq *req, void **address, uint16_t *length)
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
static usbd_respond cdc_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback)
{
	if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) != (USB_REQ_INTERFACE | USB_REQ_CLASS))
	{
		return usbd_fail;
	}
	switch (req->bRequest)
	{
	case USB_CDC_SET_CONTROL_LINE_STATE:
		return usbd_ack;
	case USB_CDC_SET_LINE_CODING:
		memmove(req->data, &cdc_line, sizeof(cdc_line));
		return usbd_ack;
	case USB_CDC_GET_LINE_CODING:
		dev->status.data_ptr = &cdc_line;
		dev->status.data_count = sizeof(cdc_line);
		return usbd_ack;
	default:
		return usbd_fail;
	}
}

//--------------------------------------------
static void cdc_loopback(usbd_device *dev, uint8_t event, uint8_t ep)
{
	int32_t len;

	switch (event)
	{
	case usbd_evt_eprx:
		if (fpos < (sizeof(buff) - CDC_DATA_SZ))
		{
			len = usbd_ep_read(dev, CDC_RXD_EP, (uint8_t *)buff + fpos, CDC_DATA_SZ);
			if (len > 0)
			{
				fpos += len;
			}
		}
	case usbd_evt_eptx:
		if (fpos == 0)
		{
			break;
		}
		len = usbd_ep_write(dev, CDC_TXD_EP, buff, (fpos < CDC_DATA_SZ) ? fpos : CDC_DATA_SZ);
		if (len > 0)
		{
			memmove(buff, (uint8_t *)buff + len, fpos - len);
			fpos -= len;
		}
		break;
	default:
		break;
	}
}

//--------------------------------------------
static usbd_respond cdc_setconf(usbd_device *dev, uint8_t cfg)
{
	switch (cfg)
	{
	case 0:
		// deconfiguring device
		usbd_ep_deconfig(dev, CDC_RXD_EP);
		usbd_ep_deconfig(dev, CDC_TXD_EP);
		usbd_ep_deconfig(dev, CDC_NTF_EP);
		usbd_reg_endpoint(dev, CDC_RXD_EP, NULL);
		usbd_reg_endpoint(dev, CDC_TXD_EP, NULL);
		return usbd_ack;
	case 1:
        // configuring device
		usbd_ep_config(dev, CDC_RXD_EP, USB_EPTYPE_BULK, CDC_DATA_SZ);
		usbd_ep_config(dev, CDC_TXD_EP, USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF, CDC_DATA_SZ);
        usbd_ep_config(dev, CDC_NTF_EP, USB_EPTYPE_INTERRUPT, CDC_NTF_SZ);
		usbd_reg_endpoint(dev, CDC_RXD_EP, cdc_loopback);
		usbd_reg_endpoint(dev, CDC_TXD_EP, cdc_loopback);
		return usbd_ack;
	default:
		return usbd_fail;
	}
}

//--------------------------------------------
void usb_cdc_loopback_init(void)
{
	usbd_hw_init(&udev);
	usbd_init(&udev, &usbd_hw, CDC_EP0_SIZE, ubuf, sizeof(ubuf));
	usbd_reg_control(&udev, cdc_control);
	usbd_reg_config(&udev, cdc_setconf);
	usbd_reg_descr(&udev, cdc_getdesc);
}

//--------------------------------------------
void usb_cdc_loopback_loop(void)
{
	usbd_enable(&udev, true);
	usbd_connect(&udev, true);
	while (1);
}
