#include "bootloader.h"


uint64_t load_image(void *pImage) {
    Elf64_Addr *file_segment;
    EFI_PHYSICAL_ADDRESS mem_segment;
    UINT8 magic[4] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3}; // 0x7F, 'E', 'L', 'F'

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)pImage;
    UINT8 *phoff     = (UINT8 *)pImage + ehdr->e_phoff;

    for(UINTN i = 0; i < 4; ++i) {
        if(ehdr->e_ident[i] != magic[i]) {
            Print(L"\n[X]\tBAD ELFMAG");
            for(;;) { __asm__ volatile("hlt"); }
        }
    }
    
    if(ehdr->e_machine != EM_X86_64) {
        Print(L"\n[X]\tBAD ARCH");
        for(;;) { __asm__ volatile("hlt"); }
    }

    Print(L"[Y]\tELF64\n");

    for(UINTN i = 0; i < ehdr->e_phnum; ++i) {
        
        Elf64_Phdr *phdr = (Elf64_Phdr *)phoff;

        if(phdr->p_type == PT_LOAD) {
            file_segment = (Elf64_Addr *)((UINTN)pImage + phdr->p_offset);    // offset in the image (src)
            mem_segment  = (EFI_PHYSICAL_ADDRESS)phdr->p_paddr;               // loc in the memory (dest)

            gBS->AllocatePages(AllocateAddress, EfiLoaderCode, EFI_SIZE_TO_PAGES(phdr->p_memsz), &mem_segment); // alloc mem for segments (ignore clashes)
            
            gBS->CopyMem((VOID *)mem_segment, file_segment, phdr->p_filesz);  // load
        }

        phdr += ehdr->e_phentsize;  // next seg
    }

    return (uint64_t)ehdr->e_entry;
}
