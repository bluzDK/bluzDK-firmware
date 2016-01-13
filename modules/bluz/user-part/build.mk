ifeq (,$(SYSTEM_PART1_MODULE_VERSION))
$(error SYSTEM_PART1_MODULE_VERSION not defined)
endif

# TARGET_FILE sets the file stem of the target. It's typically only specified when building applications.
ifdef TARGET_FILE
TARGET_FILE_NAME = $(TARGET_FILE)
endif

# TARGET_DIR is the externally specified target directory
ifdef TARGET_DIR
TARGET_PATH = $(TARGET_DIR)
endif

ifdef APP
# when TARGET_FILE is defined on the command line,
TARGET_FILE_NAME ?= $(notdir $(APP))
TARGET_DIR_NAME ?= applications/$(APP)
endif

ifdef APPDIR
# APPDIR is where the sources are found
# if TARGET_DIR is not defined defaults to $(APPDIR)/target
# if TARGET_FILE_NAME is not defined, defaults to the name of the $(APPDIR)
TARGET_FILE_NAME ?= $(notdir $(APPDIR))
TARGET_DIR_NAME ?= $(APPDIR)/target
# do not use $(BUILD_PATH) since the TARGET_DIR specifies fully where the output should go
ifdef TARGET_DIR
TARGET_PATH = $(TARGET_DIR)/
else
TARGET_PATH = $(TARGET_DIR_NAME)/
endif
BUILD_PATH = $(TARGET_PATH)/obj
endif

ifdef TEST
TARGET_FILE_NAME ?= $(notdir $(TEST))
TARGET_DIR_NAME ?= test/$(TEST)
endif

# to allow _malloc_r to be overridden we have to remove it from the libg_nano.a library
# this symbol is the target for the library
LIBG_TWEAK = $(TARGET_BASE_DIR)libg_tweak.a

GLOBAL_DEFINES += MODULE_VERSION=$(USER_PART_MODULE_VERSION)
GLOBAL_DEFINES += MODULE_FUNCTION=$(MODULE_FUNCTION_USER_PART)
GLOBAL_DEFINES += MODULE_INDEX=2
GLOBAL_DEFINES += MODULE_DEPENDENCY=${MODULE_FUNCTION_SYSTEM_PART},1,${SYSTEM_PART1_MODULE_VERSION}

LINKER_FILE=$(USER_PART_MODULE_PATH)/linker.ld
LINKER_DEPS += $(LINKER_FILE)
LINKER_DEPS += $(SYSTEM_PART1_MODULE_PATH)/module_system_part1_export.ld

LDFLAGS += --specs=nano.specs -lnosys
LDFLAGS += -T$(LINKER_FILE)
LDFLAGS += -L$(USER_PART_MODULE_PATH)
LDFLAGS += -Wl,-Map,$(TARGET_BASE).map

USER_PART_MODULE_SRC_PATH = $(USER_PART_MODULE_PATH)/src

CPPSRC += $(call target_files,$(USER_PART_MODULE_SRC_PATH),*.cpp)
CSRC += $(call target_files,$(USER_PART_MODULE_SRC_PATH),*.c)

BUILTINS_EXCLUDE = malloc free realloc
CFLAGS += $(addprefix -fno-builtin-,$(BUILTINS_EXCLUDE))
