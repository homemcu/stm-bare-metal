#!/bin/bash
#--------------------------------------------------------------
#
# Copyright (c) 2021 Vladimir Alemasov
# All rights reserved
#
# This program and the accompanying materials are distributed under 
# the terms of GNU General Public License version 2 
# as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
#--------------------------------------------------------------

apps_makefiles=(
    "examples/audio/audio-in/gcc-stm32f746ig"
    "examples/audio/audio-out/gcc-stm32f746ig"
    "examples/audio/fatfs+sd-card+i2s/gcc-stm32f746ig"
    "examples/camera/ov2640/gcc-stm32f407zg"
    "examples/camera/ov2640/gcc-stm32f746ig"
    "examples/camera/ov7670/gcc-stm32f407zg"
    "examples/camera/ov7670/gcc-stm32f746ig"
    "examples/camera/ov7725/gcc-stm32f407zg"
    "examples/camera/ov7725/gcc-stm32f746ig"
    "examples/display-color/ili9341+xpt2046/gcc-stm32f407zg"
    "examples/display-color/ili9341+xpt2046/gcc-stm32f746ig"
    "examples/display-color/st7735/gcc-stm32f407zg"
    "examples/display-color/st7735/gcc-stm32f746ig"
    "examples/display-mono/pcd8544/gcc-stm32f407zg"
    "examples/display-mono/ssd1306/gcc-stm32f407zg"
    "examples/ethernet-lwip/lan8720/gcc-stm32f407zg"
    "examples/ethernet-uip/enc28j60/gcc-stm32f407zg"
    "examples/ethernet-uip/lan8720/gcc-stm32f407zg"
    "examples/file-system/fatfs+sd-card/gcc-stm32f407zg"
    "examples/file-system/fatfs+sd-card/gcc-stm32f746ig"
    "examples/file-system/spiffs+spi-flash/gcc-stm32f407zg"
    "examples/rtc/internal/gcc-stm32f407zg"
    "examples/sd-card/sd-card/gcc-stm32f407zg"
    "examples/sd-card/sd-card/gcc-stm32f746ig"
    "examples/spi-flash/w25q/gcc-stm32f407zg"
    "examples/usb-device/cdc-loopback/gcc-stm32f746ig"
    "examples/usb-device/cdc-rndis-uip/gcc-stm32f746ig"
    "examples/usb-device/hid-custom/gcc-stm32f746ig"
    "examples/usb-device/msc-sdcard/gcc-stm32f746ig"
    "examples/usb-device/uac-adc/gcc-stm32f746ig"
    "examples/usb-device/uac-dac/gcc-stm32f746ig"
    "examples/usb-device/uvc-camera/gcc-stm32f746ig"
)

#-----------------------------------
usage()
{
    echo ""
    echo "Usage: $0 [-b] [-c]"
    echo ""
    echo "   -b     build all arm-none-eabi-gcc projects"
    echo "   -c     clean all arm-none-eabi-gcc projects"
    exit 1
}

#-----------------------------------
clean=0
build=0
shpath="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"/..

echo shpath="$shpath"

while getopts "bc" opt
do
    case "$opt" in
        b ) build=1 ;;
        c ) clean=1 ;;
        ? ) usage ;;
    esac
done

if [ $clean == 0 ] && [ $build == 0 ]
then
    usage
fi

echo ""
echo "======== STM32 examples:"

for makefile in "${apps_makefiles[@]}" ; do
    echo ""
    echo "--------------------------------------"
    echo "project: $shpath/$makefile"
    if [ $build == 1 ]
	then
        echo ""
        echo " --- build ---"
        cd $shpath/$makefile
		make || exit
    fi
    if [ $clean == 1 ]
	then
        echo ""
        echo " --- clean ---"
        cd $shpath/$makefile
		make distclean
    fi
done

echo ""
echo "============="
echo "All builds OK"
