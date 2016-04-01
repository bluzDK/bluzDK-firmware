# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_STDPERIPH_SRC_PATH = $(TARGET_STDPERIPH_PATH)/src

# C source files included in this build.
# CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_button.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_error.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_scheduler.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_timer.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_timer_appsh.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/bootloader.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/bootloader_settings.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/bootloader_util.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/crc16.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/device_manager_peripheral.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_timer.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_trace.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_uart.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_pwm.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_fifo.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/ble_advdata.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/ble_conn_params.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/ble_debug_assert_handler.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/ble_radio_notification.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_adc.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_delay.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_drv_common.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_drv_gpiote.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_drv_ppi.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_drv_twi.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_drv_timer.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_drv_interrupts.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_drv_wdt.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/pstorage.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/softdevice_handler.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/softdevice_handler_appsh.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/spi_master.c

# C++ source files included in this build.
CPPSRC +=

# ASM source files included in this build.
ASRC +=

