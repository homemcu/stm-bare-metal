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
# stm32f407zg ethernet-lwip example
#--------------------------------------------------------------

#--------------------------------------------------------------
# Target definitions
TARGETS = ethernet-lwip
DEF1 = -DSTM32F407xx -DHSE_VALUE=8000000 -DLWIP_FREERTOS -DUART_TERMINAL=1

#--------------------------------------------------------------
# Paths
MAINDIR = ../src
CPUDIR = ../../../../cpu/stm32f407zg
DEVDIR = ../../../../devices/ethernet-uip/lan8720/src/dev
HALDIR = ../../../../devices/ethernet-uip/lan8720/src/platform/stm32f407zg/dev
PLATFORMHDIR = ../../../../platform
PLATFORMFREERTOSHDIR = ../../../../platform/stm32f407zg/freertos
PLATFORMCDIR = ../../../../platform/stm32f407zg
PLATFORMFGCCSYSCALLSDIR = ../../../../platform/stm32f407zg/gcc
CMSISDIR = ../../../../3rd-party/drivers/cmsis/core
CMSISHDIR = ../../../../3rd-party/drivers/cmsis/device/ST/STM32F4xx/Include
CMSISCDIR = ../../../../3rd-party/drivers/cmsis/device/ST/STM32F4xx/Source/Templates
CMSISADIR = ../../../../3rd-party/drivers/cmsis/device/ST/STM32F4xx/Source/Templates/gcc
FREERTOSDIR = ../../../../3rd-party/middlewares/freertos
FREERTOSHDIR = ../../../../3rd-party/middlewares/freertos/include
FREERTOSMEMMANGDIR = ../../../../3rd-party/middlewares/freertos/portable/MemMang
FREERTOSPORTDIR = ../../../../3rd-party/middlewares/freertos/portable/GCC/ARM_CM4F
LWIPH1DIR = ../../../../3rd-party/middlewares/lwip/src/include
LWIPH2DIR = ../../../../3rd-party/middlewares/lwip/src/include/lwip
LWIPH3DIR = ../../../../3rd-party/middlewares/lwip/src/include/netif
LWIPAPIDIR = ../../../../3rd-party/middlewares/lwip/src/api
LWIPCOREDIR = ../../../../3rd-party/middlewares/lwip/src/core
LWIPCOREIPV4DIR = ../../../../3rd-party/middlewares/lwip/src/core/ipv4
LWIPNETIFDIR = ../../../../3rd-party/middlewares/lwip/src/netif
LWIPPORT1DIR = ../src/lwip
LWIPPORT2DIR = ../src/lwip/port
LINKERSCRIPTDIR = ../../../../3rd-party/drivers/cmsis/device/ST/STM32F4xx/Source/Templates/gcc/linker

#--------------------------------------------------------------
# Include files directories
INCLDIRS += -I$(MAINDIR)
INCLDIRS += -I$(CPUDIR)
INCLDIRS += -I$(DEVDIR)
INCLDIRS += -I$(PLATFORMHDIR)
INCLDIRS += -I$(PLATFORMFREERTOSHDIR)
INCLDIRS += -I$(CMSISDIR)
INCLDIRS += -I$(CMSISHDIR)
INCLDIRS += -I$(FREERTOSHDIR)
INCLDIRS += -I$(FREERTOSPORTDIR)
INCLDIRS += -I$(LWIPH1DIR)
INCLDIRS += -I$(LWIPH2DIR)
INCLDIRS += -I$(LWIPH3DIR)
INCLDIRS += -I$(LWIPPORT1DIR)
INCLDIRS += -I$(LWIPPORT2DIR)

