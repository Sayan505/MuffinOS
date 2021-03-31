OUTPUT_FORMAT(elf64-x86-64)

ENTRY(start_kernel)

SECTIONS
{
    . = 0x100000;
    .text   : ALIGN(0x1000) { *(.text)   }
    .rodata : ALIGN(0x1000) { *(.rodata) }
    .data   : ALIGN(0x1000) { *(.data)   }
    
    __LNK_SYM_bss_start = .;
    .bss    : ALIGN(0x1000) { *(.bss)    }
    __LNK_SYM_bss_end   = .;

    . = ALIGN(0x4000);
}
