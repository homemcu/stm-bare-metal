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

// USB MSC class
// This module based on
// Universal Serial Bus Specification Revision 2.0 April 27, 2000
// USB Mass Storage Class - Bulk Only Transport Revision 1.0 September 31, 1999
// Information technology - SCSI Primary Commands - 4 (SPC-4) Revision 37 17 May 2014
// Information technology - SCSI Block Commands – 3 (SBC-3) Revision 25 27 October 2010
// Information technology - Multi-Media Commands - 6 (MMC-6) Revision 2g 11 December 2009

#include "platform.h"
#include "usbd_core.h"
#include "usb_std.h"
#include "hal-usbd-init.h"
#include "usb-msc.h"
#include "sd.h"
#include "sd-card-drv.h"
#include <string.h>

//--------------------------------------------
extern const sd_card_drv_t sd_card_drv;

#ifdef USBD_OTGFS
extern const struct usbd_driver usbd_otgfs;
#define usbd_hw usbd_otgfs
#elif defined USBD_OTGHS || defined USBD_OTGHS_FS
extern const struct usbd_driver usbd_otghs;
#define usbd_hw usbd_otghs
#endif

//--------------------------------------------
#define MSC_EP0_SIZE    64
#define MSC_RXD_EP      0x01
#define MSC_TXD_EP      0x82
#ifdef USBD_FULL_SPEED
#define MSC_DATA_SZ     64
#else
#define MSC_DATA_SZ     512
#endif
#define SECTOR_SIZE     512

//--------------------------------------------
#pragma pack(push, 1)
typedef struct msc_config
{
	struct usb_config_descriptor        config;
	struct usb_interface_descriptor     comm;
	struct usb_endpoint_descriptor      data_eprx;
	struct usb_endpoint_descriptor      data_eptx;
} msc_config_t;
#pragma pack(pop)

//--------------------------------------------
static const struct usb_device_descriptor device_desc =
{
	.bLength            = sizeof(struct usb_device_descriptor),
	.bDescriptorType    = USB_DTYPE_DEVICE,
	.bcdUSB             = CPU_TO_LE16(VERSION_BCD(2,0,0)),
	.bDeviceClass       = USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass    = USB_SUBCLASS_NONE,
	.bDeviceProtocol    = USB_PROTO_NONE,
	.bMaxPacketSize0    = MSC_EP0_SIZE,
	.idVendor           = CPU_TO_LE16(0x0483),
	.idProduct          = CPU_TO_LE16(0x5740),
	.bcdDevice          = CPU_TO_LE16(VERSION_BCD(1,0,0)),
	.iManufacturer      = 1,
	.iProduct           = 2,
	.iSerialNumber      = INTSERIALNO_DESCRIPTOR,
	.bNumConfigurations = 1,
};

//--------------------------------------------
static const msc_config_t config_desc =
{
	.config =
	{
		.bLength                = sizeof(struct usb_config_descriptor),
		.bDescriptorType        = USB_DTYPE_CONFIGURATION,
		.wTotalLength           = CPU_TO_LE16(sizeof(msc_config_t)),
		.bNumInterfaces         = 1,
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
		.bNumEndpoints          = 2,
		.bInterfaceClass        = USB_CLASS_MASS_STORAGE,
		.bInterfaceSubClass     = USB_MSC_SUBCLASS_SCSI,
		.bInterfaceProtocol     = USB_MSC_PROTO_BULKONLY,
		.iInterface             = NO_DESCRIPTOR,
	},
	.data_eprx =
	{
		.bLength                = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType        = USB_DTYPE_ENDPOINT,
		.bEndpointAddress       = MSC_RXD_EP,
		.bmAttributes           = USB_EPTYPE_BULK,
		.wMaxPacketSize         = CPU_TO_LE16(MSC_DATA_SZ),
		.bInterval              = 0,
	},
	.data_eptx =
	{
		.bLength                = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType        = USB_DTYPE_ENDPOINT,
		.bEndpointAddress       = MSC_TXD_EP,
		.bmAttributes           = USB_EPTYPE_BULK,
		.wMaxPacketSize         = CPU_TO_LE16(MSC_DATA_SZ),
		.bInterval              = 0,
	},
};

