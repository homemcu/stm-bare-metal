Examples of common device drivers for the different MCUs (STM8, STM32) with separate bare metal hardware abstraction layers.

Projects structure:

| Firmware<br>examples | STM8S207R8<br>(SDCC, IAR) | STM32F407ZG<br>(GCC, IAR) | STM32F746IG<br>(GCC, IAR) |
| ------ | :------: | :------: | :------: |
| audio: | | | |
| audio-in | | | SPI-I2S, SAI-I2S |
| audio-out | | | SPI-I2S, SAI-I2S |
| fatfs+sd-card+i2s | | | SPI, SDMMC<br>SPI-I2S, SAI-I2S |
| camera: | | | |
| ov2640 | | DCMI+I2C | DCMI+I2C |
| ov7670 | | DCMI+I2C | DCMI+I2C |
| ov7725 | | DCMI+I2C | DCMI+I2C |
| display-color: | | | |
| ili9341+xpt2046 | SPI | SPI, FSMC | SPI, FMC |
| st7735 | SPI | SPI | SPI |
| display-mono: | | | |
| pcd8544 | SPI | SPI | |
| ssd1306 | SPI, I2C | SPI, I2C | |
| ethernet-lwip: | | | |
| lan8720 | | RMII | |
| ethernet-uip: | | | |
| enc28j60 | SPI | SPI | |
| lan8720 | | RMII | |
| file-system: | | | |
| fatfs+sd-card | SPI | SPI, SDIO | SPI, SDMMC |
| spiffs+spi-flash | | SPI | |
| rtc: | | | |
| internal | | internal | |
| pcf8563 | I2C | | |
| sd-card: | | | |
| sd-card | SPI | SPI, SDIO | SPI, SDMMC |
| spi-flash: | | | |
| w25q | SPI | SPI | |
| usb-device: | | | |
| cdc-loopback | | | OTGFS, OTGHS(ULPI) |
| cdc-rndis-uip | | | OTGFS, OTGHS(ULPI) |
| hid-custom | | | OTGFS, OTGHS(ULPI) |
| msc-sdcard | | | OTGFS, OTGHS(ULPI)<br>SPI, SDMMC |
| uac-adc | | | OTGFS, OTGHS(ULPI)<br>SPI-I2S, SAI-I2S |
| uac-dac | | | OTGFS, OTGHS(ULPI)<br>SPI-I2S, SAI-I2S |
| uvc-camera | | | OTGHS+ULPI<br>DCMI+I2C |

Dependencies:<BR>
[STM32 CMSIS Device MCU Components](3rd-party/drivers/cmsis/device/ST/ReadMe.md)
