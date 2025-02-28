
SECTIONS
{
	. = 2M;

	.enter_kernel BLOCK(4K) : ALIGN(4K)
	{
		*(.enter_kernel)
		*(.text)
	}

	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
}
