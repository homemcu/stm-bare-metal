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
#include "hal-usbd-init.h"
#include "usb_cdc.h"
#include "rndis.h"
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
#define CDC_NTF_SZ      8


#define RNDIS_MTU                       1500
#define ETH_HEADER_SIZE                 14
#define ETH_MAX_PACKET_SIZE             (ETH_HEADER_SIZE + RNDIS_MTU)
#define RNDIS_MAX_PACKET_SIZE           (ETH_MAX_PACKET_SIZE + sizeof(rndis_packet_msg_t))
#define RNDIS_MAX_CTRL_SIZE             92
#define RNDIS_VENDOR                    "STM32-RNDIS"
// Define UIP_CONF_BUFFER_SIZE in the uip-conf.h file = ETH_MAX_PACKET_SIZE = 1514

// https://github.com/torvalds/linux/blob/master/drivers/usb/gadget/function/f_rndis.c#L87
// peak (theoretical) bulk transfer rate in bits-per-second
#ifdef USBD_FULL_SPEED
#define RNDIS_LINK_SPEED                (19 * 64 * 1 * 1000 * 8)
#else
#define RNDIS_LINK_SPEED                (13 * 512 * 8 * 1000 * 8)
#endif

// Three-byte IEEE-registered vendor code, followed by a single byte
// that the vendor assigns to identify a particular NIC.
// The IEEE code uniquely identifies the vendor and is the same as
// the three bytes appearing at the beginning of the NIC hardware address.
// Vendors without an IEEE-registered code should use the value 0xFFFFFF
//#define RNDIS_VENDOR_ID                 0x00FFFFFF
#define RNDIS_VENDOR_ID                 0x00E18000 // 0080E1 = STMicroelectronics SRL


