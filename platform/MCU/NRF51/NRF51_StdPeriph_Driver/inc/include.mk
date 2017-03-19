TARGET_STDPERIPH_PATH = $(PLATFORM_MCU_PATH)/NRF51_StdPeriph_Driver

INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/device
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/common
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/config
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/gpiote
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/hal
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/ppi
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/simple_uart
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/spi_master
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/spi_slave
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/ble_flash
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/pstorage
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/pstorage/config
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/timer
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/twi_master
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/uart
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/drivers_nrf/wdt
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/softdevice/common/softdevice_handler
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/ble/common
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/ble/ble_db_discovery
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/ble/device_manager
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/ble/ble_radio_notification
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/ble/device_manager/config
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/ble/ble_error_log
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/ble/ble_debug_assert_handler
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/bootloader_dfu
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/crc16
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/timer
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/pwm
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/scheduler
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/gpiote
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/fifo
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/button
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/trace
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/util
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/libraries/timer
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/toolchain
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/toolchain/gcc

ifeq ("$(PLATFORM_ID)","103")
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/softdevice/s110/headers
endif

ifeq ("$(PLATFORM_ID)","102")
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/softdevice/s110/headers
endif

ifeq ("$(PLATFORM_ID)","269")
INCLUDE_DIRS += $(TARGET_STDPERIPH_PATH)/inc/softdevice/s120/headers
endif


