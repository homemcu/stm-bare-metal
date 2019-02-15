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

#ifndef RNDIS_H_
#define RNDIS_H_

// USB MS-RNDIS class
// Generic USB MS-RNDIS class definitions
// This module based on
// [MS-RNDIS]: Remote Network Driver Interface Specification (RNDIS) Protocol Revision 5.0, May 1, 2014
// [MSDN-OIDs] Microsoft Corporation, "Remote NDIS OIDs", http://msdn.microsoft.com/en-us/library/ff570633.aspx
// CDC-1.2 refers to the USB Class Definitions for Communications Devices Revision 1.2 (Errata 1), November 3, 2010

// https://www.usb.org/defined-class-codes#anchor_BaseClassE0h
#define USB_WIRELESS_SUBCLASS_RF             1
#define USB_WIRELESS_PROTO_RNDIS             3

// RNDIS Message Types (MS-RNDIS 2.2.1.1)
#define RNDIS_MSG_COMPLETION                 0x80000000
#define RNDIS_PACKET_MSG                     0x00000001
#define RNDIS_INITIALIZE_MSG                 0x00000002
#define RNDIS_INITIALIZE_CMPLT               0x80000002
#define RNDIS_HALT_MSG                       0x00000003
#define RNDIS_QUERY_MSG                      0x00000004
#define RNDIS_QUERY_CMPLT                    0x80000004
#define RNDIS_SET_MSG                        0x00000005
#define RNDIS_SET_CMPLT                      0x80000005
#define RNDIS_RESET_MSG                      0x00000006
#define RNDIS_RESET_CMPLT                    0x80000006
#define RNDIS_INDICATE_STATUS_MSG            0x00000007
#define RNDIS_KEEPALIVE_MSG                  0x00000008
#define RNDIS_KEEPALIVE_CMPLT                0x80000008

// Common Status Values (MS-RNDIS 2.2.1.2)
#define RNDIS_STATUS_SUCCESS                 0x00000000
//#define RNDIS_STATUS_PENDING                 0x00000103
#define	RNDIS_STATUS_NOT_SUPPORTED           0xC00000BB

// https://docs.microsoft.com/en-us/windows-hardware/drivers/network/remote-ndis-initialize-cmplt
// MajorVersion and MinorVersion for REMOTE_NDIS_INITIALIZE_CMPLT (MS-RNDIS 2.2.9)
#define RNDIS_MAJOR_VERSION                  0x00000001
#define RNDIS_MINOR_VERSION                  0x00000000
// DeviceFlags for REMOTE_NDIS_INITIALIZE_CMPLT (MS-RNDIS 2.2.9)
#define RNDIS_DF_CONNECTIONLESS              0x00000001
#define RNDIS_DF_CONNECTION_ORIENTED         0x00000002
#define RNDIS_DF_RAW_DATA                    0x00000004
// Medium for REMOTE_NDIS_INITIALIZE_CMPLT (MS-RNDIS 2.2.9)
#define RNDIS_MEDIUM_802_3                   0x00000000

// General (required) OIDs for Remote NDIS Ethernet devices (MSDN-OIDs)
#define OID_GEN_SUPPORTED_LIST               0x00010101
#define OID_GEN_HARDWARE_STATUS              0x00010102
#define OID_GEN_MEDIA_SUPPORTED              0x00010103
#define OID_GEN_MEDIA_IN_USE                 0x00010104
//#define OID_GEN_MAXIMUM_LOOKAHEAD            0x00010105
#define OID_GEN_MAXIMUM_FRAME_SIZE           0x00010106
#define OID_GEN_LINK_SPEED                   0x00010107
//#define OID_GEN_TRANSMIT_BUFFER_SPACE        0x00010108
//#define OID_GEN_RECEIVE_BUFFER_SPACE         0x00010109
#define OID_GEN_TRANSMIT_BLOCK_SIZE          0x0001010A
#define OID_GEN_RECEIVE_BLOCK_SIZE           0x0001010B
#define OID_GEN_VENDOR_ID                    0x0001010C
#define OID_GEN_VENDOR_DESCRIPTION           0x0001010D
#define OID_GEN_CURRENT_PACKET_FILTER        0x0001010E
//#define OID_GEN_CURRENT_LOOKAHEAD            0x0001010F
//#define OID_GEN_DRIVER_VERSION               0x00010110
#define OID_GEN_MAXIMUM_TOTAL_SIZE           0x00010111
//#define OID_GEN_PROTOCOL_OPTIONS             0x00010112
//#define OID_GEN_MAC_OPTIONS                  0x00010113
#define OID_GEN_MEDIA_CONNECT_STATUS         0x00010114
//#define OID_GEN_MAXIMUM_SEND_PACKETS         0x00010115
#define OID_GEN_VENDOR_DRIVER_VERSION        0x00010116
#define OID_GEN_PHYSICAL_MEDIUM              0x00010202

