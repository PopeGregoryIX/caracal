ENTRY(bmain)

PHDRS
{
  boot PT_LOAD;
}
SECTIONS
{
    . = 0xfffffffff8002000;
    kernelStart = .;
    .text : {
        KEEP(*(.text.boot*)) 
        *(.fini*)
        *(.init*)
		*(.ctor*)
		*(.dtor*)
		*(.ehframe* .eh_frame*)
        *(.text* .text.*)
        *(.rodata* .rodata.*)
        *(.data* .data.*)
    } :boot 
    .bss (NOLOAD) : {
        . = ALIGN(16);
        *(.bss* .bss.*)
        *(COMMON)
    } :boot

    kernelEnd = .;

    /DISCARD/ : { *(.comment) }
}