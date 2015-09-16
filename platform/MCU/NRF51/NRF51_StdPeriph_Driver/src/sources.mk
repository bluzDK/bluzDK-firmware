# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_STDPERIPH_SRC_PATH = $(TARGET_STDPERIPH_PATH)/src

# C source files included in this build.
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_button.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_error.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_gpiote.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_scheduler.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_timer.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_uart_fifo.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/app_fifo.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/ble_advdata.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/ble_conn_params.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/ble_debug_assert_handler.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/nrf_delay.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/pstorage.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/simple_uart.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/softdevice_handler.c
CSRC += $(TARGET_STDPERIPH_SRC_PATH)/spi_master.c

# C++ source files included in this build.
CPPSRC +=

# ASM source files included in this build.
ASRC +=

