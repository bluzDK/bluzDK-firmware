
/**
 * The end of the statically allocated region.
 * This is much more than we need but ensures the RAM vtor table in part2 is
 * 512-byte aligned.
 */
system_part1_module_ram_end = 0x20007000;
system_part1_start = 0x00018000;

system_part1_vtor_table_size = 0xc0;
system_part1_module_info_size = 24;
system_part1_vtor_table_start = system_part1_start;

system_part1_module_info_start = system_part1_start + system_part1_vtor_table_size;

system_part1_module_info_end = system_part1_module_info_start + system_part1_module_info_size;

system_part1_module_table = system_part1_module_info_end;


/**
 * The dyanmic libraries exported by this module.
 */
PROVIDE ( dynalib_location_communication = system_part1_module_table + 0 );
PROVIDE ( dynalib_location_services = system_part1_module_table + 4 );
PROVIDE ( dynalib_location_hal = system_part1_module_table + 8 );
PROVIDE ( dynalib_location_rt = system_part1_module_table + 12 );
PROVIDE ( dynalib_location_system = system_part1_module_table + 16 );
PROVIDE ( dynalib_location_hal_peripherals = system_part1_module_table + 20 );
PROVIDE ( dynalib_location_hal_i2c = system_part1_module_table + 24 );
PROVIDE ( dynalib_location_hal_gpio = system_part1_module_table + 28 );
PROVIDE ( dynalib_location_hal_spi = system_part1_module_table + 32 );
PROVIDE ( dynalib_location_hal_core = system_part1_module_table + 36 );
PROVIDE ( dynalib_location_hal_socket = system_part1_module_table + 40 );
PROVIDE ( dynalib_location_hal_usart = system_part1_module_table + 44 );
PROVIDE ( dynalib_location_system_net = system_part1_module_table + 48 );
PROVIDE ( dynalib_location_system_cloud = system_part1_module_table + 52 );
PROVIDE ( dynalib_location_hal_concurrent = system_part1_module_table + 56 );
PROVIDE ( dynalib_location_hal_ble = system_part1_module_table + 60 );
PROVIDE ( dynalib_location_hal_apptimer = system_part1_module_table + 64 );

