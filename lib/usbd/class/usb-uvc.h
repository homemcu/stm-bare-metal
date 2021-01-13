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

#ifndef USB_UVC_H_
#define USB_UVC_H_

// USB UVC class
// Generic USB video class definitions
// This module is based on
// Universal Serial Bus Device Class Definition for Video Devices Revision 1.1 June 1, 2005
// VDC-1 refers to the USB Device Class Definition for Video Devices, Revision 1.1
// UP-1 refers to the USB Device Class Definition for Video Devices: Uncompressed Payload, Revision 1.1

// Video Interface Subclass Codes (VDC-1 Table A-2)
#define USB_SUBCLASS_VIDEOCONTROL                 0x01
#define USB_SUBCLASS_VIDEOSTREAMING               0x02
#define USB_SUBCLASS_VIDEO_INTERFACE_COLLECTION   0x03

// Video Class-Specific VC Interface Descriptor Subtypes (VDC-1 Table A-5)
#define USB_DTYPE_UVC_VC_HEADER                   0x01
#define USB_DTYPE_UVC_VC_INPUT_TERMINAL           0x02
#define USB_DTYPE_UVC_VC_OUTPUT_TERMINAL          0x03
#define USB_DTYPE_UVC_VC_SELECTOR_UNIT            0x04
#define USB_DTYPE_UVC_VC_PROCESSING_UNIT          0x05
#define USB_DTYPE_UVC_VC_EXTENSION_UNIT           0x06

// USB Terminal Types (VDC-1 Table B-1)
#define USB_UVC_TT_VENDOR_SPECIFIC                0x0100
#define USB_UVC_TT_STREAMING                      0x0101
// Input Terminal Types (VDC-1 Table B-2)
#define USB_UVC_ITT_VENDOR_SPECIFIC               0x0200
#define USB_UVC_ITT_CAMERA                        0x0201
#define USB_UVC_ITT_MEDIA_TRANSPORT_INPUT         0x0202
// Output Terminal Types (VDC-1 Table B-3)
#define USB_UVC_OTT_VENDOR_SPECIFIC               0x0300
#define USB_UVC_OTT_DISPLAY                       0x0301
#define USB_UVC_OTT_MEDIA_TRANSPORT_OUTPUT        0x0302

// Video Class-Specific VS Interface Descriptor Subtypes (VDC-1 Table A-6)
#define USB_DTYPE_UVC_VS_INPUT_HEADER             0x01
#define USB_DTYPE_UVC_VS_OUTPUT_HEADER            0x02
#define USB_DTYPE_UVC_VS_STILL_IMAGE_FRAME        0x03
#define USB_DTYPE_UVC_VS_FORMAT_UNCOMPRESSED      0x04
#define USB_DTYPE_UVC_VS_FRAME_UNCOMPRESSED       0x05
#define USB_DTYPE_UVC_VS_FORMAT_MJPEG             0x06
#define USB_DTYPE_UVC_VS_FRAME_MJPEG              0x07
#define USB_DTYPE_UVC_VS_FORMAT_MPEG2TS           0x0A
#define USB_DTYPE_UVC_VS_FORMAT_DV                0x0C
#define USB_DTYPE_UVC_VS_COLORFORMAT              0x0D
#define USB_DTYPE_UVC_VS_FORMAT_FRAME_BASED       0x10
#define USB_DTYPE_UVC_VS_FRAME_FRAME_BASED        0x11
#define USB_DTYPE_UVC_VS_FORMAT_STREAM_BASED      0x12

// Video Class-Specific Request Codes (VDC-1 Table A-8)
#define USB_UVC_SET_CUR                           0x01
#define USB_UVC_GET_CUR                           0x81
#define USB_UVC_GET_MIN                           0x82
#define USB_UVC_GET_MAX                           0x83
#define USB_UVC_GET_RES                           0x84
#define USB_UVC_GET_LEN                           0x85
#define USB_UVC_GET_INFO                          0x86
#define USB_UVC_GET_DEF                           0x87

// VideoStreaming Interface Control Selectors (VDC-1 Table A-15)
#define USB_UVC_VS_PROBE_CONTROL                  0x01
#define USB_UVC_VS_COMMIT_CONTROL                 0x02

// Stream Header Format for Uncompressed Streams (UP-1 Table 2-2)
#define USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_FID   0x01
#define USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_EOF   0x02
#define USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_PTS   0x04
#define USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_SCR   0x08
#define USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_RES   0x10
#define USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_STI   0x20
#define USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_ERR   0x40
#define USB_UVC_VS_DATA_UNCOMPRESSED_HEADER_EOH   0x80

#pragma pack(push, 1)

// Class-specific VC Interface Header Descriptor (VDC-1 Table 3-3)
struct usb_uvc_vc_header_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint16_t bcdUVC;
	uint16_t wTotalLength;
	uint32_t dwClockFrequency;
	uint8_t bInCollection;
	uint8_t baInterfaceNr1;
};

