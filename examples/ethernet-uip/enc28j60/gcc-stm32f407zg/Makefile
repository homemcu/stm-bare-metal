#--------------------------------------------------------------
#
# Copyright (c) 2018 Vladimir Alemasov
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
# stm32f407zg enc28j60-uip examples
#--------------------------------------------------------------

#--------------------------------------------------------------
# Target definitions
TARGETS = hello-world dhcp telnetd smtp resolv webclient webserver freertos-hello-world freertos-dhcp freertos-telnetd freertos-smtp freertos-resolv freertos-webclient freertos-webserver
DEF = -DSTM32F407xx -DHSE_VALUE=8000000 -DUART_TERMINAL=1
DEF1 += $(DEF)
DEF1 += -DUIP_EXAMPLE_HELLO_WORLD
DEF2 += $(DEF)
DEF2 += -DUIP_EXAMPLE_DHCPC
DEF3 += $(DEF)
DEF3 += -DUIP_EXAMPLE_TELNETD
DEF4 += $(DEF)
DEF4 += -DUIP_EXAMPLE_SMTP
DEF5 += $(DEF)
DEF5 += -DUIP_EXAMPLE_RESOLV
DEF6 += $(DEF)
DEF6 += -DUIP_EXAMPLE_WEBCLIENT
DEF7 += $(DEF)
DEF7 += -DUIP_EXAMPLE_WEBSERVER
DEF8 = $(DEF1)
DEF9 = $(DEF2)
DEF10 = $(DEF3)
DEF11 = $(DEF4)
DEF12 = $(DEF5)
DEF13 = $(DEF6)
DEF14 = $(DEF7)


#--------------------------------------------------------------
# Paths
MAINDIR = ../../src
HALHDIR = ../../../../hal/inc
HALDIR = ../../../../hal/src/stm32f407zg
DRVHDIR = ../../../../drv/ethernet
DRVDIR1 = ../../../../drv/ethernet/eth-uip
DRVDIR2 = ../../../../drv/ethernet/enc28j60
CPUDIR = ../../../../cpu/stm32f407zg
PLATFORMHDIR = ../../../../platform
PLATFORMFREERTOSHDIR = ../../../../platform/stm32f407zg/freertos
PLATFORMCDIR = ../../../../platform/stm32f407zg
PLATFORMFGCCSYSCALLSDIR = ../../../../platform/stm32f407zg/gcc
CMSISDIR = ../../../../3rd-party/drivers/cmsis/core
CMSISHDIR = ../../../../3rd-party/drivers/cmsis/device/ST/cmsis_device_f4/Include
CMSISCDIR = ../../../../3rd-party/drivers/cmsis/device/ST/cmsis_device_f4/Source/Templates
CMSISADIR = ../../../../3rd-party/drivers/cmsis/device/ST/cmsis_device_f4/Source/Templates/gcc
FREERTOSDIR = ../../../../3rd-party/middlewares/freertos
FREERTOSHDIR = ../../../../3rd-party/middlewares/freertos/include
FREERTOSMEMMANGDIR = ../../../../3rd-party/middlewares/freertos/portable/MemMang
FREERTOSPORTDIR = ../../../../3rd-party/middlewares/freertos/portable/GCC/ARM_CM4F
UIPDIR = ../../../../3rd-party/middlewares/uip
UIPPORTDIR = ../../../../lib/uip
APP1DIR = ../../../../3rd-party/middlewares/uip/apps/hello-world
APP2DIR = ../../../../3rd-party/middlewares/uip/apps/dhcpc
APP3DIR = ../../../../3rd-party/middlewares/uip/apps/telnetd
APP4DIR = ../../../../3rd-party/middlewares/uip/apps/smtp
APP5DIR = ../../../../3rd-party/middlewares/uip/apps/resolv
APP6DIR = ../../../../3rd-party/middlewares/uip/apps/webclient
APP7DIR = ../../../../3rd-party/middlewares/uip/apps/webserver

LINKERSCRIPTDIR = ../../../../platform/stm32f407zg/gcc/linker

