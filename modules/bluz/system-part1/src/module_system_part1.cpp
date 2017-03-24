#include "module_system_part1.h"
#include "dynalib.h"
#include "system_mode.h"
#include "module_user_init.h"
#include "core_hal.h"
#include <stdint.h>
#include <stddef.h>

extern "C" {

DYNALIB_TABLE_EXTERN(communication);
DYNALIB_TABLE_EXTERN(services);
DYNALIB_TABLE_EXTERN(hal);
DYNALIB_TABLE_EXTERN(rt);
DYNALIB_TABLE_EXTERN(system);
DYNALIB_TABLE_EXTERN(hal_peripherals);
DYNALIB_TABLE_EXTERN(hal_i2c);
DYNALIB_TABLE_EXTERN(hal_gpio);
DYNALIB_TABLE_EXTERN(hal_spi);
DYNALIB_TABLE_EXTERN(hal_core);
DYNALIB_TABLE_EXTERN(hal_socket);
DYNALIB_TABLE_EXTERN(hal_usart);
DYNALIB_TABLE_EXTERN(system_net);
DYNALIB_TABLE_EXTERN(system_cloud);
DYNALIB_TABLE_EXTERN(hal_concurrent);
DYNALIB_TABLE_EXTERN(hal_ble);
DYNALIB_TABLE_EXTERN(hal_apptimer);
DYNALIB_TABLE_EXTERN(hal_ble_stream);

extern "C" __attribute__((externally_visible)) const void* const system_part1_module[] = {
    DYNALIB_TABLE_NAME(communication),
    DYNALIB_TABLE_NAME(services),
    DYNALIB_TABLE_NAME(hal),
    DYNALIB_TABLE_NAME(rt),
    DYNALIB_TABLE_NAME(system),
    DYNALIB_TABLE_NAME(hal_peripherals),
    DYNALIB_TABLE_NAME(hal_i2c),
    DYNALIB_TABLE_NAME(hal_gpio),
    DYNALIB_TABLE_NAME(hal_spi),
    DYNALIB_TABLE_NAME(hal_core),
    DYNALIB_TABLE_NAME(hal_socket),
    DYNALIB_TABLE_NAME(hal_usart),
    DYNALIB_TABLE_NAME(system_net),
    DYNALIB_TABLE_NAME(system_cloud),
    DYNALIB_TABLE_NAME(hal_concurrent),
    DYNALIB_TABLE_NAME(hal_ble),
    DYNALIB_TABLE_NAME(hal_apptimer),
    DYNALIB_TABLE_NAME(hal_ble_stream),
};

#include "system_part1_loader.c"

}
