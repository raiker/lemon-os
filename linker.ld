ENTRY (loader)

SECTIONS
{
	. = 0x00100000;

	.multibootheader ALIGN (0x400) :
	{
		*(.multibootheader)
	}

	.text ALIGN (0x1000) :
	{
		*(.text)
		*(.gnu.linkonce.t*)
	}

	.rodata ALIGN (0x1000) :
	{
		start_ctors = .;
		*(.ctor*)
		end_ctors = .;

		start_dtors = .;
		*(.dtor*)
		end_dtors = .;

		*(.rodata*)
		*(.gnu.linkonce.r*)
	}

	.data ALIGN (0x1000) :
	{
		*(.gdt)
		/*(.idt)*/
		*(.data)
		*(.gnu.linkonce.d*)
	}

	.bss :
	{
		sbss = .;
		*(COMMON)
		*(.bss)
		*(.gnu.linkonce.b*)
		ebss = .;
	}

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame) /* discard this, unless you are implementing runtime support for C++ exceptions. */
	}
}
