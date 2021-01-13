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

#ifndef USB_UAC_H_
#define USB_UAC_H_

// USB UAC class
// Generic USB audio class definitions
// This module is based on
// Universal Serial Bus Device Class Definition for Audio Devices Release 1.0 March 18, 1998
// USB-2 refers to the Universal Serial Bus Specification, Revision 2.0
// ADC-1 refers to the USB Device Class Definition for Audio Devices, Release 1.0
// FMT-1 refers to the Audio Data Formats specification, Release 1.0
// TT-1 refers to the Terminal Types specification, Release 1.0

// Audio Interface Subclass Codes (ADC-1 Table A-2)
#define USB_SUBCLASS_AUDIOCONTROL                      0x01
#define USB_SUBCLASS_AUDIOSTREAMING                    0x02
#define USB_SUBCLASS_MIDISTREAMING                     0x03

// Audio Class-Specific AC Interface Descriptor Subtypes (ADC-1 Table A-5)
#define USB_DTYPE_UAC_AC_HEADER                        0x01
#define USB_DTYPE_UAC_AC_INPUT_TERMINAL                0x02
#define USB_DTYPE_UAC_AC_OUTPUT_TERMINAL               0x03
#define USB_DTYPE_UAC_AC_MIXER_UNIT                    0x04
#define USB_DTYPE_UAC_AC_SELECTOR_UNIT                 0x05
#define USB_DTYPE_UAC_AC_FEATURE_UNIT                  0x06
#define USB_DTYPE_UAC_AC_EFFECT_UNIT                   0x07
#define USB_DTYPE_UAC_AC_PROCESSING_UNIT               0x08

// USB Terminal Types (TT-1 Table 2-1)
#define USB_UAC_TT_USB_STREAMING                       0x0101
#define USB_UAC_TT_VENDOR_SPECIFIC                     0x01FF
// Input Terminal Types (TT-1 Table 2-2)
#define USB_UAC_ITT_MICROPHONE                         0x0201
#define USB_UAC_ITT_DESKTOP_MICROPHONE                 0x0202
#define USB_UAC_ITT_PERSONAL_MICROPHONE                0x0203
#define USB_UAC_ITT_OMNIDIRECTIONAL_MICROPHONE         0x0204
#define USB_UAC_ITT_MICROPHONE_ARRAY                   0x0205
#define USB_UAC_ITT_PROCESSING_MICROPHONE_ARRAY        0x0206
// Output Terminal Types (TT-1 Table 2-3)
#define USB_UAC_OTT_SPEAKER                            0x0301
#define USB_UAC_OTT_HEADPHONES                         0x0302
#define USB_UAC_OTT_HEAD_MOUNTED_DISPLAY_AUDIO         0x0303
#define USB_UAC_OTT_DESKTOP_SPEAKER                    0x0304
#define USB_UAC_OTT_ROOM_SPEAKER                       0x0305
#define USB_UAC_OTT_COMMUNICATION_SPEAKER              0x0306
#define USB_UAC_OTT_LOW_FREQUENCY_EFFECTS_SPEAKER      0x0307

// Feature Unit Control Selectors (ADC-1 Table A-11)
#define USB_UAC_FU_MUTE_CONTROL                        0x01
#define USB_UAC_FU_VOLUME_CONTROL                      0x02
#define USB_UAC_FU_BASS_CONTROL                        0x03
#define USB_UAC_FU_MID_CONTROL                         0x04
#define USB_UAC_FU_TREBLE_CONTROL                      0x05
#define USB_UAC_FU_GRAPHIC_EQUALIZER_CONTROL           0x06
#define USB_UAC_FU_AUTOMATIC_GAIN_CONTROL              0x07
#define USB_UAC_FU_DELAY_CONTROL                       0x08
#define USB_UAC_FU_BASS_BOOST_CONTROL                  0x09
#define USB_UAC_FU_LOUDNESS_CONTROL                    0x0A
// Feature Unit Descriptor bmaControls bits (ADC-1 Table 4-7)
#define USB_UAC_FU_BMAC_MUTE_CONTROL                   0x0001
#define USB_UAC_FU_BMAC_VOLUME_CONTROL                 0x0002
#define USB_UAC_FU_BMAC_BASS_CONTROL                   0x0004
#define USB_UAC_FU_BMAC_MID_CONTROL                    0x0008
#define USB_UAC_FU_BMAC_TREBLE_CONTROL                 0x0010
#define USB_UAC_FU_BMAC_GRAPHIC_EQUALIZER_CONTROL      0x0020
#define USB_UAC_FU_BMAC_AUTOMATIC_GAIN_CONTROL         0x0040
#define USB_UAC_FU_BMAC_DELAY_CONTROL                  0x0080
#define USB_UAC_FU_BMAC_BASS_BOOST_CONTROL             0x0100
#define USB_UAC_FU_BMAC_LOUDNESS_CONTROL               0x0200

