#pragma once


#include <elf.h>

#include <Uefi.h>
#include <Guid/Acpi.h>
#include <Guid/FileInfo.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "types.h"
#include "protocol/stiletto.h"

#define KERNEL_ABS_PATH    L"\\sys\\kernel"
#define IMAGE_LOAD_ADDR    (0x0000000002000000)


uint64_t load_image(void *pImage);
