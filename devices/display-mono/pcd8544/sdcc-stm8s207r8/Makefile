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
# stm8s207r8 pcd8544 example
#--------------------------------------------------------------

#--------------------------------------------------------------
# Target definitions
TARGETS = pcd8544
DEF1 = -DSTM8S207

#--------------------------------------------------------------
# Paths
MAINDIR = ../../src
CPUDIR = ../../../../cpu/stm8s207r8
PLATFORMHDIR = ../../../../platform
PLATFORMCDIR = ../../../../platform/stm8s207r8
DEVDIR = ../src/dev
HALDIR = ../src/platform/stm8s207r8/dev

#--------------------------------------------------------------
# Include files directories
INCLDIRS += -I$(DEVDIR)
INCLDIRS += -I$(MAINDIR)
INCLDIRS += -I$(CPUDIR)
INCLDIRS += -I$(PLATFORMHDIR)

#--------------------------------------------------------------
# Each source file must be added to the SOURCEFILES list
MAINSOURCEFILE = $(MAINDIR)/main.c
SOURCEFILES1 += $(MAINSOURCEFILE)
SOURCEFILES1 += $(MAINDIR)/fonts.c
SOURCEFILES1 += $(MAINDIR)/mono-scr.c
SOURCEFILES1 += $(MAINDIR)/mono-scr-pcd8544-drv.c
SOURCEFILES1 += $(DEVDIR)/pcd8544.c
SOURCEFILES1 += $(HALDIR)/hal-pcd8544.c
SOURCEFILES1 += $(PLATFORMCDIR)/platform.c

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
