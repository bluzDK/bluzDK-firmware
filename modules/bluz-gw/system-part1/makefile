MODULE=system-part1
SYSTEM_PART1_MODULE_PATH=.
PROJECT_ROOT = ../../..
BUILD_PATH_EXT = $(BUILD_TARGET_PLATFORM)
HAL_LINK :=
PLATFORM_DFU = 0x8060000
MAKE_DEPENDENCIES = communication hal platform system wiring dynalib services
DEPENDENCIES = modules/bluz-gw/user-part $(MAKE_DEPENDENCIES) dynalib system wiring hal platform rt-dynalib
include ../modular.mk
include $(PROJECT_ROOT)/build/platform-id.mk

LIBS += $(MAKE_DEPENDENCIES)
LIB_DEPS += $(COMMUNICATION_LIB_DEP) $(DYNALIB_LIB_DEP) $(SERVICES_LIB_DEP) $(SERVICES_DYNALIB_LIB_DEP) $(HAL_DYNALIB_LIB_DEP) $(PLATFORM_LIB_DEP) $(HAL_LIB_DEP) $(SYSTEM_LIB_DEP) $(WIRING_LIB_DEP)
LIB_DIRS += $(dir $(LIB_DEPS))

TARGET=elf bin lst hex size


include $(PROJECT_ROOT)/build/arm-tlm.mk

$(call check_modular)