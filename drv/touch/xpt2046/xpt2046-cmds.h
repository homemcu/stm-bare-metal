#ifndef XPT2046_CMDS_H_
#define XPT2046_CMDS_H_

#define XPT2046_CB_START           0x80
#define XPT2046_CB_A2              0x40
#define XPT2046_CB_A1              0x20
#define XPT2046_CB_A0              0x10
#define XPT2046_CB_MODE            0x08
#define XPT2046_CB_SER_DFR         0x04
#define XPT2046_CB_PD1             0x02
#define XPT2046_CB_PD0             0x01

#define XPT2046_CMD_XPOS           XPT2046_CB_START | XPT2046_CB_A2 | XPT2046_CB_A0
#define XPT2046_CMD_YPOS           XPT2046_CB_START | XPT2046_CB_A0

#endif // XPT2046_CMDS_H_
