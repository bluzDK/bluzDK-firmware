MEMORY
{
    APP_FLASH (rx) : ORIGIN = 0x0001d000, LENGTH = 0x1a000
    RAM (rwx)  : ORIGIN = 0x20002800, LENGTH = 0x5400
}

INCLUDE module_system_part1_export.ld
INCLUDE module_user_export.ld

INCLUDE part1.ld
