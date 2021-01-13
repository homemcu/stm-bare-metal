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
# stm32f407zg ov7725 example
#--------------------------------------------------------------

#--------------------------------------------------------------
# Target definitions
TARGETS = ov7725+ili9341-fsmc ov7725+ili9341-spi
DEF = -DSTM32F407xx -DHSE_VALUE=8000000 -DUART_TERMINAL=1
DEF1 += $(DEF)
DEF1 += -DILI9341_8080I
DEF2 += $(DEF)
DEF2 += -DILI9341_SPI

#--------------------------------------------------------------
# Paths
MAINDIR = ../../src
HALHDIR = ../../../../hal/inc
HALDIR = ../../../../hal/src/stm32f407zg
DRVDIR1 = ../../../../drv/color-scr
DRVDIR2 = ../../../../drv/color-scr/ili9341
DRVDIR3 = ../../../../drv/imgsensor
DRVDIR4 = ../../../../drv/imgsensor/ov7725
LIBHDIR = ../../../../lib/screen
LIBDIR1 = ../../../../lib/screen/color-scr
LIBDIR2 = ../../../../lib/fonts
CPUDIR = ../../../../cpu/stm32f407zg
PLATFORMHDIR = ../../../../platform
PLATFORMCDIR = ../../../../platform/stm32f407zg
PLATFORMFGCCSYSCALLSDIR = ../../../../platform/stm32f407zg/gcc
CMSISDIR = ../../../../3rd-party/drivers/cmsis/core
CMSISHDIR = ../../../../3rd-party/drivers/cmsis/device/ST/cmsis_device_f4/Include
CMSISCDIR = ../../../../3rd-party/drivers/cmsis/device/ST/cmsis_device_f4/Source/Templates
CMSISADIR = ../../../../3rd-party/drivers/cmsis/device/ST/cmsis_device_f4/Source/Templates/gcc

LINKERSCRIPTDIR = ../../../../platform/stm32f407zg/gcc/linker

#--------------------------------------------------------------
# Include files directories
INCLDIRS += -I$(MAINDIR)
INCLDIRS += -I$(HALHDIR)
INCLDIRS += -I$(DRVDIR1)
INCLDIRS += -I$(DRVDIR2)
INCLDIRS += -I$(DRVDIR3)
INCLDIRS += -I$(DRVDIR4)
INCLDIRS += -I$(LIBHDIR)
INCLDIRS += -I$(LIBDIR1)
INCLDIRS += -I$(LIBDIR2)
INCLDIRS += -I$(CPUDIR)
INCLDIRS += -I$(PLATFORMHDIR)
INCLDIRS += -I$(CMSISDIR)
INCLDIRS += -I$(CMSISHDIR)

#--------------------------------------------------------------
# Each source file must be added to the SOURCEFILES list
MAINSOURCEFILE = $(MAINDIR)/main-row.c
SOURCEFILES += $(MAINSOURCEFILE)
SOURCEFILES += $(CPUDIR)/stm32f4xx-hw.c
SOURCEFILES += $(PLATFORMCDIR)/platform.c
SOURCEFILES += $(PLATFORMFGCCSYSCALLSDIR)/syscalls.c
SOURCEFILES += $(CMSISCDIR)/system_stm32f4xx.c
SOURCEFILES += $(HALDIR)/hal-imgsensor-dcmi.c
SOURCEFILES += $(HALDIR)/hal-imgsensor-i2c_ov7670_ov7725.c
SOURCEFILES += $(DRVDIR1)/color-scr-ili9341-drv.c
SOURCEFILES += $(DRVDIR2)/ili9341.c
SOURCEFILES += $(DRVDIR3)/imgsensor-ov7725-drv.c
SOURCEFILES += $(DRVDIR4)/ov7725.c
SOURCEFILES += $(LIBDIR1)/color-scr.c
SOURCEFILES += $(LIBDIR2)/fonts.c
SOURCEFILES1 += $(SOURCEFILES)
SOURCEFILES1 += $(HALDIR)/hal-ili9341-8080I-16bit-fsmc.c
SOURCEFILES2 += $(SOURCEFILES)
SOURCEFILES2 += $(HALDIR)/hal-ili9341-4line-spi.c

SOURCEASMFILES += $(CMSISADIR)/startup_stm32f407xx.s
SOURCEASMFILES1 += $(SOURCEASMFILES)
SOURCEASMFILES2 += $(SOURCEASMFILES)

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