// Audio Class-Specific AS Interface Descriptor Subtypes (ADC-1 Table A-6)
#define USB_DTYPE_UAC_AS_GENERAL                       0x01
#define USB_DTYPE_UAC_AS_FORMAT_TYPE                   0x02
#define USB_DTYPE_UAC_AS_FORMAT_SPECIFIC               0x03

// Format Type Codes (FMT-1 Table A-4)
#define USB_UAC_DATA_FORMAT_TYPE_I                     0x01
#define USB_UAC_DATA_FORMAT_TYPE_II                    0x02
#define USB_UAC_DATA_FORMAT_TYPE_III                   0x03

// Audio Data Format Type I Codes (FMT-1 Table A-1)
#define USB_UAC_DATA_FORMAT_TYPE_I_UNDEFINED           0x0000
#define USB_UAC_DATA_FORMAT_PCM                        0x0001
#define USB_UAC_DATA_FORMAT_PCM8                       0x0002
#define USB_UAC_DATA_FORMAT_IEEE_FLOAT                 0x0003
#define USB_UAC_DATA_FORMAT_ALAW                       0x0004
#define USB_UAC_DATA_FORMAT_MULAW                      0x0005
// Audio Data Format Type II Codes (FMT-1 Table A-2)
#define USB_UAC_DATA_FORMAT_TYPE_II_UNDEFINED          0x1000
#define USB_UAC_DATA_FORMAT_MPEG                       0x1001
#define USB_UAC_DATA_FORMAT_AC3                        0x1002
// Audio Data Format Type III Codes (FMT-1 Table A-3)
#define USB_UAC_DATA_FORMAT_TYPE_III_UNDEFINED         0x2000
#define USB_UAC_DATA_FORMAT_IEC1937_AC3                0x2001
#define USB_UAC_DATA_FORMAT_IEC1937_MPEG1_LAYER1       0x2002
#define USB_UAC_DATA_FORMAT_IEC1937_MPEG2_NOEXT        0x2003
#define USB_UAC_DATA_FORMAT_IEC1937_MPEG2_EXT          0x2004
#define USB_UAC_DATA_FORMAT_IEC1937_MPEG2_LAYER1       0x2005

// Audio Class-Specific Endpoint Descriptor Subtypes (ADC-1 Table A-8)
#define USB_DTYPE_UAC_EP_GENERAL                       0x01

// Audio Class-Specific Request Codes (ADC-1 Table A-9)
#define USB_UAC_SET_CUR                                0x01
#define USB_UAC_GET_CUR                                0x81

#pragma pack(push, 1)

// Class-Specific AC Interface Header Descriptor (ADC-1 Table 4-2)
struct usb_uac_ac_header_sz1_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint16_t bcdADC;
	uint16_t wTotalLength;
	uint8_t bInCollection;
	uint8_t baInterfaceNr[1];
};

// Input Terminal Descriptor (ADC-1 Table 4-3)
struct usb_uac_input_terminal_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t bNrChannels;
	uint16_t wChannelConfig;
	uint8_t iChannelNames;
	uint8_t iTerminal;
};

// Feature Unit Descriptor (ADC-1 Table 4-7)
struct usb_uac_feature_unit_sz2x16_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint8_t bControlSize;
	uint16_t bmaControls[2];
	uint8_t iFeature;
};
struct usb_uac_feature_unit_sz3x16_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint8_t bControlSize;
	uint16_t bmaControls[3];
	uint8_t iFeature;
};

// Output Terminal Descriptor (ADC-1 Table 4-4)
struct usb_uac_output_terminal_desc
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

// Class-Specific AS Interface Descriptor (ADC-1 Table 4-19)
struct usb_uac_as_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalLink;
	uint8_t bDelay;
	uint16_t wFormatTag;
};

// Type I Format Type Descriptor (FMT-1 Table 2-1)
struct usb_uac_as_format_type1_sz3_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bFormatType;
	uint8_t bNrChannels;
	uint8_t bSubframeSize;
	uint8_t bBitResolution;
	uint8_t bSamFreqType;
	uint8_t tSamFreq[3];
};

// Standard AS Isochronous Audio Data Endpoint Descriptor (ADC-1 Table 4-20)
struct usb_uac_std_iso_endpoint_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
	uint8_t bRefresh;
	uint8_t bSynchAddress;
};

// Class-Specific AS Isochronous Audio Data Endpoint Descriptor (ADC-1 Table 4-21)
struct usb_uac_cs_iso_endpoint_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmAttributes;
	uint8_t bLockDelayUnits;
	uint16_t wLockDelay;
};

// First Form of the Mute Control Parameter Block (ADC-2 Table 5-16)
struct usb_uac_form1_mute_control_parameter_block
{
	uint8_t bMute;
};

// Second Form of the Mute Control Parameter Block (ADC-2 Table 5-17)
struct usb_uac_form2_mute_control_parameter_sz2_block
{
	uint8_t bMute[2];
};
struct usb_uac_form2_mute_control_parameter_sz3_block
{
	uint8_t bMute[3];
};

#pragma pack(pop)

#endif /* USB_UAC_H_ */

