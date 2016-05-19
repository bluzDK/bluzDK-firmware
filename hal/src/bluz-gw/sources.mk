HAL_SRC_SHARED_PATH = $(TARGET_HAL_PATH)/src/nrf51

CSRC += $(call target_files,$(HAL_SRC_BLUZ_GW_PATH)/,*.c)
CPPSRC += $(call target_files,$(HAL_SRC_BLUZ_GW_PATH)/,*.cpp)

CSRC += $(call target_files,$(HAL_SRC_SHARED_PATH)/,*.c)
CPPSRC += $(call target_files,$(HAL_SRC_SHARED_PATH)/,*.cpp)

# ASM source files included in this build.
ASRC +=



