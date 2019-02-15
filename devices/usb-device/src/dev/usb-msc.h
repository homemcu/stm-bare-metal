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

#ifndef USB_MSC_H_
#define USB_MSC_H_

// USB MSC class
// Generic USB MSC class definitions
// This module based on
// USB Mass Storage Class - Bulk Only Transport Revision 1.0 September 31, 1999
// Information technology - SCSI Primary Commands - 4 (SPC-4) Revision 37 17 May 2014
// Information technology - SCSI Block Commands – 3 (SBC-3) Revision 25 27 October 2010
// Information technology - Multi-Media Commands - 6 (MMC-6) Revision 2g 11 December 2009
// MSC-BOT-1 refers to the USB Mass Storage Class - Bulk Only Transport, Revision 1.0
// MSC-SO-1 refers to the USB Mass Storage Class - Specification Overview, Revision 1.4 February 19, 2010
// SPC-4 refers to the SCSI Primary Commands - 4, Revision 37
// SBC-3 refers to the SCSI Block Commands – 3, Revision 25
// MMC-6 refers to the Multi-Media Commands - 6, Revision 2g

// SCSI transparent command set (MSC-SO-1 Table 1)
#define USB_MSC_SUBCLASS_SCSI           0x06
// Bulk-Only Transport (MSC-SO-1 Table 2)
#define USB_MSC_PROTO_BULKONLY          0x50

// Get Max LUN (MSC-BOT-1 Table 3.2)
#define USB_MSC_BOT_GET_MAX_LUN         0xFE
// Bulk-Only Mass Storage Reset (MSC-BOT-1 Table 3.1)
#define USB_MSC_BOT_RESET               0xFF

// Command Block Wrapper (MSC-BOT-1 Table 5.1)
#define USB_MSC_BOT_CBW_SIGNATURE       0x43425355
// Command Status Wrapper (MSC-BOT-1 Table 5.2)
#define USB_MSC_BOT_CSW_SIGNATURE       0x53425355
#define USB_MSC_BOT_CSW_CMD_PASSED      0x00
#define USB_MSC_BOT_CSW_CMD_FAILED      0x01
#define USB_MSC_BOT_CSW_PHASE_ERROR     0x02

// INQUIRY command (SPC-4 Table 175)
#define SCSI_INQUIRY                                0x12
// REQUEST SENSE command (SPC-4 Table 314)
#define SCSI_REQUEST_SENSE                          0x03
// MODE SENSE(6) command (SPC-4 Table 199)
#define SCSI_MODE_SENSE_6                           0x1A
// MODE SENSE(10) command (SPC-4 Table 202)
#define SCSI_MODE_SENSE_10                          0x5A
// TEST UNIT READY command (SPC-4 Table 335)
#define SCSI_TEST_UNIT_READY                        0x00
// READ CAPACITY(10) command (SBC-3 Table 62)
#define SCSI_READ_CAPACITY_10                       0x25
// READ(10) command (SBC-3 Table 56)
#define SCSI_READ_10                                0x28
// WRITE(10) command (SBC-3 Table 97)
#define SCSI_WRITE_10                               0x2A
// VERIFY(10) command (SBC-3 Table 88)
#define SCSI_VERIFY_10                              0x2F
// PREVENT ALLOW MEDIUM REMOVAL command (SBC-3 Table 52)
#define SCSI_PREVENT_ALLOW_MEDIUM_REMOVAL           0x1E
// START STOP UNIT command (SBC-3 Table 80)
#define SCSI_START_STOP_UNIT                        0x1B
// READ FORMAT CAPACITIES command (MMC-6 Table 460)
#define SCSI_READ_FORMAT_CAPACITIES                 0x23

// Sense key descriptions (SPC-4 Table 54)
#define ILLEGAL_REQUEST                             5

// ASC and ASCQ assignments (SPC-4 Table 55)
#define INVALID_COMMAND_OPERATION_CODE              0x20
#define LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE          0x21
#define INVALID_FIELD_IN_CDB                        0x24

#pragma pack(push, 1)

// Command Block Wrapper (MSC-BOT-1 Table 5.1)
typedef struct usb_msc_bot_cbw
{
	uint32_t dSignature;
	uint32_t dTag;
	uint32_t dDataTransferLength;
	uint8_t bmFlags;
	uint8_t bLUN;
	uint8_t bCBLength;
	uint8_t CB[16];
} usb_msc_bot_cbw_t;