#--------------------------------------------------------------
# Include files directories
INCLDIRS += -I$(MAINDIR)
INCLDIRS += -I$(HALHDIR)
INCLDIRS += -I$(CPUDIR)
INCLDIRS += -I$(DRVHDIR)
INCLDIRS += -I$(DRVDIR1)
INCLDIRS += -I$(DRVDIR2)
INCLDIRS += -I$(PLATFORMHDIR)
INCLDIRS += -I$(PLATFORMFREERTOSHDIR)
INCLDIRS += -I$(CMSISDIR)
INCLDIRS += -I$(CMSISHDIR)
INCLDIRS += -I$(FREERTOSHDIR)
INCLDIRS += -I$(FREERTOSPORTDIR)
INCLDIRS += -I$(UIPDIR)
INCLDIRS += -I$(UIPPORTDIR)
INCLDIRS += -I$(APP1DIR)
INCLDIRS += -I$(APP2DIR)
INCLDIRS += -I$(APP3DIR)
INCLDIRS += -I$(APP4DIR)
INCLDIRS += -I$(APP5DIR)
INCLDIRS += -I$(APP6DIR)
INCLDIRS += -I$(APP7DIR)

#--------------------------------------------------------------
# Each source file must be added to the SOURCEFILES1...n list
MAINSOURCEFILE = $(MAINDIR)/main.c
SOURCEFILES += $(MAINSOURCEFILE)
SOURCEFILES += $(HALDIR)/hal-enc28j60.c
SOURCEFILES += $(DRVDIR1)/eth-uip-enc28j60-drv.c
SOURCEFILES += $(DRVDIR2)/enc28j60.c
SOURCEFILES += $(CPUDIR)/stm32f4xx-hw.c
SOURCEFILES += $(CMSISCDIR)/system_stm32f4xx.c
SOURCEFILES += $(PLATFORMFGCCSYSCALLSDIR)/syscalls.c
SOURCEFILESFREERTOS += $(FREERTOSDIR)/list.c
SOURCEFILESFREERTOS += $(FREERTOSDIR)/queue.c
SOURCEFILESFREERTOS += $(FREERTOSDIR)/tasks.c
SOURCEFILESFREERTOS += $(FREERTOSMEMMANGDIR)/heap_4.c
SOURCEFILESFREERTOS += $(FREERTOSPORTDIR)/port.c
SOURCEFILESUIP += $(UIPDIR)/psock.c
SOURCEFILESUIP += $(UIPDIR)/timer.c
SOURCEFILESUIP += $(UIPDIR)/uip.c
SOURCEFILESUIP += $(UIPDIR)/uip_arp.c
SOURCEFILESUIP += $(UIPDIR)/uip-fw.c
SOURCEFILESUIP += $(UIPDIR)/uiplib.c
SOURCEFILESUIP += $(UIPDIR)/uip-neighbor.c
SOURCEFILESUIP += $(UIPPORTDIR)/clock-arch.c

SOURCEFILES1 += $(SOURCEFILES)
SOURCEFILES1 += $(DRVDIR1)/eth-uip-plain.c
SOURCEFILES1 += $(PLATFORMCDIR)/platform.c
SOURCEFILES1 += $(SOURCEFILESUIP)
SOURCEFILES1 += $(APP1DIR)/hello-world.c

SOURCEFILES2 += $(SOURCEFILES)
SOURCEFILES2 += $(DRVDIR1)/eth-uip-plain.c
SOURCEFILES2 += $(PLATFORMCDIR)/platform.c
SOURCEFILES2 += $(SOURCEFILESUIP)
SOURCEFILES2 += $(APP2DIR)/dhcpc.c
SOURCEFILES2 += $(APP3DIR)/memb.c
SOURCEFILES2 += $(APP3DIR)/shell.c
SOURCEFILES2 += $(APP3DIR)/telnetd.c

SOURCEFILES3 += $(SOURCEFILES)
SOURCEFILES3 += $(DRVDIR1)/eth-uip-plain.c
SOURCEFILES3 += $(PLATFORMCDIR)/platform.c
SOURCEFILES3 += $(SOURCEFILESUIP)
SOURCEFILES3 += $(APP3DIR)/memb.c
SOURCEFILES3 += $(APP3DIR)/shell.c
SOURCEFILES3 += $(APP3DIR)/telnetd.c