//--------------------------------------------
static const struct usb_string_descriptor lang_desc = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC("Homemcu firmware examples");
static const struct usb_string_descriptor prod_desc_en = USB_STRING_DESC("MSC sd-card demo");
static const struct usb_string_descriptor *const dtable[] =
{
	&lang_desc,
	&manuf_desc_en,
	&prod_desc_en,
};

//--------------------------------------------
static const db_inquiry_t db_inquiry =
{
	0x00, // PERIPHERAL QUALIFIER = 0, PERIPHERAL DEVICE TYPE = 0 (Direct access block device SBC-3)
	0x80, // RMB = 1 (Removable media), LU_CONG = 0 (Logical unit is not part of a logical unit conglomerate)
	0x04, // The device server complies to SPC-2
	0x02, // The response data format complies to SPC-2/3/4
	0x1F, // ADDITIONAL LENGTH = sizeof(scsi_std_inquiry) - 5
	0x00, // SCCS = 0, ACC = 0, TPGS = 0, PROTECT = 0
	0x00, // ENCSERV = 0, VS(Vendor Specific) = 0, MULTIP = 0
	0x00, // CMDQUE = 0, VS(Vendor Specific) = 0
	"STM     ", // T10 VENDOR IDENTIFICATION
	"Mass Storage    ", // PRODUCT IDENTIFICATION
	"0001" // PRODUCT REVISION LEVEL
};

// Information technology - SCSI Block Commands – 3 (SBC-3) Revision 25 27 October 2010
// 6.4.1 Mode parameters overview
static const db_mode_sence_6_t db_mode_sence_6 =
{
	0x03, // The number of bytes that follow
	0x00, // The MEDIUM TYPE field in the mode parameter header (see SPC-4) shall be set to 00h
	0x00, // WP = 0 (not write-protected), DPOFUA = 0
	0x00  // No block descriptors in the mode parameter list
};

// Information technology - SCSI Block Commands – 3 (SBC-3) Revision 25 27 October 2010
// 6.4.1 Mode parameters overview
static const db_mode_sence_10_t db_mode_sence_10 =
{
	{ 0x00,	0x06, }, // The number of bytes that follow
	0x00, // The MEDIUM TYPE field in the mode parameter header (see SPC-4) shall be set to 00h
	0x00, // WP = 0 (not write-protected), DPOFUA = 0
	0x00, // LONGLBA = 0
	0x00, // Reserved
	{ 0x00,	0x00 }, // No block descriptors in the mode parameter list
};

//--------------------------------------------
typedef enum bot_state
{
	bot_idle = 0,
	bot_data,
	bot_data_read,
	bot_data_write,
	bot_csw
} bote_state_t;

typedef struct bot
{
	uint8_t max_lun;
	bote_state_t state;
	usb_msc_bot_cbw_t cbw;
	usb_msc_bot_csw_t csw;
	uint32_t buff[(SECTOR_SIZE + 3) / sizeof(uint32_t)];
	uint32_t fpos;
	uint32_t lba;
	uint32_t lbn;
	uint8_t sence_key;
	uint8_t asc;
} bot_t;

//--------------------------------------------
static usbd_device udev;
static bot_t bot;
// Due to use with USB FIFO and/or DMA, the data buffers below must be 32-bit aligned:
#define USB_CTRL_BUFF_SZ 32
static uint32_t ubuf[(USB_CTRL_BUFF_SZ + 3) / sizeof(uint32_t)];

