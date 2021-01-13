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
# stm8s207r8 uip examples
#--------------------------------------------------------------

#--------------------------------------------------------------
# Target definitions
TARGETS = hello-world dhcp telnetd smtp resolv webclient
DEF = -DSTM8S207 -DUIP_CONF_BYTE_ORDER=UIP_BIG_ENDIAN -DUART_TERMINAL=1
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

#--------------------------------------------------------------
# Paths
MAINDIR = ../../src
CPUDIR = ../../../../cpu/stm8s207r8
HALHDIR = ../../../../hal/inc
HALDIR = ../../../../hal/src/stm8s207r8
DRVHDIR = ../../../../drv/ethernet
DRVDIR1 = ../../../../drv/ethernet/eth-uip
DRVDIR2 = ../../../../drv/ethernet/enc28j60
PLATFORMHDIR = ../../../../platform
PLATFORMCDIR = ../../../../platform/stm8s207r8
UIPDIR = ../../../../3rd-party/middlewares/uip
UIPPORTDIR = ../../../../lib/uip
APP1DIR = ../../../../3rd-party/middlewares/uip/apps/hello-world
APP2DIR = ../../../../3rd-party/middlewares/uip/apps/dhcpc
APP3DIR = ../../../../3rd-party/middlewares/uip/apps/telnetd
APP4DIR = ../../../../3rd-party/middlewares/uip/apps/smtp
APP5DIR = ../../../../3rd-party/middlewares/uip/apps/resolv
APP6DIR = ../../../../3rd-party/middlewares/uip/apps/webclient

#--------------------------------------------------------------
# Include files directories
INCLDIRS += -I$(MAINDIR)
INCLDIRS += -I$(HALHDIR)
INCLDIRS += -I$(CPUDIR)
INCLDIRS += -I$(DRVHDIR)
INCLDIRS += -I$(DRVDIR1)
INCLDIRS += -I$(DRVDIR2)
INCLDIRS += -I$(PLATFORMHDIR)
INCLDIRS += -I$(UIPDIR)
INCLDIRS += -I$(UIPPORTDIR)
INCLDIRS += -I$(APP1DIR)
INCLDIRS += -I$(APP2DIR)
INCLDIRS += -I$(APP3DIR)
INCLDIRS += -I$(APP4DIR)
INCLDIRS += -I$(APP5DIR)
INCLDIRS += -I$(APP6DIR)

#--------------------------------------------------------------
# Each source file must be added to the SOURCEFILES list
MAINSOURCEFILE = $(MAINDIR)/main.c
SOURCEFILES += $(MAINSOURCEFILE)
SOURCEFILES += $(HALDIR)/hal-enc28j60.c
SOURCEFILES += $(DRVDIR1)/eth-uip-plain.c
SOURCEFILES += $(DRVDIR1)/eth-uip-enc28j60-drv.c
SOURCEFILES += $(DRVDIR2)/enc28j60.c
SOURCEFILES += $(PLATFORMCDIR)/platform.c
SOURCEFILES += $(UIPDIR)/psock.c
SOURCEFILES += $(UIPDIR)/timer.c
SOURCEFILES += $(UIPDIR)/uip.c
SOURCEFILES += $(UIPDIR)/uip_arp.c
SOURCEFILES += $(UIPDIR)/uip-fw.c
SOURCEFILES += $(UIPDIR)/uiplib.c
SOURCEFILES += $(UIPDIR)/uip-neighbor.c
SOURCEFILES += $(UIPPORTDIR)/clock-arch.c
SOURCEFILES1 += $(SOURCEFILES)
SOURCEFILES1 += $(APP1DIR)/hello-world.c
SOURCEFILES2 += $(SOURCEFILES)
SOURCEFILES2 += $(APP2DIR)/dhcpc.c
SOURCEFILES2 += $(APP3DIR)/memb.c
SOURCEFILES2 += $(APP3DIR)/shell.c
SOURCEFILES2 += $(APP3DIR)/telnetd.c
SOURCEFILES3 += $(SOURCEFILES)
SOURCEFILES3 += $(APP3DIR)/memb.c
SOURCEFILES3 += $(APP3DIR)/shell.c
SOURCEFILES3 += $(APP3DIR)/telnetd.c
SOURCEFILES4 += $(SOURCEFILES)
SOURCEFILES4 += $(APP4DIR)/smtp.c
SOURCEFILES4 += $(APP4DIR)/smtp-strings.c
SOURCEFILES5 += $(SOURCEFILES)
SOURCEFILES5 += $(APP5DIR)/resolv.c
SOURCEFILES5 += $(APP6DIR)/webclient.c
SOURCEFILES5 += $(APP6DIR)/webclient-strings.c
SOURCEFILES6 += $(SOURCEFILES)
SOURCEFILES6 += $(APP5DIR)/resolv.c
SOURCEFILES6 += $(APP6DIR)/webclient.c
SOURCEFILES6 += $(APP6DIR)/webclient-strings.c