//--------------------------------------------
#pragma pack(push, 1)
typedef struct cdc_config
{
	struct usb_config_descriptor        config;
	struct usb_iad_descriptor           iad;
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
	.bDeviceClass       = USB_CLASS_MISC,
	.bDeviceSubClass    = USB_SUBCLASS_COMMON,
	.bDeviceProtocol    = USB_PROTO_IAD,
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
	.iad =
	{
		.bLength                = sizeof(struct usb_iad_descriptor),
		.bDescriptorType        = USB_DTYPE_INTERFACEASSOC,
		.bFirstInterface        = 0,
		.bInterfaceCount        = 2,
		.bFunctionClass         = USB_CLASS_WIRELESS,
		.bFunctionSubClass      = USB_WIRELESS_SUBCLASS_RF,
		.bFunctionProtocol      = USB_WIRELESS_PROTO_RNDIS,
		.iFunction              = NO_DESCRIPTOR,
	},
	.comm =
	{
		.bLength                = sizeof(struct usb_interface_descriptor),
		.bDescriptorType        = USB_DTYPE_INTERFACE,
		.bInterfaceNumber       = 0,
		.bAlternateSetting      = 0,
		.bNumEndpoints          = 1,
		.bInterfaceClass        = USB_CLASS_WIRELESS,
		.bInterfaceSubClass     = USB_WIRELESS_SUBCLASS_RF,
		.bInterfaceProtocol     = USB_WIRELESS_PROTO_RNDIS,
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
static const struct usb_string_descriptor prod_desc_en = USB_STRING_DESC("RNDIS demo");
static const struct usb_string_descriptor *const dtable[] =
{
	&lang_desc,
	&manuf_desc_en,
	&prod_desc_en,
};

//--------------------------------------------
static const uint32_t oid_supported_list[] =
{
	OID_GEN_SUPPORTED_LIST,
	OID_GEN_HARDWARE_STATUS,
	OID_GEN_MEDIA_SUPPORTED,
	OID_GEN_MEDIA_IN_USE,
	OID_GEN_MAXIMUM_FRAME_SIZE,
	OID_GEN_LINK_SPEED,
	OID_GEN_TRANSMIT_BLOCK_SIZE,
	OID_GEN_RECEIVE_BLOCK_SIZE,
	OID_GEN_VENDOR_ID,
	OID_GEN_VENDOR_DESCRIPTION,
	OID_GEN_VENDOR_DRIVER_VERSION,
	OID_GEN_CURRENT_PACKET_FILTER,
	OID_GEN_MAXIMUM_TOTAL_SIZE,
	OID_GEN_MEDIA_CONNECT_STATUS,
	OID_GEN_XMIT_OK,
	OID_GEN_RCV_OK,
	OID_GEN_XMIT_ERROR,
	OID_GEN_RCV_ERROR,
	OID_GEN_RCV_NO_BUFFER,
	OID_802_3_PERMANENT_ADDRESS,
	OID_802_3_CURRENT_ADDRESS,
	OID_802_3_MULTICAST_LIST,
	OID_802_3_MAXIMUM_LIST_SIZE,
};

//--------------------------------------------
typedef enum rndis_state
{
	rndis_uninitialized,
	rndis_initialized,
	rndis_data_initialized,
} rndis_state_t;

typedef struct rndis_stat
{
	uint32_t tx_ok;
	uint32_t rx_ok;
	uint32_t tx_err;
	uint32_t rx_err;
	uint32_t tx_drop;
	uint32_t rx_drop;
} rndis_stat_t;

typedef struct rndis
{
	rndis_state_t state;
	uint32_t buff_usb_ctrl[RNDIS_MAX_CTRL_SIZE / sizeof(uint32_t)];
	uint32_t fpos_usb_ctrl;
	uint32_t buff_usb_rx[(RNDIS_MAX_PACKET_SIZE + 3) / sizeof(uint32_t)];
	uint32_t fpos_usb_rx;
	uint32_t buff_usb_tx[(RNDIS_MAX_PACKET_SIZE + 3) / sizeof(uint32_t)];
	uint32_t fpos_usb_tx;
	uint32_t len_usb_rx;
	uint32_t len_usb_tx;
	rndis_stat_t stat;
	uint32_t filter;
} rndis_t;

//--------------------------------------------
static usbd_device udev;
static rndis_t rndis;
static uint8_t *mac_addr;
static const char *rndis_vendor = RNDIS_VENDOR;
// Due to use with USB FIFO and/or DMA, the data buffers must be 32-bit aligned
#define USB_CTRL_BUFF_SZ (RNDIS_MAX_CTRL_SIZE + offsetof(usbd_ctlreq, data))
static uint32_t ubuf[(USB_CTRL_BUFF_SZ + 3) / sizeof(uint32_t)];
static uint32_t buff_uip_rx[(RNDIS_MAX_PACKET_SIZE + 3) / sizeof(uint32_t)];
static uint32_t len_uip_rx;


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
static void send_notification_msg(usbd_device *dev)
{
	rndis_notification_msg_t msg = {CPU_TO_LE32(RNDIS_RESPONSE_AVAILABLE), CPU_TO_LE32(0x00000000)};
	usbd_ep_write(dev, CDC_NTF_EP, &msg, sizeof(rndis_notification_msg_t));
}

//--------------------------------------------
static void rndis_initialize(usbd_device *dev, const rndis_initialize_msg_t *msg)
{
	rndis_initialize_cmplt_t *resp = (rndis_initialize_cmplt_t *)rndis.buff_usb_ctrl;

	resp->MessageType = CPU_TO_LE32(RNDIS_INITIALIZE_CMPLT);
	resp->MessageLength = CPU_TO_LE32(sizeof(rndis_initialize_cmplt_t));
	resp->RequestID = msg->RequestID;
	resp->Status = CPU_TO_LE32(RNDIS_STATUS_SUCCESS);
	resp->MajorVersion = CPU_TO_LE32(RNDIS_MAJOR_VERSION);
	resp->MinorVersion = CPU_TO_LE32(RNDIS_MINOR_VERSION);
	resp->DeviceFlags = CPU_TO_LE32(RNDIS_DF_CONNECTIONLESS);
	resp->Medium = CPU_TO_LE32(RNDIS_MEDIUM_802_3);
	resp->MaxPacketsPerTransfer = CPU_TO_LE32(1);
	resp->MaxTransferSize = CPU_TO_LE32(RNDIS_MAX_PACKET_SIZE);
	resp->PacketAlignmentFactor = CPU_TO_LE32(0);
	resp->AFListOffset = CPU_TO_LE32(0);
	resp->AFListSize = CPU_TO_LE32(0);

	rndis.fpos_usb_ctrl = resp->MessageLength;
	send_notification_msg(dev);
}

//--------------------------------------------
static void rndis_query_oid_buf8(rndis_query_cmplt_t *resp, uint8_t *buf, size_t size)
{
	resp->MessageLength = CPU_TO_LE32(sizeof(rndis_query_cmplt_t) + size);
	resp->InformationBufferLength = CPU_TO_LE32(size);
	memcpy((uint8_t *)&resp[1], buf, size);
}

//--------------------------------------------
static void rndis_query_oid_buf32(rndis_query_cmplt_t *resp, uint32_t *buf, size_t size)
{
	size_t cnt;
	uint32_t *oid_input_buffer = (uint32_t *)&resp[1];

	resp->MessageLength = CPU_TO_LE32(sizeof(rndis_query_cmplt_t) + size * sizeof(uint32_t));
	resp->InformationBufferLength = CPU_TO_LE32(size * sizeof(uint32_t));
	for (cnt = 0; cnt < size; cnt++)
	{
		oid_input_buffer[cnt] = cpu_to_le32(buf[cnt]);
	}
}

//--------------------------------------------
static int rndis_query_oid(const rndis_query_msg_t *msg, rndis_query_cmplt_t *resp)
{
	uint32_t value;
	uint32_t oid = le32_to_cpup(&msg->OID);

	switch (oid)
	{
	case OID_GEN_SUPPORTED_LIST:
		rndis_query_oid_buf32(resp, (uint32_t *)oid_supported_list, sizeof(oid_supported_list) / sizeof(uint32_t));
		return 0;
	case OID_GEN_HARDWARE_STATUS:
		value = CPU_TO_LE32(0);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_MEDIA_SUPPORTED:
	case OID_GEN_PHYSICAL_MEDIUM:
	case OID_GEN_MEDIA_IN_USE:
		value = CPU_TO_LE32(RNDIS_MEDIUM_802_3);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_MAXIMUM_FRAME_SIZE:
		value = CPU_TO_LE32(RNDIS_MTU);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_LINK_SPEED:
		value = CPU_TO_LE32(RNDIS_LINK_SPEED / 100); // the unit of measurement is 100 bps
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_TRANSMIT_BLOCK_SIZE:
		value = CPU_TO_LE32(ETH_MAX_PACKET_SIZE);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_RECEIVE_BLOCK_SIZE:
		value = CPU_TO_LE32(ETH_MAX_PACKET_SIZE);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_VENDOR_ID:
		value = CPU_TO_LE32(RNDIS_VENDOR_ID);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_VENDOR_DESCRIPTION:
		rndis_query_oid_buf8(resp, (uint8_t *)&rndis_vendor, strlen(rndis_vendor));
		return 0;
	case OID_GEN_CURRENT_PACKET_FILTER:
		value = cpu_to_le32(rndis.filter);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_MAXIMUM_TOTAL_SIZE:
		value = CPU_TO_LE32(ETH_MAX_PACKET_SIZE);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_MEDIA_CONNECT_STATUS:
		value = CPU_TO_LE32(RNDIS_MEDIA_STATE_CONNECTED);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_VENDOR_DRIVER_VERSION:
		value = CPU_TO_LE32(0x00000100); // version 1.0
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_XMIT_OK:
		value = cpu_to_le32(rndis.stat.tx_ok);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_RCV_OK:
		value = cpu_to_le32(rndis.stat.rx_ok);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_XMIT_ERROR:
		value = cpu_to_le32(rndis.stat.tx_err);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_RCV_ERROR:
		value = cpu_to_le32(rndis.stat.rx_err);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_GEN_RCV_NO_BUFFER:
		value = cpu_to_le32(rndis.stat.rx_drop);
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_802_3_PERMANENT_ADDRESS:
	case OID_802_3_CURRENT_ADDRESS:
		rndis_query_oid_buf8(resp, (uint8_t *)&mac_addr, 6);
		return 0;
	case OID_802_3_MULTICAST_LIST:
		value = CPU_TO_LE32(0xE0000000); // multicast base address only
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	case OID_802_3_MAXIMUM_LIST_SIZE:
		value = CPU_TO_LE32(1); // multicast base address only
		rndis_query_oid_buf32(resp, &value, 1);
		return 0;
	default:
		break;
	}
	return 1;
}

//--------------------------------------------
static void rndis_query(usbd_device *dev, const rndis_query_msg_t *msg)
{
	rndis_query_cmplt_t *resp = (rndis_query_cmplt_t *)rndis.buff_usb_ctrl;

	resp->MessageType = CPU_TO_LE32(RNDIS_QUERY_CMPLT);
	resp->RequestID = msg->RequestID;

	if (rndis_query_oid(msg, resp))
	{
		// OID is not supported
		resp->Status = CPU_TO_LE32(RNDIS_STATUS_NOT_SUPPORTED);
		resp->MessageLength = CPU_TO_LE32(sizeof(rndis_query_cmplt_t));
		resp->InformationBufferLength = CPU_TO_LE32(0);
		resp->InformationBufferOffset = CPU_TO_LE32(0);
	}
	else
	{
		resp->Status = CPU_TO_LE32(RNDIS_STATUS_SUCCESS);
		resp->InformationBufferOffset = CPU_TO_LE32(16);
	}

	rndis.fpos_usb_ctrl = resp->MessageLength;
	send_notification_msg(dev);
}

//--------------------------------------------
static int rndis_set_oid(const rndis_set_msg_t *msg, rndis_set_cmplt_t *resp)
{
	uint32_t *value;
	uint32_t oid = le32_to_cpup(&msg->OID);

	switch (oid)
	{
	case OID_GEN_CURRENT_PACKET_FILTER:
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/network/oid-gen-current-packet-filter
		value = (uint32_t *)((uint8_t *)msg + offsetof(rndis_set_msg_t, RequestID) + msg->InformationBufferOffset);
		rndis.filter = le32_to_cpup(value);
		if (rndis.filter)
		{
			rndis.state = rndis_data_initialized;
		}
		else
		{
			rndis.state = rndis_initialized;
		}
		return 0;
	case OID_802_3_MULTICAST_LIST:
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/network/oid-802-3-multicast-list
		// ignore
		return 0;
	case OID_802_3_MAXIMUM_LIST_SIZE:
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/network/oid-802-3-maximum-list-size
		// ignore
		return 0;
	default:
		break;
	}
	return 1;
}

//--------------------------------------------
static void rndis_set(usbd_device *dev, const rndis_set_msg_t *msg)
{
	rndis_set_cmplt_t *resp = (rndis_set_cmplt_t *)rndis.buff_usb_ctrl;

	resp->MessageType = CPU_TO_LE32(RNDIS_SET_CMPLT);
	resp->RequestID = msg->RequestID;
	resp->MessageLength = CPU_TO_LE32(sizeof(rndis_set_cmplt_t));

	if (rndis_set_oid(msg, resp))
	{
		// OID is not supported
		resp->Status = CPU_TO_LE32(RNDIS_STATUS_NOT_SUPPORTED);
	}
	else
	{
		resp->Status = CPU_TO_LE32(RNDIS_STATUS_SUCCESS);
	}

	rndis.fpos_usb_ctrl = resp->MessageLength;
	send_notification_msg(dev);
}

//--------------------------------------------
static void rndis_reset(usbd_device *dev, const rndis_reset_msg_t *msg)
{
	rndis_reset_cmplt_t *resp = (rndis_reset_cmplt_t *)rndis.buff_usb_ctrl;

	resp->MessageType = CPU_TO_LE32(RNDIS_RESET_CMPLT);
	resp->MessageLength = CPU_TO_LE32(sizeof(rndis_reset_cmplt_t));
	resp->Status = CPU_TO_LE32(RNDIS_STATUS_SUCCESS);
	resp->AddressingReset = CPU_TO_LE32(1);

	rndis.fpos_usb_ctrl = resp->MessageLength;
	send_notification_msg(dev);
}

//--------------------------------------------
static void rndis_keepalive(usbd_device *dev, const rndis_keepalive_msg_t *msg)
{
	rndis_keepalive_cmplt_t *resp = (rndis_keepalive_cmplt_t *)rndis.buff_usb_ctrl;

	resp->MessageType = CPU_TO_LE32(RNDIS_KEEPALIVE_CMPLT);
	resp->RequestID = msg->RequestID;
	resp->MessageLength = CPU_TO_LE32(sizeof(rndis_keepalive_cmplt_t));
	resp->Status = CPU_TO_LE32(RNDIS_STATUS_SUCCESS);

	rndis.fpos_usb_ctrl = resp->MessageLength;
	send_notification_msg(dev);
}

//--------------------------------------------
static usbd_respond cdc_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback)
{
	if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) != (USB_REQ_INTERFACE | USB_REQ_CLASS))
	{
		return usbd_fail;
	}
	switch (req->bRequest)
	{
	case USB_CDC_SEND_ENCAPSULATED_CMD:
		{
			uint32_t MessageType = le32_to_cpup((uint32_t *)&req->data);
			switch (MessageType)
			{
			case RNDIS_INITIALIZE_MSG:
				rndis_initialize(dev, (rndis_initialize_msg_t *)&req->data);
				rndis.state = rndis_initialized;
				return usbd_ack;
			case RNDIS_HALT_MSG:
				rndis.state = rndis_uninitialized;
				return usbd_ack;
			case RNDIS_QUERY_MSG:
				rndis_query(dev, (rndis_query_msg_t *)&req->data);
				return usbd_ack;
			case RNDIS_SET_MSG:
				rndis_set(dev, (rndis_set_msg_t *)&req->data);
				return usbd_ack;
			case RNDIS_RESET_MSG:
				rndis_reset(dev, (rndis_reset_msg_t *)&req->data);
				return usbd_ack;
			case RNDIS_KEEPALIVE_MSG:
				rndis_keepalive(dev, (rndis_keepalive_msg_t *)&req->data);
				return usbd_ack;
			default:
				return usbd_fail;
			}
		}
	case USB_CDC_GET_ENCAPSULATED_RESP:
		{
			dev->status.data_ptr = rndis.buff_usb_ctrl;
			dev->status.data_count = rndis.fpos_usb_ctrl;
			return usbd_ack;
		}
	default:
		return usbd_fail;
	}
}

