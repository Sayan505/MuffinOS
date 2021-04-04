OUTPUT_FORMAT(elf64-x86-64)

ENTRY(start_kernel)

SECTIONS
{
    . = 0x100000;
    __LNK_SYM_kernel_start = .;

    .text : ALIGN(0x1000)
    {
        __LNK_SYM_text_start = .;
        *(.text)
        __LNK_SYM_text_end   = .;
    }

    .rodata : ALIGN(0x1000)
    {
        __LNK_SYM_rodata_start = .;
        *(.rodata)
        __LNK_SYM_rodata_end   = .;
    }

    .data : ALIGN(0x1000)
    {
        __LNK_SYM_data_start = .;
        *(.data)
        __LNK_SYM_data_end   = .;
    }

    .eh_frame : ALIGN(0x1000)
    {
        __LNK_SYM_eh_frame_start = .;
        *(.eh_frame)
        __LNK_SYM_eh_frame_end  = .;
    }

    .bss : ALIGN(0x1000)
    {
        __LNK_SYM_bss_start = .;
        *(.bss)
        LONG(0)     /* "The LONG(0) forces the bss section to be written to the file." */
        __LNK_SYM_bss_end   = .;

        *(COMMON)
    }

    /DISCARD/ :
    {
        *(.comment)
    }


    . = ALIGN(0x1000);
    __LNK_SYM_kernel_end = .;
}
