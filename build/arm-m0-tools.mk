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
CFLAGS +=  -g3 -gdwarf-2 -Os -mcpu=cortex-m0 -mthumb -DNRF51

# C++ specific flags
CPPFLAGS += -fno-exceptions -fno-rtti

CONLYFLAGS += -std=gnu99

ASFLAGS +=  -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb 

LDFLAGS += -Xlinker --gc-sections


