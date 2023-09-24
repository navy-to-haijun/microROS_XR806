#
# Rules for building application
#

# ----------------------------------------------------------------------------
# common rules
# ----------------------------------------------------------------------------
ROOT_PATH := ../../../..

include $(ROOT_PATH)/gcc.mk

# ----------------------------------------------------------------------------
# project name and project root path
# ----------------------------------------------------------------------------
PROJECT := $(notdir $(shell cd .. && pwd))
PRJ_PARENT_DIR := $(notdir $(shell cd ../.. && pwd))
PRJ_ROOT_PATH := $(ROOT_PATH)/project/$(PRJ_PARENT_DIR)/$(PROJECT)

# ----------------------------------------------------------------------------
# objects
# ----------------------------------------------------------------------------
INCLUDE_PATHS += -I$(PRJ_ROOT_PATH)

DIRS_IGNORE := $(PRJ_ROOT_PATH)/gcc%
DIRS_IGNORE += $(PRJ_ROOT_PATH)/image%
DIRS_IGNORE += $(ROOT_PATH)/project/common/board/%
DIRS_IGNORE += $(ROOT_PATH)/project/common/board/$(shell echo $(CONFIG_BOARD))
# 忽视文件夹中的内容：不参与编译
DIRS_IGNORE +=  $(PRJ_ROOT_PATH)/microros%


DIRS_ALL := $(shell find $(PRJ_ROOT_PATH) $(ROOT_PATH)/project/common -type d)
DIRS := $(filter-out $(DIRS_IGNORE),$(DIRS_ALL))
DIRS += $(ROOT_PATH)/project/common/board/$(shell echo $(CONFIG_BOARD))

SRCS := $(basename $(foreach dir,$(DIRS),$(wildcard $(dir)/*.[csS])))

OBJS := $(addsuffix .o,$(SRCS))


# extra libraries searching path
PRJ_EXTRA_LIBS_PATH := -L$(PRJ_ROOT_PATH)/microros

# extra libraries
PRJ_EXTRA_LIBS := -lmicroros

# extra header files searching path
PRJ_EXTRA_INC_PATH := -I$(PRJ_ROOT_PATH)/microros/include

# extra symbols (macros)
# PRJ_EXTRA_SYMBOLS :=

# ----------------------------------------------------------------------------
# override project variables
# ----------------------------------------------------------------------------
# linker script file
#   - relative to "./"
#   - define your own "LINKER_SCRIPT" to override the default one
# LINKER_SCRIPT :=

# image config file
#   - relative to "../image/$(CONFIG_CHIP_TYPE)/", eg. "../image/xr806/"
#   - define your own "IMAGE_CFG" to override the default one
# IMAGE_CFG :=

# image name, default to xr_system
IMAGE_NAME := microros_system

# project make rules
include $(PRJ_MAKE_RULES)
