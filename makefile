# REQUIRES ROOT ACCESS TO BUILD THE FINAL DISK IMAGE

# make init_submodules : fetch EDK2 and it's submodules.

# make all		  : build full system with disk images.
# make testall	  : build full system in $(FSDIR) without generating disk images.

# make bootloader : build the MuffinBoot bootloader only.
# make kernel	  : build the kernel only.

# make test		  : build the kernel and copies the bootloader over into in $(FSDIR), if present.
# make testboot   : build the bootloader and copies the kernel over into in $(FSDIR), if present.

# make run		  : run full system from the disk image in $(IMGDIR).
# make testrun	  : run full system from the $(FSDIR) with the disk in R/W mode.


# STEPS FOR A FULL MANUAL BUILD:

# make dirs
# make bootloader
# make kernel
# make fs
#	(at this stage, make any manual changes to the filesystem if any)
# make img


# KERNEL DEV WORKFLOW:

# code...
# make test
# make testrun



# toolchain:
CC = clang
LD = ld
AS = nasm

# outputs:
KNL = kernel
BOOTX64.EFI = BootX64.efi

IMG = OS.img

# disk image size in MiB:
IMGMB = 64

# QEMU config:
RAM = 8G
CPUS = 4

# dirs:
  # extra sources: 
EDKDIR     = edk2
BOOTDIR    = boot
PROGSRCDIR = programs

  # EDK2 init & MuffinBootPkg:
EDKCFGDIR = edk2config

  # output:
    # base build folder:
BUILDDIR  = build

    # base binary folder:
BINDIR    = $(BUILDDIR)/bin

    # kernel:
KNLDIR    = $(BINDIR)/kernel

    # programs:
PROGDIR  = $(BINDIR)/programs

    # root filesystem directory:
FSDIR     = $(BUILDDIR)/fs

    # bootloader in $(FSDIR):
EFIDIR    = $(FSDIR)/EFI/BOOT

	# kernel in $(FSDIR):
SYSDIR    = $(FSDIR)/sys

	# programs in $(FSDIR):
PRGDIR    = $(FSDIR)/prog

    # disk image output folder: 
IMGDIR    = $(BUILDDIR)/img

    # loop-disk mount point:
MNTPNT    = $(BUILDDIR)/mnt


# kernel sources:
C_KERNELSRC   := $(shell find ./ -type d \( -path ./$(BOOTDIR) -o -path ./$(EDKDIR) -o -path ./$(PROGDIR) \) -prune -false -o -name '*.c')
ASM_KERNELSRC := $(shell find ./ -type d \( -path ./$(BOOTDIR) -o -path ./$(EDKDIR) -o -path ./$(PROGDIR) \) -prune -false -o -name '*.asm')
##$(shell find ./ -type d \( -path ./boot -o -path ./progs \) -prune -false -o -name '*.c')

# objs:
OBJ := ${C_KERNELSRC:.c=.o} ${ASM_KERNELSRC:.asm=.o}


# toolchain flags:
CFLAGS = -target x86_64-unknown-none-elf64 	\
		 -O0								\
		 -std=c17							\
		 -Wall								\
		 -Wextra							\
		 -Wpedantic							\
		 -ffreestanding						\
		 -nostdlib							\
		 -nodefaultlibs						\
		 -fno-pic							\
		 -fno-stack-protector				\
		 -fno-omit-frame-pointer			\
		 -mcmodel=kernel					\
		 -mno-red-zone						\
		 -mno-mmx							\
		 -mno-sse							\
		 -mno-sse2							\
		 -mno-sse3							\
		 -mno-ssse3							\
		 -mno-sse4							\
		 -mno-sse4.1						\
		 -mno-sse4.2						\
		 -mno-80387							\
		 -I.

LDFLAGS = -T linker.ld						\
		  -no-pie							\
		  -nostdlib							\
		  -z max-page-size=0x1000

ASFLAGS = -O0								\
		  -f								\
		  elf64

QEMUFLAGS = -bios ovmf/OVMF.fd				\
			-machine q35					\
			-cpu max,kvm=off				\
			-d cpu_reset					\
			-serial stdio					\
			-hda $(IMGDIR)/$(IMG)			\
			-m $(RAM)						\
			-smp cpus=$(CPUS),maxcpus=$(CPUS),cores=$(CPUS),threads=1,sockets=1

# testing:
QEMURUNFLAGS = -bios ovmf/OVMF.fd			\
			   -machine q35					\
			   -cpu max,kvm=off				\
			   -d cpu_reset					\
			   -serial stdio				\
			   fat:rw:$(FSDIR)/  			\
			   -m $(RAM)					\
			   -smp cpus=$(CPUS),maxcpus=$(CPUS),cores=$(CPUS),threads=1,sockets=1

# /dev/loop???:
LOOPDSK := 0




# START:
all : build_system
	@echo "\nOutput: $(IMGDIR)/$(IMG)\nmake run"


# build the whole system with disk images:
build_system : dirs bootloader kernel progs fs img




# generate the build folder hierarchy:
dirs : build_dir knl_dir prog_dir fs_dir efi_dir_fs knl_dir_fs prog_dir_fs img_dir mnt_pnt