//--------------------------------------------
static usbd_respond msc_getdesc(usbd_ctlreq *req, void **address, uint16_t *length)
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
static usbd_respond msc_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback)
{
	if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) != (USB_REQ_INTERFACE | USB_REQ_CLASS))
	{
		return usbd_fail;
	}
	switch (req->bRequest)
	{
	case USB_MSC_BOT_GET_MAX_LUN:
		dev->status.data_ptr = &bot.max_lun;
		dev->status.data_count = 1;
		return usbd_ack;
	case USB_MSC_BOT_RESET:
		memset(&bot, 0, sizeof(bot));
		return usbd_ack;
	default:
		return usbd_fail;
	}
}

//--------------------------------------------
static void bot_tx(usbd_device *dev)
{
	int32_t len;

	switch (bot.state)
	{
	case bot_data:
		len = usbd_ep_write(dev, MSC_TXD_EP, bot.buff, (bot.fpos < MSC_DATA_SZ) ? bot.fpos : MSC_DATA_SZ);
		if (len > 0)
		{
			memmove(bot.buff, (uint8_t *)bot.buff + len, bot.fpos - len);
			bot.fpos -= len;
			if (bot.fpos == 0)
			{
				bot.state = bot_csw;
			}
		}
		break;
	case bot_data_read:
		if (bot.fpos == 0)
		{
			sd_error err;
			do
			{
				err = sd_card_drv.read((uint8_t *)bot.buff, bot.lba, 1);
			}
			while (err == sd_err_wrong_status);
			bot.lba++;
			bot.lbn--;
			bot.fpos = sizeof(bot.buff);
		}
		len = usbd_ep_write(dev, MSC_TXD_EP, bot.buff, (bot.fpos < MSC_DATA_SZ) ? bot.fpos : MSC_DATA_SZ);
		if (len > 0)
		{
			memmove(bot.buff, (uint8_t *)bot.buff + len, bot.fpos - len);
			bot.fpos -= len;
		}
		if (!bot.lbn && !bot.fpos)
		{
			bot.csw.dDataResidue = 0;
			bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
			bot.state = bot_csw;
		}
		break;
	case bot_csw:
		len = usbd_ep_write(dev, MSC_TXD_EP, &bot.csw, sizeof(bot.csw));
		bot.state = bot_idle;
		break;
	default:
		break;
	}
}

//--------------------------------------------
static void bot_error(usbd_device *dev, uint8_t sence_key, uint8_t asc)
{
	bot.sence_key = sence_key;
	bot.asc = asc;

	bot.state = bot_csw;
	bot.csw.dDataResidue = 0;
	bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_FAILED;
	bot_tx(dev);
}

