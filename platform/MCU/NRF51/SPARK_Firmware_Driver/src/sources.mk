# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_SPARK_SRC_PATH = $(TARGET_SPARK_PATH)/src

INCLUDE_DIRS += SPARK_Services/inc

# C source files included in this build.
CSRC += $(TARGET_SPARK_SRC_PATH)/hw_config.c
CSRC += $(TARGET_SPARK_SRC_PATH)/flash.c
CSRC += $(TARGET_SPARK_SRC_PATH)/sst25vf_spi.c
CSRC += $(TARGET_SPARK_SRC_PATH)/system_nrf51.c
CSRC += $(TARGET_SPARK_SRC_PATH)/nrf51_callbacks.c
CSRC += $(TARGET_SPARK_SRC_PATH)/spi_master_fast.c
CSRC += $(TARGET_SPARK_SRC_PATH)/beacon_helper.c
CSRC += $(TARGET_SPARK_SRC_PATH)/ble_stream.c

ifeq ("$(PLATFORM_ID)","103")
CSRC += $(TARGET_SPARK_SRC_PATH)/ble_scs.c
CSRC += $(TARGET_SPARK_SRC_PATH)/particle_data_service.c
endif

ifeq ("$(PLATFORM_ID)","102")
CSRC += $(TARGET_SPARK_SRC_PATH)/ble_scs.c
CSRC += $(TARGET_SPARK_SRC_PATH)/particle_data_service.c
endif

ifeq ("$(PLATFORM_ID)","269")
CSRC += $(TARGET_SPARK_SRC_PATH)/hw_gateway_config.c
CSRC += $(TARGET_SPARK_SRC_PATH)/spi_slave_stream.c
CSRC += $(TARGET_SPARK_SRC_PATH)/client_handling.c
endif


# C++ source files included in this build.
CPPSRC += $(TARGET_SPARK_SRC_PATH)/socket.cpp
CPPSRC += $(TARGET_SPARK_SRC_PATH)/info_data_service.cpp
CPPSRC += $(TARGET_SPARK_SRC_PATH)/socket_manager.cpp
CPPSRC += $(TARGET_SPARK_SRC_PATH)/data_management_layer.cpp
CPPSRC += $(TARGET_SPARK_SRC_PATH)/data_service.cpp
CPPSRC += $(TARGET_SPARK_SRC_PATH)/custom_data_service.cpp
CPPSRC += $(TARGET_SPARK_SRC_PATH)/data_services.cpp
CPPSRC += $(TARGET_SPARK_SRC_PATH)/rgbled_hal.cpp
# ASM source files included in this build.
ASRC +=


