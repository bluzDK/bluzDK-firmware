#include "module_system_part1.h"
#include "system_part1_loader.c"

DYNALIB_TABLE_EXTERN(communication);
DYNALIB_TABLE_EXTERN(services);
DYNALIB_TABLE_EXTERN(system_module_part1);
DYNALIB_TABLE_EXTERN(hal);
DYNALIB_TABLE_EXTERN(rt);
DYNALIB_TABLE_EXTERN(system);
DYNALIB_TABLE_EXTERN(hal_peripherals);
DYNALIB_TABLE_EXTERN(hal_i2c);
DYNALIB_TABLE_EXTERN(hal_gpio);
DYNALIB_TABLE_EXTERN(hal_spi);
DYNALIB_TABLE_EXTERN(hal_core);
DYNALIB_TABLE_EXTERN(hal_socket);
DYNALIB_TABLE_EXTERN(hal_wlan);
DYNALIB_TABLE_EXTERN(hal_usart);
DYNALIB_TABLE_EXTERN(system_net);
DYNALIB_TABLE_EXTERN(system_cloud);
DYNALIB_TABLE_EXTERN(hal_concurrent);

__attribute__((externally_visible)) const void* const system_part1_module[] = {
    DYNALIB_TABLE_NAME(communication),
    DYNALIB_TABLE_NAME(services),
    DYNALIB_TABLE_NAME(system_module_part1),
    DYNALIB_TABLE_NAME(hal),
    DYNALIB_TABLE_NAME(rt),
    DYNALIB_TABLE_NAME(system),
    DYNALIB_TABLE_NAME(hal_peripherals),
    DYNALIB_TABLE_NAME(hal_i2c),
    DYNALIB_TABLE_NAME(hal_gpio),
    DYNALIB_TABLE_NAME(hal_spi),
    DYNALIB_TABLE_NAME(hal_core),
    DYNALIB_TABLE_NAME(hal_socket),
    DYNALIB_TABLE_NAME(hal_wlan),
    DYNALIB_TABLE_NAME(hal_usart),
    DYNALIB_TABLE_NAME(system_net),
    DYNALIB_TABLE_NAME(system_cloud),
    DYNALIB_TABLE_NAME(hal_concurrent),
};

