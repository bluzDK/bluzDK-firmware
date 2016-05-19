PROJECT_NAME := dfu_dual_bank_ble_s110_pca10028

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

TEMPLATE_PATH = ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/toolchain/gcc
ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc"
CPP       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-g++"
AS       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as"
AR       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar" -r
LD       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld"
NM       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm"
OBJDUMP  		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump"
OBJCOPY  		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy"
SIZE    		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size"

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES += \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/app_error.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/app_scheduler.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/app_timer.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/app_timer_appsh.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/app_fifo.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/app_uart.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/bootloader.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/bootloader_settings.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/bootloader_util.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/crc16.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/dfu_dual_bank.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/dfu_init_template.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/dfu_transport_ble.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/hci_mem_pool.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/nrf_assert.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/nrf_delay.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/nrf_drv_gpiote.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/nrf_drv_common.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/pstorage.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/spi_master.c) \
$(abspath ../../../platform/MCU/NRF51/SPARK_Firmware_Driver/src/sst25vf_spi.c) \
$(abspath ../../../platform/MCU/NRF51/SPARK_Firmware_Driver/src/flash.c) \
$(abspath ../../../platform/MCU/NRF51/SPARK_Firmware_Driver/src/nrf51_callbacks.c) \
$(abspath dfu_ble_svc.c) \
$(abspath main.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/ble_advdata.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/ble_conn_params.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/ble_dfu.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/ble_srv_common.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/system_nrf51.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/softdevice_handler.c) \
$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/src/softdevice_handler_appsh.c) \

CPP_SOURCE_FILES += \
$(abspath ../../../platform/MCU/NRF51/SPARK_Firmware_Driver/src/rgbled_hal.cpp)

#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath ./gcc_startup_nrf51.s)

#includes common to all targets
INC_PATHS  = -I$(abspath .)
INC_PATHS  = -I$(abspath ./config)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/libraries/util)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/libraries/timer)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/libraries/fifo)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/toolchain)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/libraries/bootloader_dfu)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/libraries/scheduler)

ifeq ("$(PLATFORM_ID)","103")
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/softdevice/s110/headers)
else ifeq ("$(PLATFORM_ID)","269")
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/softdevice/s120/headers)
endif


INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/bsp)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/drivers_nrf/pstorage)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/drivers_nrf/common)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/toolchain/gcc)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/libraries/bootloader_dfu/ble_transport)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/device)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/libraries/hci)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/softdevice/common/softdevice_handler)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/libraries/crc16)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/ble/ble_services/ble_dfu)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/ble/device_manager)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/ble/device_manager/config)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/drivers_nrf/hal)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/drivers_nrf/uart)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/drivers_nrf/spi_master)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/drivers_nrf/gpiote)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/drivers_nrf/timer)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/NRF51_StdPeriph_Driver/inc/ble/common)
INC_PATHS += -I$(abspath ../../../platform/MCU/NRF51/SPARK_Firmware_Driver/inc)
INC_PATHS += -I$(abspath ../../../hal/shared)
INC_PATHS += -I$(abspath ../../../hal/inc)
INC_PATHS += -I$(abspath ../../../dynalib/inc)
INC_PATHS += -I$(abspath ../../../services/inc)

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

#flags common to all targets
CPPFLAGS  = -DSWI_DISABLE0
CPPFLAGS += -DBOARD_PCA10028
CPPFLAGS += -DSOFTDEVICE_PRESENT
CPPFLAGS += -DNRF51

ifeq ("$(PLATFORM_ID)","103")
CPPFLAGS += -DS110
else ifeq ("$(PLATFORM_ID)","269")
CPPFLAGS += -DS120
endif

CPPFLAGS += -D__HEAP_SIZE=0
CPPFLAGS += -DBLE_STACK_SUPPORT_REQD
CPPFLAGS += -DBSP_DEFINES_ONLY
CPPFLAGS += -mcpu=cortex-m0
CPPFLAGS += -mthumb -mabi=aapcs
CPPFLAGS += -Wall -Werror -Os
CPPFLAGS += -mfloat-abi=soft
# keep every function in separate section. This will allow linker to dump unused functions
CPPFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CPPFLAGS += -fno-builtin --short-enums

CFLAGS = $(CPPFLAGS)
CFLAGS += --std=gnu99

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -DBOARD_PCA10028
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DNRF51

ifeq ("$(PLATFORM_ID)","103")
ASMFLAGS += -DS110
else ifeq ("$(PLATFORM_ID)","269")
ASMFLAGS += -DS120
endif

ASMFLAGS += -D__HEAP_SIZE=0
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DBSP_DEFINES_ONLY
#default target - first one defined
default: clean bluz_bootloader

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e bluz_bootloader

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	bluz_bootloader


C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

CPP_SOURCE_FILE_NAMES = $(notdir $(CPP_SOURCE_FILES))
CPP_PATHS = $(call remduplicates, $(dir $(CPP_SOURCE_FILES) ) )
CPP_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(CPP_SOURCE_FILE_NAMES:.cpp=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.cpp $(CPP_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(CPP_OBJECTS) $(ASM_OBJECTS)

bluz_bootloader: OUTPUT_FILENAME := bluz_bootloader
bluz_bootloader: LINKER_SCRIPT=./dfu_gcc_nrf51.ld
bluz_bootloader: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CPP) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling C file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Create objects from CPP SRC files
$(OBJECT_DIRECTORY)/%.o: %.cpp
	@echo Compiling CPP file: $(notdir $<)
	$(NO_ECHO)$(CPP) $(CPPFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<


# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out


## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

echosize:
	-@echo ""
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ""

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash: $(MAKECMDGOALS)
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	nrfjprog --reset --program $(OUTPUT_BINARY_DIRECTORY)/$<.hex

## Flash softdevice