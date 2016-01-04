

MEMORY
{
    APP_FLASH (rx) : ORIGIN = 0x00035000, LENGTH = 0x07000
    RAM (rwx)  : ORIGIN = 0x20007000, LENGTH = 0x1000
}

INCLUDE ../system-part1/module_system_part1_export.ld
INCLUDE module_user_export.ld
INCLUDE user.ld
