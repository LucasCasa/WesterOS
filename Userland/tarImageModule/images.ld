OUTPUT_FORMAT("binary")
ENTRY(main)
SECTIONS
{
	.text 0x850000 :
	{
		text = .;
		*(.text*)
		. = ALIGN(0x2000);
		rodata = .;
		*(.rodata*)
	}
	.data ALIGN(0x2000) :
	{
		data = .;
		*(.data*)
	}
	.bss ALIGN(0x2000) :
	{
		bss = .;
		*(.bss*)
	}
	endOfBinary = .;
}
