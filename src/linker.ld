MEMORY
{
	FLASH (rx)	: ORIGIN = 0x08000000, LENGTH = 64K
	RAW (rwx)	: ORIGIN = 0x02000000, LENGTH = 20K

}

SECTIONS
{

		.text : {

				. = 0;
				KEEP(*(.interrupt_vectors))
				. = 304;

				*(.text*)
		} > FLASH = 0xFF

		_end_of_ram = ORIGIN(RAM) + LENGTH(RAM);

}
