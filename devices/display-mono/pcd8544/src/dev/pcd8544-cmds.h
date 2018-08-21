#ifndef PCD8544_CMDS_H_
#define PCD8544_CMDS_H_

#define PCD8544_SCR_W                   (uint8_t)84
#define PCD8544_SCR_H                   (uint8_t)48

#define PCD8544_EXTENDEDINSTRUCTION     0x01
#define PCD8544_DISPLAYNORMAL           0x04
#define PCD8544_DISPLAYCONTROL          0x08
#define PCD8544_SETBIAS                 0x10
#define PCD8544_FUNCTIONSET             0x20
#define PCD8544_SETYADDR                0x40
#define PCD8544_SETXADDR                0x80
#define PCD8544_SETVOP                  0x80

#endif // PCD8544_CMDS_H_
