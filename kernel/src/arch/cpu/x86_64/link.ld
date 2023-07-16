ENTRY(_entry)

/*  Used by BOOTBOOT.   */
/* overridden default bootboot values
mmio                = 0xfffffffff8000000;
linearFrameBuffer   = 0xfffffffffc000000;
bootboot            = 0xffffffffffe00000;
environment         = 0xffffffffffe01000;*/

mmio                = 0xFFFFFE8000000000;
fb                  = 0xFFFFFE8004000000;
bootboot            = 0xFFFFFE8008000000;
environment         = 0xFFFFFE8008100000;
initstack = 0x4000;

/*  Bootboot - puts everything in a single segment  */
PHDRS
{
  boot PT_LOAD;
}
SECTIONS
{
    . = 0xFFFFFE8100000000;
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

    . = ALIGN(0x1000);
    gdtSpace = .;

    /DISCARD/ : { *(.comment) }
}
