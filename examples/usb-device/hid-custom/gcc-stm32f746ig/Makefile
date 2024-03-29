#--------------------------------------------------------------
#
# Copyright (c) 2019 Vladimir Alemasov
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
# stm32f746ig usb-custom-hid examples
#--------------------------------------------------------------

#--------------------------------------------------------------
# Target definitions
TARGETS = hid-custom-otgfs hid-custom-otghs-fs hid-custom-otghs-ulpi-fs hid-custom-otghs-ulpi-hs
DEF = -DSTM32F746xx -DHSE_VALUE=8000000 -DUART_TERMINAL=1
DEF1 += $(DEF) -DUSBD_OTGFS
DEF2 += $(DEF) -DUSBD_OTGHS_FS
DEF3 += $(DEF) -DUSBD_OTGHS_FS -DUSBD_ULPI
DEF4 += $(DEF) -DUSBD_OTGHS -DUSBD_ULPI

#--------------------------------------------------------------
# Paths
MAINDIR = ../src
CPUDIR = ../../../../cpu/stm32f746ig
HALHDIR = ../../../../hal/inc
HALDIR = ../../../../hal/src/stm32f746ig
LIBHDIR = ../../../../lib/usbd/class
LIBDIR = ../../../../lib/usbd/hid-custom
PLATFORMHDIR = ../../../../platform
PLATFORMCDIR = ../../../../platform/stm32f746ig
PLATFORMFGCCSYSCALLSDIR = ../../../../platform/stm32f746ig/gcc
CMSISDIR = ../../../../3rd-party/drivers/cmsis/core
CMSISHDIR = ../../../../3rd-party/drivers/cmsis/device/ST/cmsis_device_f7/Include
CMSISCDIR = ../../../../3rd-party/drivers/cmsis/device/ST/cmsis_device_f7/Source/Templates
CMSISADIR = ../../../../3rd-party/drivers/cmsis/device/ST/cmsis_device_f7/Source/Templates/gcc
LIBUSBHDIR = ../../../../3rd-party/drivers/libusb_stm32/inc
LIBUSBCDIR = ../../../../3rd-party/drivers/libusb_stm32/src

LINKERSCRIPTDIR = ../../../../platform/stm32f746ig/gcc/linker

#--------------------------------------------------------------
# Include files directories
INCLDIRS += -I$(MAINDIR)
INCLDIRS += -I$(HALHDIR)
INCLDIRS += -I$(LIBDIR)
INCLDIRS += -I$(LIBHDIR)
INCLDIRS += -I$(CPUDIR)
INCLDIRS += -I$(PLATFORMHDIR)
INCLDIRS += -I$(CMSISDIR)
INCLDIRS += -I$(CMSISHDIR)
INCLDIRS += -I$(LIBUSBHDIR)

#--------------------------------------------------------------
# Each source file must be added to the SOURCEFILES list
MAINSOURCEFILE = $(MAINDIR)/main.c
SOURCEFILES += $(MAINSOURCEFILE)
SOURCEFILES += $(CPUDIR)/stm32f7xx-hw.c
SOURCEFILES += $(PLATFORMCDIR)/platform.c
SOURCEFILES += $(PLATFORMFGCCSYSCALLSDIR)/syscalls.c
SOURCEFILES += $(CMSISCDIR)/system_stm32f7xx.c
SOURCEFILES += $(LIBUSBCDIR)/usbd_core.c
SOURCEFILES += $(HALDIR)/hal-usbd-init.c
SOURCEFILES += $(LIBDIR)/usb-hid-custom.c
SOURCEFILES1 += $(SOURCEFILES)
SOURCEFILES1 += $(LIBUSBCDIR)/usbd_stm32f746_otgfs.c
SOURCEFILES2 += $(SOURCEFILES)
SOURCEFILES2 += $(LIBUSBCDIR)/usbd_stm32f746_otghs.c
SOURCEFILES3 += $(SOURCEFILES)
SOURCEFILES3 += $(LIBUSBCDIR)/usbd_stm32f746_otghs.c
SOURCEFILES4 += $(SOURCEFILES)
SOURCEFILES4 += $(LIBUSBCDIR)/usbd_stm32f746_otghs.c

SOURCEASMFILES += $(CMSISADIR)/startup_stm32f746xx.s
SOURCEASMFILES1 += $(SOURCEASMFILES)
SOURCEASMFILES2 += $(SOURCEASMFILES)
SOURCEASMFILES3 += $(SOURCEASMFILES)
SOURCEASMFILES4 += $(SOURCEASMFILES)

LINKERSCRIPT = $(LINKERSCRIPTDIR)/STM32F746IGTx_FLASH.ld

#--------------------------------------------------------------
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
#--------------------------------------------------------------
CFLAGS += -mcpu=cortex-m7
CFLAGS += -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fshort-enums -fomit-frame-pointer -fno-builtin
CFLAGS += -std=c11
CFLAGS += -Wall -Wdouble-promotion
#--------------------------------------------------------------
ASFLAGS =
#--------------------------------------------------------------
LDFLAGS += -mcpu=cortex-m7
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
