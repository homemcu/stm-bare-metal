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

sdcc_home="/opt/sdcc-4.0.0"

apps_makefiles=(
    "examples/display-color/ili9341+xpt2046/sdcc-stm8s207r8"
    "examples/display-color/st7735/sdcc-stm8s207r8"
    "examples/display-mono/pcd8544/sdcc-stm8s207r8"
    "examples/display-mono/ssd1306/sdcc-stm8s207r8"
    "examples/ethernet-uip/enc28j60/sdcc-stm8s207r8"
    "examples/file-system/fatfs+sd-card/sdcc-stm8s207r8"
    "examples/rtc/pcf8563/sdcc-stm8s207r8"
    "examples/sd-card/sd-card/sdcc-stm8s207r8"
    "examples/spi-flash/w25q/sdcc-stm8s207r8"
)

#-----------------------------------
usage()
{
    echo ""
    echo "Usage: $0 [-b] [-c]"
    echo ""
    echo "   -b     build all sdcc projects"
    echo "   -c     clean all sdcc projects"
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
echo "======== STM8 examples:"

for makefile in "${apps_makefiles[@]}" ; do
    echo ""
    echo "--------------------------------------"
    echo "project: $shpath/$makefile"
    if [ $build == 1 ]
	then
        echo ""
        echo " --- build ---"
        cd $shpath/$makefile
		make SDCC_HOME=$sdcc_home || exit
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
