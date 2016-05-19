CFLAGS += -DUSE_STDPERIPH_DRIVER
CPPFLAGS += -std=gnu++11

ifeq ("$(PLATFORM_ID)","103")
ASRC += $(COMMON_BUILD)/arm/startup/startup_$(STM32_DEVICE).S
else ifeq ("$(PLATFORM_ID)","269")
ASRC += $(COMMON_BUILD)/arm/startup/startup_$(STM32_DEVICE).S
else
ASRC += $(COMMON_BUILD)/arm/startup/startup_$(STM32_DEVICE_LC).S
endif

ASFLAGS += -I$(COMMON_BUILD)/arm/startup

# Linker flags

ifeq ("$(PLATFORM_ID)","103")
LDFLAGS += -T$(COMMON_BUILD)/arm/linker/bootloader_nrf51.ld
LDFLAGS += -T$(COMMON_BUILD)/arm/linker/bootloader_nrf51_common.ld
else ifeq ("$(PLATFORM_ID)","269")
LDFLAGS += -T$(COMMON_BUILD)/arm/linker/bootloader_nrf51.ld
LDFLAGS += -T$(COMMON_BUILD)/arm/linker/bootloader_nrf51_common.ld
else
LDFLAGS += -T$(COMMON_BUILD)/arm/linker/linker_$(STM32_DEVICE_LC).ld
endif

LDFLAGS += -Wl,-Map,$(TARGET_BASE).map


ifeq ("$(PLATFORM_ID)","103")
LINKER_DEPS += $(COMMON_BUILD)/arm/linker/bootloader_nrf51.ld
LINKER_DEPS += $(COMMON_BUILD)/arm/linker/bootloader_nrf51_common.ld
else ifeq ("$(PLATFORM_ID)","269")
LINKER_DEPS += $(COMMON_BUILD)/arm/linker/bootloader_nrf51.ld
LINKER_DEPS += $(COMMON_BUILD)/arm/linker/bootloader_nrf51_common.ld
else
LINKER_DEPS += $(COMMON_BUILD)/arm/linker/linker_$(STM32_DEVICE_LC).ld
endif

LINKER_DEPS += $(COMMON_BUILD)/arm/linker/module_start.ld
LINKER_DEPS += $(COMMON_BUILD)/arm/linker/module_end.ld
LINKER_DEPS += $(COMMON_BUILD)/arm/linker/module_info.ld

GLOBAL_DEFINES += BOOTLOADER_VERSION=$(BOOTLOADER_VERSION)
GLOBAL_DEFINES += MODULE_VERSION=$(BOOTLOADER_VERSION)
GLOBAL_DEFINES += MODULE_FUNCTION=$(MODULE_FUNCTION_BOOTLOADER)
GLOBAL_DEFINES += MODULE_DEPENDENCY=0,0,0

# select sources from platform

# import common main.c under bootloader/src
include $(BOOTLOADER_MODULE_PATH)/src/sources.mk

# import the sources from the platform
include $(call rwildcard,$(BOOTLOADER_MODULE_PATH)/src/$(PLATFORM_NAME)/,sources.mk)





