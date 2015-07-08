/* 
**	Linker script to configure memory regions. 
**
**	Device type: xxaa, 256K flash, 16K RAM
**	Device type: xxab, 128K flash, 16K RAM
**
**	Softdevice:  none, all memory available to device, 	Code start: 0x0000 , xxaa:0x40000, xxbb:0x20000, RAM start: 0x20000000 
**	Softdevice:  s110, uses 80K code, 8K RAM, 		Code start: 0x14000, xxaa:0x2c000, xxbb:0x0c000, RAM start: 0x20002000 
**	Softdevice:  s120, uses 96K code, 10K RAM, 		Code start: 0x18000, xxaa:0x28000, xxbb:0x08000, RAM start: 0x20002800 
*/ 

GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00016000, LENGTH = 0x26000
  RAM (rwx)  : ORIGIN = 0x20002000, LENGTH = 0x6000 
}

/*
** All permutations, copy one into the MEMORY section above, removing the leading '**'s
**
** xxaa, no softdevice
**  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x40000		
**  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 0x4000 
**
** xxab, no softdevice
**  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x20000	
**  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 0x4000 
**
** xxaa s110
**  FLASH (rx) : ORIGIN = 0x00014000, LENGTH = 0x2C000
**  RAM (rwx)  : ORIGIN = 0x20002000, LENGTH = 0x2000 
**
** xxab s110
**  FLASH (rx) : ORIGIN = 0x00014000, LENGTH = 0x0C000
**  RAM (rwx)  : ORIGIN = 0x20002000, LENGTH = 0x2000 
**
** xxaa s120
**  FLASH (rx) : ORIGIN = 0x00018000, LENGTH = 0x28000
**  RAM (rwx)  : ORIGIN = 0x20002800, LENGTH = 0x1800 
**
** xxab s120
**  FLASH (rx) : ORIGIN = 0x00018000, LENGTH = 0x08000
**  RAM (rwx)  : ORIGIN = 0x20002800, LENGTH = 0x1800 
**
*/





