Examples of common device drivers for different MCUs (STM8, STM32) with separate bare metal hardware abstraction layers.

Projects structure:

| Firmware<br>examples | STM8S207R8<br>(SDCC, IAR) | STM32F407ZG<br>(GCC, IAR) |
| ------ | :------: | :------: |
| devices/ | | |
| ../display-color/ | | |
| ../../ili9341+xpt2046 | SPI | SPI, FSMC |
| ../../st7735 | SPI | SPI |
| ../display-mono/ | | |
| ../../pcd8544 | SPI | SPI |
| ../../ssd1306 | SPI, I2C | SPI, I2C |
| ../ethernet-uip/ | | |
| ../../enc28j60 | SPI | SPI |
| ../../lan8720 | | RMII |
| ../rtc/ | | |
| ../../internal | | internal |
| ../../pcf8563 | I2C | |
| ../sd-card/ | | |
| ../../sd-card | SPI | SPI, SDIO |
| ../spi-flash/ | | |
| ../../w25q | SPI(1) | SPI |
| apps/ | | |
| ../file-system/ | | |
| ../../fatfs+sd-card | SPI(2) | SPI, SDIO |
| ../../spiffs+spi-flash | SPI | SPI |
| ../network/ | | |
| ../../ethernet-lwip | | RMII |

1 - Compiling with SDCC < #10503 fails.
Please see [bug ticket #2794](https://sourceforge.net/p/sdcc/bugs/2794/)

2 - Compiling with SDCC fails when FF_USE_MKFS is defined.
Please see [bug ticket #2793](https://sourceforge.net/p/sdcc/bugs/2793/)

The source code is licensed under [GPLv2](http://www.gnu.org/licenses/gpl-2.0.html).

Some examples use 3rd-party sources with their own licensing model.
