# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_SPARK_SRC_PATH = $(TARGET_SPARK_PATH)/src

INCLUDE_DIRS += SPARK_Services/inc

# C source files included in this build.
CSRC += $(TARGET_SPARK_SRC_PATH)/hw_config.c
CSRC += $(TARGET_SPARK_SRC_PATH)/ble_scs.c
# CSRC += $(TARGET_SPARK_SRC_PATH)/sst25vf_spi.c
CSRC += $(TARGET_SPARK_SRC_PATH)/system_nrf51.c
CSRC += $(TARGET_SPARK_SRC_PATH)/nrf51_callbacks.c
#ifdef BLUZ

#endif


# C++ source files included in this build.
CPPSRC += $(TARGET_SPARK_SRC_PATH)/data_management_layer.cpp
# ASM source files included in this build.
ASRC +=