SOURCEFILES4 += $(SOURCEFILES)
SOURCEFILES4 += $(DRVDIR1)/eth-uip-plain.c
SOURCEFILES4 += $(PLATFORMCDIR)/platform.c
SOURCEFILES4 += $(SOURCEFILESUIP)
SOURCEFILES4 += $(APP4DIR)/smtp.c
SOURCEFILES4 += $(APP4DIR)/smtp-strings.c

SOURCEFILES5 += $(SOURCEFILES)
SOURCEFILES5 += $(DRVDIR1)/eth-uip-plain.c
SOURCEFILES5 += $(PLATFORMCDIR)/platform.c
SOURCEFILES5 += $(SOURCEFILESUIP)
SOURCEFILES5 += $(APP5DIR)/resolv.c
SOURCEFILES5 += $(APP6DIR)/webclient.c
SOURCEFILES5 += $(APP6DIR)/webclient-strings.c

SOURCEFILES6 += $(SOURCEFILES)
SOURCEFILES6 += $(DRVDIR1)/eth-uip-plain.c
SOURCEFILES6 += $(PLATFORMCDIR)/platform.c
SOURCEFILES6 += $(SOURCEFILESUIP)
SOURCEFILES6 += $(APP5DIR)/resolv.c
SOURCEFILES6 += $(APP6DIR)/webclient.c
SOURCEFILES6 += $(APP6DIR)/webclient-strings.c

SOURCEFILES7 += $(SOURCEFILES)
SOURCEFILES7 += $(DRVDIR1)/eth-uip-plain.c
SOURCEFILES7 += $(PLATFORMCDIR)/platform.c
SOURCEFILES7 += $(SOURCEFILESUIP)
SOURCEFILES7 += $(APP7DIR)/http-strings.c
SOURCEFILES7 += $(APP7DIR)/httpd.c
SOURCEFILES7 += $(APP7DIR)/httpd-cgi.c
SOURCEFILES7 += $(APP7DIR)/httpd-fs.c

SOURCEFILES8 += $(SOURCEFILES)
SOURCEFILES8 += $(DRVDIR1)/eth-uip-freertos.c
SOURCEFILES8 += $(PLATFORMCDIR)/platform-freertos.c
SOURCEFILES8 += $(SOURCEFILESFREERTOS)
SOURCEFILES8 += $(SOURCEFILESUIP)
SOURCEFILES8 += $(APP1DIR)/hello-world.c

SOURCEFILES9 += $(SOURCEFILES)
SOURCEFILES9 += $(DRVDIR1)/eth-uip-freertos.c
SOURCEFILES9 += $(PLATFORMCDIR)/platform-freertos.c
SOURCEFILES9 += $(SOURCEFILESFREERTOS)
SOURCEFILES9 += $(SOURCEFILESUIP)
SOURCEFILES9 += $(APP2DIR)/dhcpc.c
SOURCEFILES9 += $(APP3DIR)/memb.c
SOURCEFILES9 += $(APP3DIR)/shell.c
SOURCEFILES9 += $(APP3DIR)/telnetd.c

SOURCEFILES10 += $(SOURCEFILES)
SOURCEFILES10 += $(DRVDIR1)/eth-uip-freertos.c
SOURCEFILES10 += $(PLATFORMCDIR)/platform-freertos.c
SOURCEFILES10 += $(SOURCEFILESFREERTOS)
SOURCEFILES10 += $(SOURCEFILESUIP)
SOURCEFILES10 += $(APP3DIR)/memb.c
SOURCEFILES10 += $(APP3DIR)/shell.c
SOURCEFILES10 += $(APP3DIR)/telnetd.c

SOURCEFILES11 += $(SOURCEFILES)
SOURCEFILES11 += $(DRVDIR1)/eth-uip-freertos.c
SOURCEFILES11 += $(PLATFORMCDIR)/platform-freertos.c
SOURCEFILES11 += $(SOURCEFILESFREERTOS)
SOURCEFILES11 += $(SOURCEFILESUIP)
SOURCEFILES11 += $(APP4DIR)/smtp.c
SOURCEFILES11 += $(APP4DIR)/smtp-strings.c