//--------------------------------------------
static void usb_cdc_rndis_rx_callback(uint8_t *buf, uint32_t size)
{
	memcpy(buff_uip_rx, buf, size);
	len_uip_rx = size;
}

//--------------------------------------------
uint16_t usb_cdc_rndis_recv_packet(uint8_t *buf, uint16_t buflen)
{
	if (len_uip_rx)
	{
		if (buflen < len_uip_rx)
		{
			return 0;
		}
		__disable_irq();
		memcpy(buf, buff_uip_rx, buflen);
		len_uip_rx = 0;
		__enable_irq();
		return buflen;
	}
	return 0;
}

//--------------------------------------------
static void rndis_tx(usbd_device *dev)
{
	int32_t len;

	if (!rndis.fpos_usb_tx)
	{
		return;
	}
	len = usbd_ep_write(dev, CDC_TXD_EP, rndis.buff_usb_tx, (rndis.fpos_usb_tx < CDC_DATA_SZ) ? rndis.fpos_usb_tx : CDC_DATA_SZ);
	if (len <= 0)
	{
		return;
	}
	if (len > 0)
	{
		memmove(rndis.buff_usb_tx, (uint8_t *)rndis.buff_usb_tx + len, rndis.fpos_usb_tx - len);
		rndis.fpos_usb_tx -= len;
		if (rndis.fpos_usb_tx == 0)
		{
			rndis.len_usb_tx = 0;
		}
	}
}