//--------------------------------------------
static void bot_rx(usbd_device *dev)
{
	int32_t len;
	uint32_t alloc_len;

	switch (bot.state)
	{
	case bot_idle:
		len = usbd_ep_read(dev, MSC_RXD_EP, bot.buff, MSC_DATA_SZ);
		if (len != sizeof(struct usb_msc_bot_cbw))
		{
			bot.sence_key = ILLEGAL_REQUEST;
			bot.asc = INVALID_COMMAND_OPERATION_CODE;
			usbd_ep_stall(dev, MSC_RXD_EP);
			return;
		}
		memcpy(&bot.cbw, bot.buff, sizeof(struct usb_msc_bot_cbw));
		if ((bot.cbw.dSignature != USB_MSC_BOT_CBW_SIGNATURE) ||
			(bot.cbw.bCBLength < 1) ||
			(bot.cbw.bCBLength > 16))
		{
			bot.sence_key = ILLEGAL_REQUEST;
			bot.asc = INVALID_COMMAND_OPERATION_CODE;
			usbd_ep_stall(dev, MSC_RXD_EP);
			return;
		}
		bot.csw.dSignature = 0x53425355;
		bot.csw.dTag = bot.cbw.dTag;
		switch (bot.cbw.CB[0])
		{
		case SCSI_INQUIRY:
			{
				cdb_inquiry_t *cdb;

				cdb = (cdb_inquiry_t *)bot.cbw.CB;
				alloc_len = (uint32_t)(cdb->allocation_length[0] << 8 | cdb->allocation_length[1]);
				if (!(cdb->evpd & 0x01))
				{
					if (!cdb->page_code)
					{
						bot.fpos = (alloc_len < sizeof(db_inquiry)) ? alloc_len : sizeof(db_inquiry);
						memcpy(bot.buff, &db_inquiry, bot.fpos);
						bot.state = bot_data;
						bot.csw.dDataResidue = bot.cbw.dDataTransferLength - bot.fpos;
						bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
						bot_tx(dev);
					}
					else
					{
						bot_error(dev, ILLEGAL_REQUEST, INVALID_FIELD_IN_CDB);
					}
				}
				else
				{
					bot_error(dev, ILLEGAL_REQUEST, INVALID_FIELD_IN_CDB);
				}

				break;
			}
		case SCSI_REQUEST_SENSE:
			{
				cdb_request_sence_t *cdb;
				db_request_sence_t db = { 0 };

				cdb = (cdb_request_sence_t *)bot.cbw.CB;
				alloc_len = (uint32_t)(cdb->allocation_length);

				db.response_code = 0x70;
				db.sence_key = bot.sence_key;
				db.additional_sence_length = 10;
				db.additional_sence_code = bot.asc;

				bot.fpos = (alloc_len < sizeof(db)) ? alloc_len : sizeof(db);
				memcpy(bot.buff, &db, bot.fpos);
				bot.state = bot_data;
				bot.csw.dDataResidue = bot.cbw.dDataTransferLength - bot.fpos;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);

				break;
			}
		case SCSI_MODE_SENSE_6:
			{
				bot.fpos = sizeof(db_mode_sence_6);
				memcpy(bot.buff, &db_mode_sence_6, bot.fpos);
				bot.state = bot_data;
				bot.csw.dDataResidue = bot.cbw.dDataTransferLength - bot.fpos;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);

				break;
			}
		case SCSI_MODE_SENSE_10:
			{
				bot.fpos = sizeof(db_mode_sence_10);
				memcpy(bot.buff, &db_mode_sence_10, bot.fpos);
				bot.state = bot_data;
				bot.csw.dDataResidue = bot.cbw.dDataTransferLength - bot.fpos;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);

				break;
			}
		case SCSI_TEST_UNIT_READY:
			{
				bot.state = bot_csw;
				bot.csw.dDataResidue = 0;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);

				break;
			}
		case SCSI_READ_CAPACITY_10:
			{
				cdb_read_capacity_10_t *cdb;
				db_read_capacity_10_t db = { 0 };
				sd_info_t *sd_info;

				cdb = (cdb_read_capacity_10_t *)bot.cbw.CB;
				sd_info = sd_card_drv.getcardinfo();
				bot.lba = cdb->logical_block_address[0] << 24 |
				          cdb->logical_block_address[1] << 16 |
				          cdb->logical_block_address[2] << 8 |
				          cdb->logical_block_address[3];
				if (bot.lba > sd_info->card_size - 1)
				{
					bot_error(dev, ILLEGAL_REQUEST, LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE);
					break;
				}
				if ((cdb->pmi & 0x01) && !bot.lba)
				{
					bot_error(dev, ILLEGAL_REQUEST, INVALID_FIELD_IN_CDB);
					break;
				}

				db.logical_block_address[0] = (sd_info->card_size - 1) >> 24;
				db.logical_block_address[1] = (sd_info->card_size - 1) >> 16;
				db.logical_block_address[2] = (sd_info->card_size - 1) >> 8;
				db.logical_block_address[3] = (sd_info->card_size - 1);
				db.logical_block_length[0] = sd_info->sect_size >> 24;
				db.logical_block_length[1] = sd_info->sect_size >> 16;
				db.logical_block_length[2] = sd_info->sect_size >> 8;
				db.logical_block_length[3] = sd_info->sect_size;

				bot.fpos = sizeof(db);
				memcpy(bot.buff, &db, bot.fpos);
				bot.state = bot_data;
				bot.csw.dDataResidue = bot.cbw.dDataTransferLength - bot.fpos;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);

				break;
			}
		case SCSI_READ_10:
			{
				cdb_read_10_t *cdb;
				sd_info_t *sd_info;

				cdb = (cdb_read_10_t *)bot.cbw.CB;
				sd_info = sd_card_drv.getcardinfo();
				bot.lba = cdb->logical_block_address[0] << 24 |
				          cdb->logical_block_address[1] << 16 |
				          cdb->logical_block_address[2] << 8 |
				          cdb->logical_block_address[3];
				bot.lbn = cdb->transfer_length[0] << 8 |
				          cdb->transfer_length[1];
				if (bot.lba > sd_info->card_size - 1)
				{
					bot_error(dev, ILLEGAL_REQUEST, LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE);
					break;
				}
				if (cdb->bit_fields & 0xE0)
				{
					bot_error(dev, ILLEGAL_REQUEST, INVALID_FIELD_IN_CDB);
					break;
				}
				if (bot.cbw.dDataTransferLength != bot.lbn * sd_info->sect_size)
				{
					bot_error(dev, ILLEGAL_REQUEST, INVALID_COMMAND_OPERATION_CODE);
					break;
				}

				bot.state = bot_data_read;
				bot_tx(dev);

				break;
			}
		case SCSI_WRITE_10:
			{
				cdb_write_10_t *cdb;
				sd_info_t *sd_info;

				cdb = (cdb_write_10_t *)bot.cbw.CB;
				sd_info = sd_card_drv.getcardinfo();
				bot.lba = cdb->logical_block_address[0] << 24 |
				          cdb->logical_block_address[1] << 16 |
				          cdb->logical_block_address[2] << 8 |
				          cdb->logical_block_address[3];
				bot.lbn = cdb->transfer_length[0] << 8 |
				          cdb->transfer_length[1];
				if (bot.lba > sd_info->card_size - 1)
				{
					bot_error(dev, ILLEGAL_REQUEST, LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE);
					break;
				}
				if (cdb->bit_fields & 0xE0)
				{
					bot_error(dev, ILLEGAL_REQUEST, INVALID_FIELD_IN_CDB);
					break;
				}
				if (bot.cbw.dDataTransferLength != bot.lbn * sd_info->sect_size)
				{
					bot_error(dev, ILLEGAL_REQUEST, INVALID_COMMAND_OPERATION_CODE);
					break;
				}

				bot.fpos = 0;
				bot.state = bot_data_write;

				break;
			}
		case SCSI_VERIFY_10:
			{
				cdb_verify_10_t *cdb;
				sd_info_t *sd_info;

				cdb = (cdb_verify_10_t *)bot.cbw.CB;
				sd_info = sd_card_drv.getcardinfo();
				bot.lba = cdb->logical_block_address[0] << 24 |
				          cdb->logical_block_address[1] << 16 |
				          cdb->logical_block_address[2] << 8 |
				          cdb->logical_block_address[3];
				if (bot.lba > sd_info->card_size - 1)
				{
					bot_error(dev, ILLEGAL_REQUEST, LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE);
					break;
				}
				if (cdb->bit_fields & 0x02)
				{
					bot_error(dev, ILLEGAL_REQUEST, INVALID_FIELD_IN_CDB);
					break;
				}

				bot.state = bot_csw;
				bot.csw.dDataResidue = 0;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);

				break;
			}
		case SCSI_READ_FORMAT_CAPACITIES:
			{
				cdb_read_format_capacities_t *cdb;
				db_read_format_capacities_t db = { 0 };
				sd_info_t *sd_info;

				cdb = (cdb_read_format_capacities_t *)bot.cbw.CB;
				alloc_len = (uint32_t)(cdb->allocation_length[0] << 8 | cdb->allocation_length[1]);
				sd_info = sd_card_drv.getcardinfo();

				db.capacity_list_length = 8;
				db.number_of_blocks[0] = sd_info->card_size >> 24;
				db.number_of_blocks[1] = sd_info->card_size >> 16;
				db.number_of_blocks[2] = sd_info->card_size >> 8;
				db.number_of_blocks[3] = sd_info->card_size;
				db.descriptor_type = 0x02;
				db.block_length[0] = sd_info->sect_size >> 16;
				db.block_length[1] = sd_info->sect_size >> 8;
				db.block_length[2] = sd_info->sect_size;

				bot.fpos = (alloc_len < sizeof(db)) ? alloc_len : sizeof(db);
				memcpy(bot.buff, &db, bot.fpos);
				bot.state = bot_data;
				bot.csw.dDataResidue = bot.cbw.dDataTransferLength - bot.fpos;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);

				break;
			}
		case SCSI_PREVENT_ALLOW_MEDIUM_REMOVAL:
			{
				bot.state = bot_csw;
				bot.csw.dDataResidue = 0;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);

				break;
			}
		case SCSI_START_STOP_UNIT:
			{
				bot.state = bot_csw;
				bot.csw.dDataResidue = 0;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);

				break;
			}
		default:
			{
				bot_error(dev, ILLEGAL_REQUEST, INVALID_COMMAND_OPERATION_CODE);

				break;
			}
		}
		break;
	case bot_data_write:
		len = usbd_ep_read(dev, MSC_RXD_EP, (uint8_t *)bot.buff + bot.fpos, MSC_DATA_SZ);
		bot.fpos += len;
		if (bot.fpos == sizeof(bot.buff))
		{
			sd_error err;
			do
			{
				err = sd_card_drv.write((uint8_t *)bot.buff, bot.lba, 1);
			}
			while (err == sd_err_wrong_status);
			bot.fpos = 0;
			bot.lba++;
			bot.lbn--;
			if (!bot.lbn)
			{
				bot.state = bot_csw;
				bot.csw.dDataResidue = 0;
				bot.csw.bStatus = USB_MSC_BOT_CSW_CMD_PASSED;
				bot_tx(dev);
			}
		}
		break;
	default:
		break;
	}
}

