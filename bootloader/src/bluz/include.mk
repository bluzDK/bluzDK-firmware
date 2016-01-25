INCLUDE_DIRS += $(BOOTLOADER_MODULE_PATH)/src/bluz
INCLUDE_DIRS += $(BOOTLOADER_MODULE_PATH)/src/bluz/config
INCLUDE_DIRS += $(PLATFORM_MODULE_PATH)/MCU/NRF51/NRF51_StdPeriph_Driver/inc/libraries


LDFLAGS += --specs=nano.specs -lc -lnosys
