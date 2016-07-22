USER_PART_MODULE_VERSION ?= 8
USER_PART_MODULE_PATH ?= $(PROJECT_ROOT)/modules/bluz-gw/user-part
include $(call rwildcard,$(USER_PART_MODULE_PATH)/,include.mk)