// General statistic (required) OIDs for Remote NDIS Ethernet devices (MSDN-OIDs)
#define OID_GEN_XMIT_OK                      0x00020101
#define OID_GEN_RCV_OK                       0x00020102
#define OID_GEN_XMIT_ERROR                   0x00020103
#define OID_GEN_RCV_ERROR                    0x00020104
#define OID_GEN_RCV_NO_BUFFER                0x00020105

// Specific 802.3 (required) OIDs for Remote NDIS Ethernet devices (MSDN-OIDs)
#define OID_802_3_PERMANENT_ADDRESS          0x01010101
#define OID_802_3_CURRENT_ADDRESS            0x01010102
#define OID_802_3_MULTICAST_LIST             0x01010103
#define OID_802_3_MAXIMUM_LIST_SIZE          0x01010104

// Class-Specific Notification Codes (CDC-1.2 Table 20)
#define RNDIS_RESPONSE_AVAILABLE             0x00000001

// https://docs.microsoft.com/en-us/windows-hardware/drivers/network/oid-gen-media-connect-status
#define RNDIS_MEDIA_STATE_CONNECTED          0x00000000
#define RNDIS_MEDIA_STATE_DISCONNECTED       0x00000001

// https://docs.microsoft.com/en-us/windows-hardware/drivers/network/control-channel-characteristics
typedef struct rndis_notification_msg
{
	uint32_t Notification;
	uint32_t Reserved;
} rndis_notification_msg_t;

// REMOTE_NDIS_INITIALIZE_MSG (MS-RNDIS 2.2.2)
typedef struct rndis_initialize_msg
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t RequestID;
	uint32_t MajorVersion;
	uint32_t MinorVersion;
	uint32_t MaxTransferSize;
} rndis_initialize_msg_t;

// REMOTE_NDIS_INITIALIZE_CMPLT (MS-RNDIS 2.2.9)
typedef struct rndis_initialize_cmplt
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t RequestID;
	uint32_t Status;
	uint32_t MajorVersion;
	uint32_t MinorVersion;
	uint32_t DeviceFlags;
	uint32_t Medium;
	uint32_t MaxPacketsPerTransfer;
	uint32_t MaxTransferSize;
	uint32_t PacketAlignmentFactor;
	uint32_t AFListOffset;
	uint32_t AFListSize;
} rndis_initialize_cmplt_t;

// REMOTE_NDIS_QUERY_MSG (MS-RNDIS 2.2.4)
typedef struct rndis_query_msg
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t RequestID;
	uint32_t OID;
	uint32_t InformationBufferLength;
	uint32_t InformationBufferOffset;
	uint32_t DeviceVcHandle;
} rndis_query_msg_t;

// REMOTE_NDIS_QUERY_CMPLT (MS-RNDIS 2.2.10)
typedef struct rndis_query_cmplt
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t RequestID;
	uint32_t Status;
	uint32_t InformationBufferLength;
	uint32_t InformationBufferOffset;
} rndis_query_cmplt_t;

// REMOTE_NDIS_SET_MSG (MS-RNDIS 2.2.5)
typedef struct rndis_set_msg
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t RequestID;
	uint32_t OID;
	uint32_t InformationBufferLength;
	uint32_t InformationBufferOffset;
	uint32_t DeviceVcHandle;
} rndis_set_msg_t;

// REMOTE_NDIS_SET_CMPLT (MS-RNDIS 2.2.11)
typedef struct rndis_set_cmplt
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t RequestID;
	uint32_t Status;
} rndis_set_cmplt_t;

// REMOTE_NDIS_RESET_MSG (MS-RNDIS 2.2.6)
typedef struct rndis_reset_msg
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t Reserved;
} rndis_reset_msg_t;

// REMOTE_NDIS_RESET_CMPLT (MS-RNDIS 2.2.12)
typedef struct rndis_reset_cmplt
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t Status;
	uint32_t AddressingReset;
} rndis_reset_cmplt_t;

// REMOTE_NDIS_INDICATE_STATUS_MSG (MS-RNDIS 2.2.7)
typedef struct rndis_indicate_status_msg
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t Status;
	uint32_t StatusBufferLength;
	uint32_t StatusBufferOffset;
} rndis_indicate_status_msg_t;

// REMOTE_NDIS_KEEPALIVE_MSG (MS-RNDIS 2.2.8)
typedef struct rndis_keepalive_msg
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t RequestID;
} rndis_keepalive_msg_t;

// REMOTE_NDIS_KEEPALIVE_CMPLT (MS-RNDIS 2.2.13)
typedef struct rndis_keepalive_cmplt
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t RequestID;
	uint32_t Status;
} rndis_keepalive_cmplt_t;

// REMOTE_NDIS_PACKET_MSG (MS-RNDIS 2.2.14)
typedef struct rndis_packet_msg
{
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t DataOffset;
	uint32_t DataLength;
	uint32_t OOBDataOffset;
	uint32_t OOBDataLength;
	uint32_t NumOOBDataElements;
	uint32_t PerPacketInfoOffset;
	uint32_t PerPacketInfoLength;
	uint32_t VcHandle;
	uint32_t Reserved;
} rndis_packet_msg_t;

#endif // RNDIS_H_