
# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
HAL_SRC_BLUZ_PATH = $(TARGET_HAL_PATH)/src/bluz
HAL_SRC_NRF51_SHARED_PATH = $(TARGET_HAL_PATH)/src/nrf51

# if we are being compiled with platform as a dependency, then also include
# implementation headers.
ifneq (,$(findstring platform,$(DEPENDENCIES)))
INCLUDE_DIRS += $(HAL_SRC_BLUZ_PATH)
INCLUDE_DIRS += $(HAL_SRC_NRF51_SHARED_PATH)
endif

HAL_LINK ?= $(findstring hal,$(MAKE_DEPENDENCIES))

# if hal is used as a make dependency (linked) then add linker commands
ifneq (,$(HAL_LINK))

LDFLAGS += --specs=nano.specs -lc -lnosys
LDFLAGS += -T$(COMMON_BUILD)/arm/linker/linker_nrf51_mem.ld
LDFLAGS += -T$(COMMON_BUILD)/arm/linker/linker_nrf51_sections.ld
LDFLAGS += -L$(COMMON_BUILD)/arm/linker
# support for external linker file
LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

ASRC += $(COMMON_BUILD)/arm/startup/startup_$(PLATFORM_MCU).S
# ASFLAGS +=  -Wa,--defsym -Wa,SPARK_INIT_STARTUP=1

endif
