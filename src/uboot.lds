
SECTION {
	. = 0x33F80000;
	.test : {*(.test)}

	. = ALIGN(4);
	.rodata : {*(.rodata)}

	. = ALIGN(4);
	.data : {*(.data)}

	__bss_start = .;
	.bss : {*(.bss) *(COMMON)}
	__bss_end = .;
}
