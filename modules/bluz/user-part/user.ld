/* Linker script for Nordic Semiconductor nRF5 devices
*
* Version: Sourcery G++ 4.5-1
* Support: https://support.codesourcery.com/GNUToolchain/
*
* Copyright (c) 2007, 2008, 2009, 2010 CodeSourcery, Inc.
*
* The authors hereby grant permission to use, copy, modify, distribute,
* and license this software and its documentation for any purpose, provided
* that existing copyright notices are retained in all copies and that this
* notice is included verbatim in any distributions.  No written agreement,
* license, or royalty fee is required for any of the authorized uses.
* Modifications to this software may be copyrighted by their authors
* and need not follow the licensing terms described here, provided that
* the new terms are clearly indicated on the first page of each file where
* they apply.
*/
OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

/* Linker script to place sections and symbol values. Should be used together
* with other linker script that defines memory regions APP_FLASH and RAM.
* It references following symbols, which must be defined in code:
*   Reset_Handler : Entry of reset handler
*
* It defines following symbols, which code can use without definition:
*   __exidx_start
*   __exidx_end
*   __etext
*   __data_start__
*   __preinit_array_start
*   __preinit_array_end
*   __init_array_start
*   __init_array_end
*   __fini_array_start
*   __fini_array_end
*   __data_end__
*   __bss_start__
*   __bss_end__
*   __end__
*   end
*   __HeapLimit
*   __StackLimit
*   __StackTop
*   __stack
*/

SECTIONS
{
    INCLUDE module_start.ld

    INCLUDE module_info.ld

    .text :
    {
        expected_dynalib_start = ORIGIN (APP_FLASH) + 24 ;

        link_dynalib_start = .;
        KEEP(*(*.user_part_module))
        link_dynalib_end = . ;

        link_length_start = .;
        KEEP(*(*.user_part_length))
        link_length_end = .;

        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        link_constructors_location = .;

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        link_constructors_end = .;

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)

        *(.eh_frame*)
    } > APP_FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > APP_FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > APP_FLASH

    __exidx_end = .;

    __etext = .;

    .data :
    {
        __data_start__ = .;
        link_global_data_initial_values = LOADADDR( .data ); /* This is the location in flash of the initial values of global variables */
        link_global_data_start = .;

        *(vtable)

        *(.data*)


        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        *(.jcr)
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;
        link_global_data_end = .;

    } > RAM AT>APP_FLASH

    .bss :
    {
        . = ALIGN(4);
        link_bss_location = .;
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        link_bss_end = .;
        link_heap_start = .;
        link_end_of_static_ram = .;
    } > RAM


    INCLUDE module_end.ld    
}

