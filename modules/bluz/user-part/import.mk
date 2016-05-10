USER_PART_MODULE_VERSION ?= 5
USER_PART_MODULE_PATH ?= $(PROJECT_ROOT)/modules/bluz/user-part
include $(call rwildcard,$(USER_PART_MODULE_PATH)/,include.mk)