#--------------------------------------------------------------
# Each source file must be added to the SOURCEFILES list
MAINSOURCEFILE = $(MAINDIR)/main.c
SOURCEFILES1 += $(MAINSOURCEFILE)
SOURCEFILES1 += $(MAINDIR)/eth-lwip-lan8720-drv.c
SOURCEFILES1 += $(MAINDIR)/eth-lwip-freertos.c
SOURCEFILES1 += $(CPUDIR)/stm32f4xx-hw.c
SOURCEFILES1 += $(HALDIR)/hal-lan8720-rmii.c
SOURCEFILES1 += $(PLATFORMCDIR)/platform-freertos.c
SOURCEFILES1 += $(PLATFORMFGCCSYSCALLSDIR)/syscalls.c
SOURCEFILES1 += $(CMSISCDIR)/system_stm32f4xx.c
SOURCEFILES1 += $(FREERTOSDIR)/list.c
SOURCEFILES1 += $(FREERTOSDIR)/queue.c
SOURCEFILES1 += $(FREERTOSDIR)/tasks.c
SOURCEFILES1 += $(FREERTOSMEMMANGDIR)/heap_4.c
SOURCEFILES1 += $(FREERTOSPORTDIR)/port.c
SOURCEFILES1 += $(LWIPAPIDIR)/api_lib.c
SOURCEFILES1 += $(LWIPAPIDIR)/api_msg.c
SOURCEFILES1 += $(LWIPAPIDIR)/err.c
SOURCEFILES1 += $(LWIPAPIDIR)/netbuf.c
SOURCEFILES1 += $(LWIPAPIDIR)/netdb.c
SOURCEFILES1 += $(LWIPAPIDIR)/netifapi.c
SOURCEFILES1 += $(LWIPAPIDIR)/sockets.c
SOURCEFILES1 += $(LWIPAPIDIR)/tcpip.c
SOURCEFILES1 += $(LWIPCOREIPV4DIR)/autoip.c
SOURCEFILES1 += $(LWIPCOREIPV4DIR)/dhcp.c
SOURCEFILES1 += $(LWIPCOREIPV4DIR)/etharp.c
SOURCEFILES1 += $(LWIPCOREIPV4DIR)/icmp.c
SOURCEFILES1 += $(LWIPCOREIPV4DIR)/igmp.c
SOURCEFILES1 += $(LWIPCOREIPV4DIR)/ip4.c
SOURCEFILES1 += $(LWIPCOREIPV4DIR)/ip4_addr.c
SOURCEFILES1 += $(LWIPCOREIPV4DIR)/ip4_frag.c
SOURCEFILES1 += $(LWIPCOREDIR)/def.c
SOURCEFILES1 += $(LWIPCOREDIR)/dns.c
SOURCEFILES1 += $(LWIPCOREDIR)/inet_chksum.c
SOURCEFILES1 += $(LWIPCOREDIR)/init.c
SOURCEFILES1 += $(LWIPCOREDIR)/ip.c
SOURCEFILES1 += $(LWIPCOREDIR)/mem.c
SOURCEFILES1 += $(LWIPCOREDIR)/memp.c
SOURCEFILES1 += $(LWIPCOREDIR)/netif.c
SOURCEFILES1 += $(LWIPCOREDIR)/pbuf.c
SOURCEFILES1 += $(LWIPCOREDIR)/raw.c
SOURCEFILES1 += $(LWIPCOREDIR)/stats.c
SOURCEFILES1 += $(LWIPCOREDIR)/sys.c
SOURCEFILES1 += $(LWIPCOREDIR)/tcp.c
SOURCEFILES1 += $(LWIPCOREDIR)/tcp_in.c
SOURCEFILES1 += $(LWIPCOREDIR)/tcp_out.c
SOURCEFILES1 += $(LWIPCOREDIR)/timeouts.c
SOURCEFILES1 += $(LWIPCOREDIR)/udp.c
SOURCEFILES1 += $(LWIPNETIFDIR)/ethernet.c
SOURCEFILES1 += $(LWIPPORT1DIR)/ethernetif.c
SOURCEFILES1 += $(LWIPPORT2DIR)/sys_arch.c

SOURCEASMFILES += $(CMSISADIR)/startup_stm32f407xx.s
SOURCEASMFILES1 += $(SOURCEASMFILES)

LINKERSCRIPT = $(LINKERSCRIPTDIR)/STM32F407ZGTx_FLASH.ld

#--------------------------------------------------------------
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy
#--------------------------------------------------------------
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fshort-enums -fomit-frame-pointer -fno-builtin
CFLAGS += -std=c99
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
LIBS =
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
