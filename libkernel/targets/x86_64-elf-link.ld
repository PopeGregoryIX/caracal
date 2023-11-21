ENTRY(bmain)

mmio                = 0xfffffffff0000000;
fb                  = 0xfffffffff4000000;
bootboot            = 0xfffffffff8000000;
environment         = 0xfffffffff8001000;
initstack           = 0x4000;
cboot               = 0xFFFFFE8000608000;

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