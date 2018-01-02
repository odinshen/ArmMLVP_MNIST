# Copyright (C) ARM Limited, 2014-2016. All rights reserved.
#
# This example is intended to be built with either:
# * ARM Compiler 6
# * Linaro bare-metal GCC
#
# Environment variables for build options that the user might wish to change
#
# Variable     Example Value
# ----------   -------------
# APP          myapp.axf
# QUIET        @ for terse output, or leave blank for detailed output
# OPT_LEVEL    0, 1, 2 or 3
# DEFINES      -D MYDEFINE
# PLATFORM     CORTEXA (adds extra code for initialising Cortex-A35/A53/A57/A72/A73), or AEM

include host.mk

APP ?= ArmMLVP_MNIST.axf
QUIET ?= @
OPT_LEVEL ?= 3
PLATFORM ?= AEM
# PLATFORM is used to determine core number
# and CORTEX_A73x2_CORTEX_A53x4 is compatible
# with any x1, x2 or x2-x{1,2,3,4} cluster configuration

# Other switches the user should not normally need to change:
ARCH = armv8.2-a
DEBUG_FLAGS = -g


ifneq ($(findstring CORTEX,$(PLATFORM)),)
	CPPFLAGS_EXTRA += -D CORTEXA
endif

CPPFLAGS_EXTRA += -D $(PLATFORM)

ifeq ($(QUIET),@)
PROGRESS = @echo Compiling $<...
endif


CC = armclang
#CC = aarch64-elf-gcc

SRC_DIR = src
ASM_DIR = asm
OBJ_DIR = obj

INCLUDES = -I$(SRC_DIR)

DEPEND_FLAGS = -MD -MF $@.d
NEON_FLAGS = -MFPU=neon-fp-armv8
NO_NEON_FLAGS = -MFPU=none
CPPFLAGS = $(DEFINES) $(INCLUDES) $(DEPEND_FLAGS) $(CPPFLAGS_EXTRA) $(NEON_FLAGS)
#CPPFLAGS = $(DEFINES) $(INCLUDES) $(DEPEND_FLAGS) $(CPPFLAGS_EXTRA) $(NO_NEON_FLAGS)
CFLAGS = $(DEBUG_FLAGS) -O$(OPT_LEVEL)
ASFLAGS = $(DEBUG_FLAGS)
ifeq ($(CC),armclang)
LAYOUT = layout.scat
LDFLAGS = -Wl,--scatter=$(LAYOUT) -e start64
TARGET_ARCH = --target=aarch64-arm-none-eabi
else
LAYOUT = layout.ld
LDFLAGS = --specs=rdimon.specs -T$(LAYOUT) -Wl,--build-id=none,-Map=linkmap.txt
endif
TARGET_ARCH += -march=$(ARCH)

APP_C_SRC := $(wildcard $(SRC_DIR)/*.c)
APP_S_SRC := $(wildcard $(ASM_DIR)/*.S)
OBJ_FILES := $(APP_C_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) \
             $(APP_S_SRC:$(ASM_DIR)/%.S=$(OBJ_DIR)/%.o)
DEP_FILES := $(OBJ_FILES:%=%.d)

.phony: all clean

all: $(APP)

$(APP): $(OBJ_FILES) $(LAYOUT)
	@echo Linking $@
	$(QUIET) $(CC) $(TARGET_ARCH) $(LDFLAGS) --output $@ $(OBJ_FILES)
	@echo Done.

clean:
	$(call RM_DIRS,$(OBJ_DIR))
	$(call RM_FILES,$(APP) linkmap.txt)

$(OBJ_DIR):
	mkdir $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(PROGRESS)
	$(QUIET) $(CC) -c $(TARGET_ARCH) $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o : $(ASM_DIR)/%.S | $(OBJ_DIR)
	$(PROGRESS)
	$(QUIET) $(CC) -c $(TARGET_ARCH) $(CPPFLAGS) $(ASFLAGS) -o $@ $<

# Make sure everything is rebuilt if this makefile is changed
$(OBJ_FILES) $(APP): makefile

-include $(DEP_FILES)

help:
	@echo make [OPTIONS]
	@echo 'PLATFORM=   [AEM/CORTEXA]       Choose FVP target: AEMv8 or Cortex-A35/A53/A57/A72/A73'
	@echo ''
	@echo 'NOTE: The first value in the options indicates the default setting.'
