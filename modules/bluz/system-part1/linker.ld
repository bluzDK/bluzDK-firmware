MEMORY
{
    APP_FLASH (rx) : ORIGIN = 0x00018000, LENGTH = 0x1b000
    RAM (rwx)  : ORIGIN = 0x20002000, LENGTH = 0x5000
}

INCLUDE module_system_part1_export.ld
INCLUDE module_user_export.ld

INCLUDE part1.ld
