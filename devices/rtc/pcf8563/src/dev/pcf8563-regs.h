#ifndef PCF8563_REGS_H_
#define PCF8563_REGS_H_

#define PCF8563_CONTROL_STATUS_1      0x00
#define PCF8563_CONTROL_STATUS_2      0x01
#define PCF8563_VL_SECONDS            0x02
#define PCF8563_MINUTES               0x03
#define PCF8563_HOURS                 0x04
#define PCF8563_DAYS                  0x05
#define PCF8563_WEEKDAYS              0x06
#define PCF8563_CENTURY_MONTHS        0x07
#define PCF8563_YEARS                 0x08
#define PCF8563_MINUTE_ALARM          0x09
#define PCF8563_HOUR_ALARM            0x0A
#define PCF8563_DAY_ALARM             0x0B
#define PCF8563_WEEKDAY_ALARM         0x0C
#define PCF8563_CLKOUT_CONTROL        0x0D
#define PCF8563_TIMER_CONTROL         0x0E
#define PCF8563_TIMER                 0x0F

#define PCF8563_VL_FLAG               0x80
#define PCF8563_SECONDS_MASK          0x7F
#define PCF8563_MINUTES_MASK          0x7F
#define PCF8563_HOURS_MASK            0x3F
#define PCF8563_DAYS_MASK             0x3F
#define PCF8563_WEEKDAYS_MASK         0x07
#define PCF8563_CENTURY_FLAG          0x80
#define PCF8563_MONTHS_MASK           0x1F

#endif // PCF8563_REGS_H_