SOURCEFILES12 += $(SOURCEFILES)
SOURCEFILES12 += $(DRVDIR1)/eth-uip-freertos.c
SOURCEFILES12 += $(PLATFORMCDIR)/platform-freertos.c
SOURCEFILES12 += $(SOURCEFILESFREERTOS)
SOURCEFILES12 += $(SOURCEFILESUIP)
SOURCEFILES12 += $(APP5DIR)/resolv.c
SOURCEFILES12 += $(APP6DIR)/webclient.c
SOURCEFILES12 += $(APP6DIR)/webclient-strings.c

SOURCEFILES13 += $(SOURCEFILES)
SOURCEFILES13 += $(DRVDIR1)/eth-uip-freertos.c
SOURCEFILES13 += $(PLATFORMCDIR)/platform-freertos.c
SOURCEFILES13 += $(SOURCEFILESFREERTOS)
SOURCEFILES13 += $(SOURCEFILESUIP)
SOURCEFILES13 += $(APP5DIR)/resolv.c
SOURCEFILES13 += $(APP6DIR)/webclient.c
SOURCEFILES13 += $(APP6DIR)/webclient-strings.c

SOURCEFILES14 += $(SOURCEFILES)
SOURCEFILES14 += $(DRVDIR1)/eth-uip-freertos.c
SOURCEFILES14 += $(PLATFORMCDIR)/platform-freertos.c
SOURCEFILES14 += $(SOURCEFILESFREERTOS)
SOURCEFILES14 += $(SOURCEFILESUIP)
SOURCEFILES14 += $(APP7DIR)/http-strings.c
SOURCEFILES14 += $(APP7DIR)/httpd.c
SOURCEFILES14 += $(APP7DIR)/httpd-cgi.c
SOURCEFILES14 += $(APP7DIR)/httpd-fs.c

SOURCEASMFILES += $(CMSISADIR)/startup_stm32f407xx.s
SOURCEASMFILES1 += $(SOURCEASMFILES)
SOURCEASMFILES2 += $(SOURCEASMFILES)
SOURCEASMFILES3 += $(SOURCEASMFILES)
SOURCEASMFILES4 += $(SOURCEASMFILES)
SOURCEASMFILES5 += $(SOURCEASMFILES)
SOURCEASMFILES6 += $(SOURCEASMFILES)
SOURCEASMFILES7 += $(SOURCEASMFILES)
SOURCEASMFILES8 += $(SOURCEASMFILES)
SOURCEASMFILES9 += $(SOURCEASMFILES)
SOURCEASMFILES10 += $(SOURCEASMFILES)
SOURCEASMFILES11 += $(SOURCEASMFILES)
SOURCEASMFILES12 += $(SOURCEASMFILES)
SOURCEASMFILES13 += $(SOURCEASMFILES)
SOURCEASMFILES14 += $(SOURCEASMFILES)

LINKERSCRIPT = $(LINKERSCRIPTDIR)/STM32F407ZGTx_FLASH.ld

#--------------------------------------------------------------
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
#--------------------------------------------------------------
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fshort-enums -fomit-frame-pointer -fno-builtin
CFLAGS += -std=c11
CFLAGS += -Wall -Wdouble-promotion
#--------------------------------------------------------------
ASFLAGS =
#--------------------------------------------------------------
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LDFLAGS += -specs=nano.specs
LDFLAGS += -T$(LINKERSCRIPT)
#--------------------------------------------------------------
# Libraries
LIBS = -lgcc
LIBDIRS =

#--------------------------------------------------------------
# The function creates the directory name for object files from the target name
# parameters:
# $(1) - target name
target2objdir = $(addsuffix _obj,$(1))
#--------------------------------------------------------------
#--------------------------------------------------------------
# The function creates the object filename from the source filename
# parameters:
# $(1) - directory name for object files
# $(2) - the c source filename(s) with (or without) path
c2obj = $(addprefix $(1)/,$(notdir $(patsubst %.c,%.o,$(2))))
#--------------------------------------------------------------
# The function creates the object filename from the source filename
# parameters:
# $(1) - directory name for object files
# $(2) - the asm source filename(s) with (or without) path
s2obj = $(addprefix $(1)/,$(notdir $(patsubst %.s,%.o,$(2))))
#--------------------------------------------------------------
#--------------------------------------------------------------
# The function creates an explicit rule based on a template common to all object files
# parameters:
# $(1) - object filename with path
# $(2) - c source filename with path
# $(3) - directory name for object files
# $(4) - c preprocessor definitions
define makecrule
$(1): $(2) | $(3)
	@echo $$<
	@$(CC) $(CFLAGS) $(4) $$< -o $$@ $(INCLDIRS) -c -MMD
