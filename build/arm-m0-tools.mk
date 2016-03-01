#
# Define ARM tools
#


# Define the compiler/tools prefix
GCC_PREFIX ?= arm-none-eabi-

include $(COMMON_BUILD)/common-tools.mk


#
# default flags for targeting ARM
#

# C compiler flags
CFLAGS +=  -g3 -Os -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -mabi=aapcs -DSOFTDEVICE_PRESENT -DBLE_STACK_SUPPORT_REQUIRED -DNRF51 -DNRF51822_QFAA_CA -DBLE_STACK_SUPPORT_REQD -DUSE_CUSTOM_STATIC_ASSERT -DBLUZ

#bluz requires s110
ifeq ("$(PLATFORM_ID)","269")
CFLAGS += -DS110_SUPPORT_REQUIRED
endif

#bluz-gw requires s120
ifeq ("$(PLATFORM_ID)","269")
CFLAGS += -DS120_SUPPORT_REQUIRED
endif

# C++ specific flags
CPPFLAGS += -fno-exceptions -fno-rtti

CONLYFLAGS += -std=gnu99

ASFLAGS +=  -g3 -x assembler-with-cpp -Os -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -mabi=aapcs -DSOFTDEVICE_PRESENT -DBLE_STACK_SUPPORT_REQUIRED -DNRF51 -DNRF51822_QFAA_CA -DBLE_STACK_SUPPORT_REQD -DUSE_CUSTOM_STATIC_ASSERT -DBLUZ

#bluz requires s110
ifeq ("$(PLATFORM_ID)","269")
ASFLAGS += -DS110_SUPPORT_REQUIRED
endif

#bluz-gw requires s120
ifeq ("$(PLATFORM_ID)","269")
ASFLAGS += -DS120_SUPPORT_REQUIRED
endif

LDFLAGS += -Xlinker --gc-sections -lc -lnosys

ifeq ($(COMPILE_LTO),y)
CFLAGS += -flto
LDFLAGS += -flto -Os -fuse-linker-plugin
endif