build_dir :
	@mkdir -p $(BUILDDIR)

knl_dir :
	@mkdir -p $(BUILDDIR)
		@mkdir -p $(BINDIR)
			@mkdir -p $(KNLDIR)

prog_dir :
	@mkdir -p $(BUILDDIR)
		@mkdir -p $(BINDIR)
			@mkdir -p $(PROGDIR)

fs_dir :
	@mkdir -p $(BUILDDIR)
		@mkdir -p $(FSDIR)

efi_dir_fs :
	@mkdir -p $(BUILDDIR)
		@mkdir -p $(FSDIR)
			@mkdir -p $(EFIDIR)

knl_dir_fs :
	@mkdir -p $(BUILDDIR)
		@mkdir -p $(FSDIR)
			@mkdir -p $(SYSDIR)

prog_dir_fs :
	@mkdir -p $(BUILDDIR)
		@mkdir -p $(FSDIR)
			@mkdir -p $(PRGDIR)

img_dir :
	@mkdir -p $(BUILDDIR)
		@mkdir -p $(IMGDIR)

mnt_pnt :
	@mkdir -p $(BUILDDIR)
		@mkdir -p $(MNTPNT)



# build MuffinBoot bootloader:
bootloader : compile_loader fetch_loader

compile_loader :
	\cp  -rf $(EDKCFGDIR)/* $(EDKDIR)/
	cd $(EDKDIR); bash build.sh

fetch_loader :
	mkdir -p $(BOOTDIR)/$(BUILDDIR)
	\cp -f $(EDKDIR)/Build/MuffinBootPkg/RELEASE_GCC5/X64/$(BOOTX64.EFI) $(BOOTDIR)/$(BUILDDIR)/




# build MuffinOS kernel:
kernel : knl_dir compile_kernel

# compile & link kernel:
compile_kernel : $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $(KNLDIR)/$(KNL)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o : %.asm
	$(CC) $(CFLAGS) -c $< -o $@




# build Programs:
progs : prog_dir
	make -C $(PROGSRCDIR)/




# load the system along with it's programs into the filesystem directory in $(FSDIR):
fs : fs_dir load_bootloader load_kernel load_programs

# load the bootloader into the filesystem:
load_bootloader : efi_dir_fs
	\cp -f $(BOOTDIR)/$(BUILDDIR)/$(BOOTX64.EFI) $(EFIDIR)/ 2> /dev/null || :

# load the kernel into the filesystem:
load_kernel : knl_dir knl_dir_fs
	\cp -f $(KNLDIR)/$(KNL) $(SYSDIR) 2> /dev/null || :

# load the programs into the filesystem:
load_programs : prog_dir prog_dir_fs
	@echo " "




# generate disk image from $(FSDIR):
img : gen_img fmt_img mount_img load_img umount_img

gen_img : img_dir
	dd if=/dev/zero of=$(IMGDIR)/$(IMG) bs=1M count=$(IMGMB) oflag=sync

fmt_img : gen_img
	@echo "sudo losetup -fP $(IMGDIR)/$(IMG)"
	$(eval LOOPDSK=$(shell sudo losetup --show -fP $(IMGDIR)/$(IMG)))

	sudo parted -s $(IMGDIR)/$(IMG) mktable gpt
	sudo parted -s $(LOOPDSK) mkpart OS 1MiB 100%
	sudo parted -s $(LOOPDSK) set 1 esp on

	sudo mkfs.fat -F 32 $(LOOPDSK)p1

mount_img : mnt_pnt
	sudo mount $(LOOPDSK)p1 -t msdos -o umask=0000 $(MNTPNT)

load_img : mount_img
	\cp -rf $(FSDIR)/* $(MNTPNT)

umount_img :
	sudo umount $(MNTPNT)
	sudo losetup -d $(LOOPDSK)




# TESTING:

# build both bootloader and kernel into $(FSDIR) without generating disk images:
testall : dirs bootloader kernel progs fs

# build bootloader only:
testboot       : testloader
testloader     : testbootloader
testbootloader : bootloader fs

# build kernel only:
test : testkernel
testkernel : kernel fs


# CI/CD:
ci : knl_dir compile_loader compile_kernel


# TEST-RUNNING:
# run the system from $(FSDIR) with the disk in R/W mode:
testrun :
	@clear
	qemu-system-x86_64 $(QEMURUNFLAGS)




# RUNNING:
run :
	@clear
	qemu-system-x86_64 $(QEMUFLAGS)




init_sub 	    : init_submodules
init_submodules :
	rm -rf $(EDKDIR)
	git submodule update --init --recursive --progress




# clean:
clear : clean
clean :
	rm -rf $(BUILDDIR)
	rm -rf $(shell find ./ -type d \( -path ./edk2 \) -prune -false -o -name '*.o')
	rm -rf edk2/Build/

clean_ci :
	rm -rf $(BUILDDIR)
	rm -rf $(shell find ./ -type d \( -path ./edk2 \) -prune -false -o -name '*.o')
	rm -rf $(EDKDIR)/Build/MuffinBootPkg/RELEASE_GCC5/X64/$(BOOTX64.EFI)