endef
#--------------------------------------------------------------
# The function creates an explicit rule based on a template common to all object files
# parameters:
# $(1) - object filename with path
# $(2) - asm source filename with path
# $(3) - directory name for object files
define makesrule
$(1): $(2) | $(3)
	@echo $$<
	@$(AS) $(ASLAGS) $$< -o $$@
endef
#--------------------------------------------------------------
#--------------------------------------------------------------
# The function creates an explicit rule based on a template common to all targets
# parameters:
# $(1) - target name
# $(2) - directory name for object files
# $(3) - all object file names with path
define makerule_target
.PHONY: $(1)
$(1): $(1).hex $(1).bin
# Create directory for object files
$(2):
	@mkdir $$@
# Link firmware
$(1).elf: $(3)
	@echo ===========================
	@echo Creating elf file: $$@
	@$(LD) $(LDFLAGS) $(LD_PRE_FLAGS) $$^ -o $$@ $(LIBDIRS) $(LIBS)
# Post-process the hex file for programmers which dislike gcc output elf format
$(1).hex: $(1).elf
	@echo Creating hex file: $$@
	@$(OBJCOPY) -O ihex $$< $$@
# Post-process the bin file for programmers which dislike gcc output elf format
$(1).bin: $(1).elf
	@echo Creating bin file: $$@
	@$(OBJCOPY) -O binary $$< $$@
	@echo ===========================
endef
#--------------------------------------------------------------
#--------------------------------------------------------------
# The function creates an explicit rule to clean target
# parameters:
# $(1) - directory names for object files
define makerule_clean
.PHONY: clean
clean:
	@rm -rf $(1)
endef
#--------------------------------------------------------------
#--------------------------------------------------------------
# Additional functions
get_target_name = $(word $(1),$(TARGETS))
get_object_dir_name = $(call target2objdir,$(call get_target_name,$(1)))
get_object_file_names = $(call c2obj,$(call target2objdir,$(word $(1),$(TARGETS))),$(SOURCEFILES$(1)))
get_asm_object_file_names = $(call s2obj,$(call target2objdir,$(word $(1),$(TARGETS))),$(SOURCEASMFILES$(1)))
get_all_object_file_names = $(call get_object_file_names,$(1)) $(call get_asm_object_file_names,$(1))
#--------------------------------------------------------------


.PHONY: all
all: $(TARGETS)

CNTLIST = $(shell for x in $$(seq 1 $(words $(TARGETS))); do echo $$x; done)

define makerules
$(foreach src,$(SOURCEFILES$(1)),$(eval $(call makecrule,$(call c2obj,$(call get_object_dir_name,$(1)),$(src)),$(src),$(call get_object_dir_name,$(1)),$(DEF$(1)))))
$(foreach src,$(SOURCEASMFILES$(1)),$(eval $(call makesrule,$(call s2obj,$(call get_object_dir_name,$(1)),$(src)),$(src),$(call get_object_dir_name,$(1)))))
$(eval $(call makerule_target,$(call get_target_name,$(1)),$(call get_object_dir_name,$(1)),$(call get_all_object_file_names,$(1))))
# Include additional explicit dependencies without recipes from the compiler (*.d files in the object directories)
-include $(call get_object_dir_name,$(1))/*.d
endef

$(foreach cnt,$(CNTLIST),$(eval $(call makerules,$(cnt))))

get_object_dir_names = $(foreach cnt,$(CNTLIST),$(call get_object_dir_name,$(cnt)))
$(eval $(call makerule_clean,$(call get_object_dir_names)))

.PHONY: distclean
distclean: clean
	@rm -f *.hex *.elf *.bin
