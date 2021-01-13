::--------------------------------------------------------------
::
:: Copyright (c) 2021 Vladimir Alemasov
:: All rights reserved
::
:: This program and the accompanying materials are distributed under 
:: the terms of GNU General Public License version 2 
:: as published by the Free Software Foundation.
::
:: This program is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
:: GNU General Public License for more details.
::
::--------------------------------------------------------------

@echo off

::----------------------------------
set rootpath=%~dp0..\
set stm32_examples=
set stm32_examples=%stm32_examples%;%rootpath%examples\audio\audio-in\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\audio\audio-out\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\audio\fatfs+sd-card+i2s\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\camera\ov2640\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\camera\ov2640\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\camera\ov7670\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\camera\ov7670\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\camera\ov7725\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\camera\ov7725\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\display-color\ili9341+xpt2046\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\display-color\ili9341+xpt2046\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\display-color\st7735\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\display-color\st7735\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\display-mono\pcd8544\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\display-mono\ssd1306\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\ethernet-lwip\lan8720\iar-stm32f407zg\lwip-freertos.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\ethernet-uip\enc28j60\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\ethernet-uip\enc28j60\iar-stm32f407zg\uip-freertos.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\ethernet-uip\lan8720\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\ethernet-uip\lan8720\iar-stm32f407zg\uip-freertos.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\file-system\fatfs+sd-card\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\file-system\fatfs+sd-card\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\file-system\spiffs+spi-flash\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\rtc\internal\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\sd-card\sd-card\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\sd-card\sd-card\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\spi-flash\w25q\iar-stm32f407zg\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\usb-device\cdc-loopback\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\usb-device\cdc-rndis-uip\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\usb-device\hid-custom\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\usb-device\msc-sdcard\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\usb-device\uac-adc\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\usb-device\uac-dac\iar-stm32f746ig\plain.ewp
set stm32_examples=%stm32_examples%;%rootpath%examples\usb-device\uvc-camera\iar-stm32f746ig\plain.ewp

set stm8_examples=
set stm8_examples=%stm8_examples%;%rootpath%examples\display-color\ili9341+xpt2046\iar-stm8s207r8\plain.ewp
set stm8_examples=%stm8_examples%;%rootpath%examples\display-color\st7735\iar-stm8s207r8\plain.ewp
set stm8_examples=%stm8_examples%;%rootpath%examples\display-mono\pcd8544\iar-stm8s207r8\plain.ewp
set stm8_examples=%stm8_examples%;%rootpath%examples\display-mono\ssd1306\iar-stm8s207r8\plain.ewp
set stm8_examples=%stm8_examples%;%rootpath%examples\ethernet-uip\enc28j60\iar-stm8s207r8\plain.ewp
set stm8_examples=%stm8_examples%;%rootpath%examples\file-system\fatfs+sd-card\iar-stm8s207r8\plain.ewp
set stm8_examples=%stm8_examples%;%rootpath%examples\rtc\pcf8563\iar-stm8s207r8\plain.ewp
set stm8_examples=%stm8_examples%;%rootpath%examples\sd-card\sd-card\iar-stm8s207r8\plain.ewp
set stm8_examples=%stm8_examples%;%rootpath%examples\spi-flash\w25q\iar-stm8s207r8\plain.ewp

::----------------------------------
set iarbuild_stm32="C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.4\common\bin\IarBuild.exe"
set iarbuild_stm8="C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.3\common\bin\IarBuild.exe"
:: set iarbuild_log=-log all
set iarbuild_log=
set xpath=%~dp0xpath.bat

::----------------------------------
set "clean=0"
set "build=0"
set curpath=%~dp0

for %%A in (%*) do (
    if "%%A"=="/c"      set "clean=1"
    if "%%A"=="-c"      set "clean=1"
    if "%%A"=="--clean" set "clean=1"
    if "%%A"=="/b"      set "build=1"
    if "%%A"=="-b"      set "build=1"
    if "%%A"=="--build" set "build=1"
)
if "%build%"=="0" ( if "%clean%"=="0" goto:usage )
goto:main

::----------------------------------
:: functions
::----------------------------------
:build_stm32
echo.
echo --------------------------------------
echo project: %~1
set filepath=%~dp1
if "%build%"=="1" (
    echo.
    echo. --- build ---
    %iarbuild_stm32% %~1 * %iarbuild_log%
    if %ERRORLEVEL% neq 0 ( exit /b %ERRORLEVEL% )
)
if "%clean%"=="1" (
    echo.
    echo. --- clean ---
    setlocal enabledelayedexpansion
    set "list="
    for /f "tokens=* delims=" %%F in ('%xpath% "%~1" "//configuration[*]/name"') do (
        set list=!list!;%%F
    )
    if %ERRORLEVEL% neq 0 ( exit /b %ERRORLEVEL% )
    for %%a in (!list!) do (
        echo. %filepath%%%a
        rd /s /q %filepath%%%a
    )
)
exit /b %ERRORLEVEL%

::----------------------------------
:build_stm8
echo.
echo --------------------------------------
echo project: %~1
set filepath=%~dp1
if "%build%"=="1" (
    echo.
    echo. --- build ---
    %iarbuild_stm8% %~1 * %iarbuild_log%
    if %ERRORLEVEL% neq 0 ( exit /b %ERRORLEVEL% )
)
if "%clean%"=="1" (
    echo.
    echo. --- clean ---
    setlocal enabledelayedexpansion
    set "list="
    for /f "tokens=* delims=" %%F in ('%xpath% "%~1" "//configuration[*]/name"') do (
        set list=!list!;%%F
    )
    if %ERRORLEVEL% neq 0 ( exit /b %ERRORLEVEL% )
    for %%a in (!list!) do (
        echo. %filepath%%%a
        rd /s /q %filepath%%%a
    )
)
exit /b %ERRORLEVEL%

::----------------------------------
:main
echo.
echo ======== STM32 examples:

for %%a in (%stm32_examples%) do (
    call::build_stm32 %%a
    if %ERRORLEVEL% neq 0 (
	    goto:error
	)
)

echo.
echo ======== STM8 examples:

for %%a in (%stm8_examples%) do (
    call::build_stm8 %%a
    if %ERRORLEVEL% neq 0 (
	    goto:error
	)
)

echo.
echo =============
echo All builds OK
goto:exit

::----------------------------------
:error
echo.
echo =============
echo Build failed
goto:exit

::----------------------------------
:usage
echo =============
echo USAGE:
echo   %~nx0 [-b] [-c]
echo.
echo.  /b, -b, --build          build all IAR projects
echo.  /c, -c, --clean          clean all IAR projects
goto:eof

::----------------------------------
:exit
