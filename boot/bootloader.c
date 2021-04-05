#include "bootloader.h"


EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS efi_status;

    uint64_t   pImage_entry;    // stores the entry-point of the kernel image.
    stiletto_t stiletto;


    // fetch video info
    EFI_GUID efi_gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *efi_gfx;

    gBS->LocateProtocol(&efi_gop_guid, NULL, (VOID **)&efi_gfx);    // fetch

    // fetch available video modes
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *efi_gfx_mode_info;
    UINTN szEFI_gfx_mode_info;
    UINT32 MaxMode = efi_gfx->Mode->MaxMode;

    for(int i = 0; i < MaxMode; ++i) {
        efi_gfx->QueryMode(efi_gfx, i, &szEFI_gfx_mode_info, &efi_gfx_mode_info);    // query
        if(efi_gfx_mode_info->HorizontalResolution == 1920 && efi_gfx_mode_info->VerticalResolution == 1080) {
            //found the prefered mode
            efi_gfx->SetMode(efi_gfx, i);   // set it
            break;
        }
    }
    
    // else, use the initial mode
    Print(L"%d: %dx%d\t", efi_gfx->Mode->Mode, efi_gfx_mode_info->HorizontalResolution, efi_gfx_mode_info->VerticalResolution);   // display

    // store it
    stiletto.stiletto_video.pFrame_buffer_base = (uint32_t *)efi_gfx->Mode->FrameBufferBase;
    stiletto.stiletto_video.frame_buffer_sz    = (uint64_t)  efi_gfx->Mode->FrameBufferSize;
    stiletto.stiletto_video.horiz              = (uint32_t)  efi_gfx->Mode->Info->HorizontalResolution;
    stiletto.stiletto_video.vert               = (uint32_t)  efi_gfx->Mode->Info->VerticalResolution;
    stiletto.stiletto_video.ppsl               = (uint32_t)  efi_gfx->Mode->Info->PixelsPerScanLine;


    // get RSDP
    VOID *rsdp             = NULL;
    EFI_GUID efi_acpi_guid = EFI_ACPI_TABLE_GUID;

    for(UINTN i = 0; i < SystemTable->NumberOfTableEntries; ++i) {
        EFI_CONFIGURATION_TABLE *efi_cfg = &SystemTable->ConfigurationTable[i]; // go through all tables
        if(CompareGuid(&efi_cfg->VendorGuid, &efi_acpi_guid)) {
            rsdp = &efi_cfg->VendorTable;       // capture if matched
            
            break;
        }
    }

    stiletto.stiletto_acpi.acpi_rsdp = rsdp;    // store RSDP


    // init EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
    EFI_GUID efi_fs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *efi_fs;

    gBS->LocateProtocol(&efi_fs_guid, NULL, (VOID **)&efi_fs);

    // open a handle to the root dir of the boot vol
    EFI_FILE_PROTOCOL *root_dir;

    efi_status = efi_fs->OpenVolume(efi_fs, &root_dir);
    if(EFI_ERROR(efi_status)) {
        Print(L"[X]\tERROR: CANNOT ACCESS BOOT MEDIA!");
        for(;;) { __asm__ volatile("hlt"); }
    }

    // open a handle to the kernel image
    EFI_FILE_PROTOCOL *image_handle = NULL;

    efi_status = root_dir->Open(root_dir, &image_handle, KERNEL_ABS_PATH, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if(EFI_ERROR(efi_status)) {
        Print(L"[X]\tERROR: KERNEL NOT FOUND!");
        for(;;) { __asm__ volatile("hlt"); }
    }

    Print(L"\n[Y]\tKERNEL FOUND!\n");

    // get kernel image size
    EFI_FILE_INFO *efi_file_info;
    EFI_GUID efi_file_info_guid = EFI_FILE_INFO_ID;
    UINTN efi_file_info_sz      = sizeof(EFI_FILE_INFO) + 1024;

    efi_status = gBS->AllocatePool(EfiBootServicesData, efi_file_info_sz, (VOID **)&efi_file_info);    // alloc mem for EFI_FILE_INFO
    if(EFI_ERROR(efi_status)) {
        Print(L"\n[X]\tError Allocating memory for EFI_FILE_INFO.");
        for(;;) { __asm__ volatile("hlt"); }
    }

    image_handle->GetInfo(image_handle, &efi_file_info_guid, &efi_file_info_sz, efi_file_info);    // populate the EFI_FILE_INFO structure

    UINTN image_size = efi_file_info->FileSize; // store the image' size

    gBS->FreePool(efi_file_info);               // free mem for EFI_FILE_INFO

    Print(L"\tImage Size: %llu Bytes\n", image_size);


    // alloc kernel
    EFI_PHYSICAL_ADDRESS image_pool = (EFI_PHYSICAL_ADDRESS)IMAGE_LOAD_ADDR;

    efi_status = gBS->AllocatePages(AllocateAddress, EfiLoaderData, EFI_SIZE_TO_PAGES(image_size), &image_pool);
    if(EFI_ERROR(efi_status)) {
        Print(L"\n[X]\tError Allocating memory for kernel.");
        for(;;) { __asm__ volatile("hlt"); }
    }


    // load the image into memory
    efi_status = image_handle->Read(image_handle, &image_size, (VOID *)image_pool);
        if(EFI_ERROR(efi_status)) {
        Print(L"\n[X]\tError loading kernel from disk.");
        for(;;) { __asm__ volatile("hlt"); }
    }

    // prep image for exec and get entry point
    pImage_entry = load_image((VOID *)image_pool);

    gBS->FreePages(image_pool, EFI_SIZE_TO_PAGES(image_size));  // free the buffer

    Print(L"[Y]\tKERNEL LOADED!\n");


    // fetch EFI Memory Map
    EFI_MEMORY_DESCRIPTOR *efi_mem_map = NULL;
    UINTN efi_mem_map_sz, efi_mem_map_key, efi_desc_sz;
    UINT32 efi_desc_ver;

    gBS->GetMemoryMap(&efi_mem_map_sz, efi_mem_map, NULL, &efi_desc_sz, NULL); // get pool size

    efi_mem_map_sz += 2 * efi_desc_sz;  // adjust with new descriptors created after AllocatePool

    efi_status = gBS->AllocatePool(EfiLoaderData, efi_mem_map_sz, (VOID **)&efi_mem_map);  // alloc mem for storing the EFI Memory Map
    if(EFI_ERROR(efi_status)) {
        Print(L"\n[X]\tError allocating memory for the Memory Map!");
        for(;;) { __asm__ volatile("hlt"); }
    }

    efi_status = gBS->GetMemoryMap(&efi_mem_map_sz, efi_mem_map, &efi_mem_map_key, &efi_desc_sz, &efi_desc_ver);
    if(EFI_ERROR(efi_status)) {
        Print(L"\n[X]\tError Fetching EFI Memory Map");
        for(;;) { __asm__ volatile("hlt"); }
    }

    // store it
    stiletto.stiletto_memory.pMem_map = efi_mem_map;
    stiletto.stiletto_memory.map_sz   = efi_mem_map_sz;
    stiletto.stiletto_memory.desc_sz  = efi_desc_sz;
    
    
    // ExitBootServices()
    gBS->ExitBootServices(ImageHandle, efi_mem_map_key);

    
    // READY!
    ((__attribute__ ((sysv_abi)) void(*)(stiletto_t *))pImage_entry)(&stiletto);

    return EFI_SUCCESS;
}