//--------------------------------------------
static void bot_callback(usbd_device *dev, uint8_t event, uint8_t ep)
{
	switch (event)
	{
    case usbd_evt_eptx:
		bot_tx(dev);
		break;
    case usbd_evt_eprx:
		bot_rx(dev);
		break;
	default:
		break;
	}
}

//--------------------------------------------
static usbd_respond msc_setconf(usbd_device *dev, uint8_t cfg)
{
	switch (cfg)
	{
	case 0:
		// deconfiguring device
		usbd_ep_deconfig(dev, MSC_RXD_EP);
		usbd_ep_deconfig(dev, MSC_TXD_EP);
		usbd_reg_endpoint(dev, MSC_RXD_EP, NULL);
		usbd_reg_endpoint(dev, MSC_TXD_EP, NULL);
		return usbd_ack;
	case 1:
        // configuring device
		usbd_ep_config(dev, MSC_RXD_EP, USB_EPTYPE_BULK, MSC_DATA_SZ);
		usbd_ep_config(dev, MSC_TXD_EP, USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF, MSC_DATA_SZ);
		usbd_reg_endpoint(dev, MSC_RXD_EP, bot_callback);
		usbd_reg_endpoint(dev, MSC_TXD_EP, bot_callback);
		return usbd_ack;
	default:
		return usbd_fail;
	}
}

//--------------------------------------------
void usb_msc_bot_scsi_init(void)
{
	sd_card_drv.init();
	sd_card_drv.reset();

	usbd_hw_init(&udev);
	usbd_init(&udev, &usbd_hw, MSC_EP0_SIZE, ubuf, sizeof(ubuf));
	usbd_reg_control(&udev, msc_control);
	usbd_reg_config(&udev, msc_setconf);
	usbd_reg_descr(&udev, msc_getdesc);
}

//--------------------------------------------
void usb_msc_bot_scsi_loop(void)
{
	usbd_enable(&udev, true);
	usbd_connect(&udev, true);
	while (1);
}
