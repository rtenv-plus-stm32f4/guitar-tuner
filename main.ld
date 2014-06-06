ENTRY(main)
MEMORY
{
  FLASH (rx)       : ORIGIN = 0x08000000, LENGTH = 2048K
  RAM (rwx)        : ORIGIN = 0x20000000, LENGTH = 192K
  MEMORY_B1 (rx)   : ORIGIN = 0x60000000, LENGTH = 0K
  CCMRAM (rw)      : ORIGIN = 0x10000000, LENGTH = 64K
}

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
 		*(.text)
 		*(.text.*)
		*(.rodata)
		*(.rodata*)
		_sromdev = .;
		*(.rom.*)
		_eromdev = .;
		_sidata = .;
	} >FLASH

	/* Initialized data will initially be loaded in FLASH at the end of the .text section. */
	.data : AT (_sidata)
	{
		_sdata = .;
		*(.data)		/* Initialized data */
		*(.data*)
		_edata = .;
	} >RAM

	.bss : {
		_sbss = .;
		*(.bss)         /* Zero-filled run time allocate data memory */
		_ebss = .;
	} >RAM

    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