//--------------------------------------------
void usb_cdc_rndis_send_packet(const uint8_t *buf, uint16_t buflen)
{
	rndis_packet_msg_t *msg;

	if (buflen > sizeof(rndis.buff_usb_tx))
	{
		return;
	}
	if (rndis.len_usb_tx > 0)
	{
		return;
	}
	__disable_irq();
	msg = (rndis_packet_msg_t *)rndis.buff_usb_tx;
	memset(msg, 0, sizeof(rndis_packet_msg_t));
	msg->MessageType = CPU_TO_LE32(RNDIS_PACKET_MSG);
	msg->MessageLength = sizeof(rndis_packet_msg_t) + buflen;
	msg->DataOffset = sizeof(rndis_packet_msg_t) - offsetof(rndis_packet_msg_t, DataOffset);
	msg->DataLength = buflen;

	memcpy((uint8_t *)rndis.buff_usb_tx + sizeof(rndis_packet_msg_t), buf, buflen);
	rndis.len_usb_tx = sizeof(rndis_packet_msg_t) + buflen;
	rndis.fpos_usb_tx = sizeof(rndis_packet_msg_t) + buflen;
	rndis_tx(&udev);
	__enable_irq();
}

//--------------------------------------------
static void cdc_loopback(usbd_device *dev, uint8_t event, uint8_t ep)
{
	int32_t len;
	rndis_packet_msg_t *msg;

	switch (event)
	{
	case usbd_evt_eprx:
		if (rndis.fpos_usb_rx > (sizeof(rndis.buff_usb_rx) - CDC_DATA_SZ))
		{
			rndis.fpos_usb_rx = 0;
			++rndis.stat.rx_err;
		}
		len = usbd_ep_read(dev, CDC_RXD_EP, (uint8_t *)rndis.buff_usb_rx + rndis.fpos_usb_rx, CDC_DATA_SZ);
		if (len <= 0)
		{
			return;
		}
		msg = (rndis_packet_msg_t *)rndis.buff_usb_rx;
		if (!rndis.fpos_usb_rx)
		{
			// new packet
			rndis.len_usb_rx = le32_to_cpup(&msg->MessageLength);
		}
		if (len > 0)
		{
			rndis.fpos_usb_rx += len;
		}
		if (rndis.fpos_usb_rx >= rndis.len_usb_rx)
		{
			uint32_t offset = offsetof(rndis_packet_msg_t, DataOffset) + le32_to_cpup(&msg->DataOffset);
			uint32_t len = le32_to_cpup(&msg->DataLength);
			// packet is received completely
			if (offset + len != rndis.len_usb_rx)
			{
				++rndis.stat.rx_err;
			}
			else
			{
				++rndis.stat.rx_ok;
				usb_cdc_rndis_rx_callback((uint8_t *)rndis.buff_usb_rx + offset, len);
			}
			memmove(rndis.buff_usb_rx, (uint8_t *)rndis.buff_usb_rx + rndis.len_usb_rx, rndis.fpos_usb_rx - rndis.len_usb_rx);
			rndis.fpos_usb_rx -= rndis.len_usb_rx;
		}
		break;
	case usbd_evt_eptx:
		rndis_tx(dev);
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
		usbd_ep_deconfig(dev, CDC_NTF_EP);
		usbd_ep_deconfig(dev, CDC_TXD_EP);
		usbd_ep_deconfig(dev, CDC_RXD_EP);
		usbd_reg_endpoint(dev, CDC_RXD_EP, NULL);
		usbd_reg_endpoint(dev, CDC_TXD_EP, NULL);
		return usbd_ack;
	case 1:
        // configuring device
		usbd_ep_config(dev, CDC_RXD_EP, USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF, CDC_DATA_SZ);
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
void usb_cdc_rndis_init(const uint8_t *mac)
{
	usbd_hw_init(&udev);
	usbd_init(&udev, &usbd_hw, CDC_EP0_SIZE, ubuf, sizeof(ubuf));
	usbd_reg_control(&udev, cdc_control);
	usbd_reg_config(&udev, cdc_setconf);
	usbd_reg_descr(&udev, cdc_getdesc);

	mac_addr = (uint8_t *)mac;

	usbd_enable(&udev, true);
	usbd_connect(&udev, true);
}
