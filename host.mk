# Copyright (C) ARM Limited, 2016. All rights reserved.

define EOL =


endef

ifeq ($(OS),Windows_NT)
RM_FILES = $(foreach file,$(1),if exist $(file) del /q $(file)$(EOL))
RM_DIRS = $(foreach dir,$(1),if exist $(dir) rmdir /s /q $(dir)$(EOL))
else
RM_FILES = $(foreach file,$(1),rm -f $(file)$(EOL))
RM_DIRS = $(foreach dir,$(1),rm -rf $(dir)$(EOL))
endif