// Camera Terminal Descriptor (VDC-1 Table 3-6)
struct usb_uvc_input_terminal_camera_sz1_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t iTerminal;
	uint16_t wObjectiveFocalLengthMin;
	uint16_t wObjectiveFocalLengthMax;
	uint16_t wOcularFocalLength;
	uint8_t bControlSize;
	uint8_t bmControls[1];
};
struct usb_uvc_input_terminal_camera_sz2_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t iTerminal;
	uint16_t wObjectiveFocalLengthMin;
	uint16_t wObjectiveFocalLengthMax;
	uint16_t wOcularFocalLength;
	uint8_t bControlSize;
	uint8_t bmControls[2];
};
struct usb_uvc_input_terminal_camera_sz3_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t iTerminal;
	uint16_t wObjectiveFocalLengthMin;
	uint16_t wObjectiveFocalLengthMax;
	uint16_t wOcularFocalLength;
	uint8_t bControlSize;
	uint8_t bmControls[3];
};

// Output Terminal Descriptor (VDC-1 Table 3-5)
struct usb_uvc_output_terminal_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t bSourceID;
	uint8_t iTerminal;
};

// Selector Unit Descriptor (VDC-1 Table 3-7)
struct usb_uvc_selector_unit_sz2_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bNrInPins;
	uint8_t baSourceID1;
	uint8_t baSourceID2;
	uint8_t iSelector;
};

// Processing Unit Descriptor (VDC-1 Table 3-8)
struct usb_uvc_processing_unit_sz1_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint16_t wMaxMultiplier;
	uint8_t bControlSize;
	uint8_t bmControls[1];
	uint8_t iProcessing;
};
struct usb_uvc_processing_unit_sz2_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint16_t wMaxMultiplier;
	uint8_t bControlSize;
	uint8_t bmControls[2];
	uint8_t iProcessing;
};
struct usb_uvc_processing_unit_sz3_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint16_t wMaxMultiplier;
	uint8_t bControlSize;
	uint8_t bmControls[3];
	uint8_t iProcessing;
};

// Class-specific VS Interface Input Header Descriptor (VDC-1 Table 3-13)
struct usb_uvc_vs_input_header_sz1_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bNumFormats;
	uint16_t wTotalLength;
	uint8_t bEndpointAddress;
	uint8_t bmInfo;
	uint8_t bTerminalLink;
	uint8_t bStillCaptureMethod;
	uint8_t bTriggerSupport;
	uint8_t bTriggerUsage;
	uint8_t bControlSize;
	uint8_t bmaControls[1];
};

// Uncompressed Video Format Descriptor (UP-1 Table 3-1)
struct usb_uvc_vs_format_uncompressed_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bFormatIndex;
	uint8_t bNumFrameDescriptors;
	uint8_t guidFormat[16];
	uint8_t bBitsPerPixel;
	uint8_t bDefaultFrameIndex;
	uint8_t bAspectRatioX;
	uint8_t bAspectRatioY;
	uint8_t bmInterlaceFlags;
	uint8_t bCopyProtect;
};

// Uncompressed Video Frame Descriptors (UP-1 Table 3-2, 3-3)
struct usb_uvc_vs_frame_uncompressed_cont_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bFrameIndex;
	uint8_t bmCapabilities;
	uint16_t wWidth;
	uint16_t wHeight;
	uint32_t dwMinBitRate;
	uint32_t dwMaxBitRate;
	uint32_t dwMaxVideoFrameBufferSize;
	uint32_t dwDefaultFrameInterval;
	uint8_t bFrameIntervalType;
	uint32_t dwMinFrameInterval;
	uint32_t dwMaxFrameInterval;
	uint32_t dwFrameIntervalStep;
};

// Color Matching Descriptor (VDC-1 Table 3-18)
struct usb_uvc_vs_color_matching_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bColorPrimaries;
	uint8_t bTransferCharacteristics;
	uint8_t bMatrixCoefficients;
};

// Video Probe and Commit Controls (VDC-1 Table 4-47)
struct usb_uvc_vs_control
{
	uint16_t bmHint;
	uint8_t bFormatIndex;
	uint8_t bFrameIndex;
	uint32_t dwFrameInterval;
	uint16_t wKeyFrameRate;
	uint16_t wPFrameRate;
	uint16_t wCompQuality;
	uint16_t wCompWindowSize;
	uint16_t wDelay;
	uint32_t dwMaxVideoFrameSize;
	uint32_t dwMaxPayloadTransferSize;
};

// Stream Header Format for Uncompressed Streams (UP-1 Table 2-2)
struct usb_uvc_vs_data_uncompressed_header
{
	uint8_t HLE;
	uint8_t BFH;
};

#pragma pack(pop)

#endif /* USB_UVC_H_ */