// Command Status Wrapper (MSC-BOT-1 Table 5.2)
typedef struct usb_msc_bot_csw
{
	uint32_t dSignature;
	uint32_t dTag;
	uint32_t dDataResidue;
	uint8_t bStatus;
} usb_msc_bot_csw_t;

// INQUIRY command (SPC-4 Table 175)
typedef struct cdb_inquiry
{
	uint8_t oper_code;
	uint8_t evpd;
	uint8_t page_code;
	uint8_t allocation_length[2];
	uint8_t control;
} cdb_inquiry_t;

// Standard INQUIRY data format (SPC-4 Table 176)
typedef struct db_inquiry
{
	uint8_t peripheral;
	uint8_t rmb_lu_conf;
	uint8_t version;
	uint8_t response_data_format;
	uint8_t additional_length;
	uint8_t bit_fields_1;
	uint8_t bit_fields_2;
	uint8_t bit_fields_3;
	uint8_t vendor_identification[8];
	uint8_t product_identification[16];
	uint8_t product_revision_level[4];
} db_inquiry_t;

// READ FORMAT CAPACITIES command (MMC-6 Table 460)
typedef struct cdb_read_format_capacities
{
	uint8_t oper_code;
	uint8_t reserved[6];
	uint8_t allocation_length[2];
	uint8_t control;
} cdb_read_format_capacities_t;

// READ FORMAT CAPACITIES data format (MMC-6 Table 461)
typedef struct db_read_format_capacities
{
	uint8_t reserved[3];
	uint8_t capacity_list_length;
	uint8_t number_of_blocks[4];
	uint8_t descriptor_type;
	uint8_t block_length[3];
} db_read_format_capacities_t;

// READ CAPACITY(10) command (SBC-3 Table 62)
typedef struct cdb_read_capacity_10
{
	uint8_t oper_code;
	uint8_t reserved;
	uint8_t logical_block_address[4];
	uint8_t reserved2[2];
	uint8_t pmi;
	uint8_t control;
} cdb_read_capacity_10_t;

// READ CAPACITY(10) parameter data (SBC-3 Table 63)
typedef struct db_read_capacity_10
{
	uint8_t logical_block_address[4];
	uint8_t logical_block_length[4];
} db_read_capacity_10_t;

// READ(10) command (SBC-3 Table 56)
typedef struct cdb_read_10
{
	uint8_t oper_code;
	uint8_t bit_fields;
	uint8_t logical_block_address[4];
	uint8_t group_number;
	uint8_t transfer_length[2];
	uint8_t control;
} cdb_read_10_t;

// Mode parameter header(6) (SPC-4 Table 454)
typedef struct db_mode_sence_6
{
	uint8_t mode_data_length;
	uint8_t medium_type;
	uint8_t device_specific_parameter;
	uint8_t block_descriptor_length;
} db_mode_sence_6_t;

// Mode parameter header(10) (SPC-4 Table 455)
typedef struct db_mode_sence_10
{
	uint8_t mode_data_length[2];
	uint8_t medium_type;
	uint8_t device_specific_parameter;
	uint8_t bit_field_longlba;
	uint8_t reserved;
	uint8_t block_descriptor_length[2];
} db_mode_sence_10_t;

// WRITE(10) command (SBC-3 Table 97)
typedef struct cdb_write_10
{
	uint8_t operation_code;
	uint8_t bit_fields;
	uint8_t logical_block_address[4];
	uint8_t group_number;
	uint8_t transfer_length[2];
	uint8_t control;
} cdb_write_10_t;

// VERIFY(10) command (SBC-3 Table 88)
typedef struct cdb_verify_10
{
	uint8_t operation_code;
	uint8_t bit_fields;
	uint8_t logical_block_address[4];
	uint8_t group_number;
	uint8_t verification_length[2];
	uint8_t control;
} cdb_verify_10_t;

// REQUEST SENSE command (SPC-4 Table 314)
typedef struct cdb_request_sence
{
	uint8_t operation_code;
	uint8_t desc;
	uint8_t reserved[2];
	uint8_t allocation_length;
	uint8_t control;
} cdb_request_sence_t;

// Fixed format sense data (SPC-4 Table 53)
typedef struct db_request_sence
{
	uint8_t response_code;
	uint8_t obsolete;
	uint8_t sence_key;
	uint8_t information[4];
	uint8_t additional_sence_length;
	uint8_t command_specific_information[4];
	uint8_t additional_sence_code;
	uint8_t additional_sence_code_qualifier;
	uint8_t field_replaceable_unit_code;
	uint8_t sense_key_specific_information[3];
} db_request_sence_t;

#pragma pack(pop)

#endif /* USB_MSC_H_ */