#--------------------------------------------------------------
SDCC_HOME = /home/sdcc/build-mcs51-stm8-standard
CC = $(SDCC_HOME)/bin/sdcc
LD = $(CC)
PACKIHX = $(SDCC_HOME)/bin/packihx
#--------------------------------------------------------------
CFLAGS += -mstm8 --std-c99
CFLAGS += --fomit-frame-pointer
# preprocessed c files:
#CFLAGS += -E
#--------------------------------------------------------------
LDFLAGS += -mstm8 --out-fmt-ihx
#--------------------------------------------------------------
# Libraries
LIBS = -lstm8
LIBDIRS = -L$(SDCC_HOME)/share/sdcc/lib/stm8

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
# $(2) - the source filename(s) with (or without) path
src2obj = $(addprefix $(1)/,$(notdir $(patsubst %.c,%.rel,$(2))))
#--------------------------------------------------------------
#--------------------------------------------------------------
# The function creates an explicit rule based on a template common to all object files
# parameters:
# $(1) - object filename with path
# $(2) - source filename with path
# $(3) - directory name for object files
# $(4) - preprocessor definitions
define makerule
$(1): $(2) | $(3)
	$(CC) $(CFLAGS) $(4) -c $$< -o $$@ $(INCLDIRS) -Wp,-MMD,$$(@:.rel=.d),-MQ,$$@
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
$(1): $(1).hex
# Create directory for object files
$(2):
	mkdir $$@
# Link firmware
$(1).ihx: $(3)
	$(LD) $(LDFLAGS) $(LD_PRE_FLAGS) $$^ -o $$@ $(LIBDIRS) $(LIBS)
# Post-process the hex file for programmers which dislike SDCC output hex format
$(1).hex: $(1).ihx
	$(PACKIHX) $(1).ihx > $(1).hex
endef
#--------------------------------------------------------------
#--------------------------------------------------------------
# The function creates an explicit rule to clean target
# parameters:
# $(1) - directory names for object files
define makerule_clean
.PHONY: clean
clean:
	@rm -f *.map *.lst *.lk *.mem *.cdb *.ihx
	@rm -rf $(1)
endef
#--------------------------------------------------------------
#--------------------------------------------------------------
# Additional functions
get_target_name = $(word $(1),$(TARGETS))
get_object_dir_name = $(call target2objdir,$(call get_target_name,$(1)))
get_object_file_names = $(call src2obj,$(call target2objdir,$(word $(1),$(TARGETS))),$(SOURCEFILES$(1)))
#--------------------------------------------------------------


.PHONY: all
all: $(TARGETS)

CNTLIST = $(shell for x in $$(seq 1 $(words $(TARGETS))); do echo $$x; done)

define makerules
$(eval $(call makerule_target,$(call get_target_name,$(1)),$(call get_object_dir_name,$(1)),$(call get_object_file_names,$(1))))
$(foreach src,$(SOURCEFILES$(1)),$(eval $(call makerule,$(call src2obj,$(call get_object_dir_name,$(1)),$(src)),$(src),$(call get_object_dir_name,$(1)),$(DEF$(1)))))
# Include additional explicit dependencies without recipes from the compiler (*.d files in the object directories)
-include $(call get_object_dir_name,$(1))/*.d
endef

$(foreach cnt,$(CNTLIST),$(eval $(call makerules,$(cnt))))

get_object_dir_names = $(foreach cnt,$(CNTLIST),$(call get_object_dir_name,$(cnt)))
$(eval $(call makerule_clean,$(call get_object_dir_names)))

.PHONY: distclean
distclean: clean
	@rm -f *.hex
