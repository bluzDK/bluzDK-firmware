

MEMORY
{
    APP_FLASH (rx) : ORIGIN = 0x00037000, LENGTH = 0x05000
    RAM (rwx)  : ORIGIN = 0x20007c00, LENGTH = 0x400
}

INCLUDE ../system-part1/module_system_part1_export.ld
INCLUDE module_user_export.ld
INCLUDE user.ld
