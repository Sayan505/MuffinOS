# REQUIRES ROOT ACCESS TO BUILD THE FINAL DISK IMAGE

# make prepdirs 	   : prepares build directory hierarchy.
# make init_submodules : init submodules (EDK2):
# make 				   : makes full system with disk images.
# make testall		   : builds full system in $(FSDIR) without generating disk images.
# make test 		   : makes the KERNEL in $(FSDIR) without generating disk images.
# make bootloader 	   : makes the bootloader with EDK2.
# make kernel	 	   : build the full kernel.
# make run 			   : runs the full system from the disk image.
# make testrun 		   : runs the full system from $(FSDIR) in RW mode.
# make ci			   : for CI/CD.


# toolchain:
CC = clang
LD = ld
AS = nasm

# outputs:
BIN   = kernel
IMG   = OS.img

# disk image size in MiB:
IMGMB = 64

# QEMU config:
RAM = 8G
CPU = 4

# dirs:
EDKDIR    = edk2
LOADRDIR  = boot
EDKCFGDIR = edk2config
BUILDDIR  = build
BINDIR    = $(BUILDDIR)/exec
FSDIR     = $(BUILDDIR)/fs
IMGDIR    = $(BUILDDIR)/img
EFIDIR    = $(FSDIR)/EFI/BOOT
MNTPNT    = $(BUILDDIR)/mnt

# bootloader binary:
BOOTX64.EFI = BootX64.efi

# sources:
C_SYSTEMSRC   := $(shell find ./ -type d \( -path ./boot -o -path ./edk2 \) -prune -false -o -name '*.c')
ASM_SYSTEMSRC := $(shell find ./ -type d \( -path ./boot -o -path ./edk2 \) -prune -false -o -name '*.asm')
##$(shell find ./ -type d \( -path ./boot -o -path ./progs \) -prune -false -o -name '*.c')

# objs:
OBJ := ${C_SYSTEMSRC:.c=.o} ${ASM_SYSTEMSRC:.asm=.o}

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
		 -mno-80387

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
			-smp cpus=$(CPU),maxcpus=$(CPU),cores=$(CPU),threads=1,sockets=1

# testing:
QEMURUNFLAGS = -bios ovmf/OVMF.fd			\
			   -machine q35					\
			   -cpu max,kvm=off				\
			   -d cpu_reset					\
			   -serial stdio				\
			   fat:rw:$(FSDIR)/  			\
			   -m $(RAM)					\
			   -smp cpus=$(CPU),maxcpus=$(CPU),cores=$(CPU),threads=1,sockets=1

# /dev/loop???:
LOOPDSK := 0




# START:
all : build_system
	@echo "\nOutput: $(IMGDIR)/$(IMG)\nmake run"

# build the whole system with disk images:
build_system : loadimg

# load the file-system into the image:
loadimg : fmtimg
	sudo mount $(LOOPDSK)p1 -t msdos -o umask=0000 $(MNTPNT)

	\cp -rf $(FSDIR)/* $(MNTPNT)

	sudo umount $(MNTPNT)
	sudo losetup -d $(LOOPDSK)

# format the file-system (FAT32) in the disk image:
fmtimg : genimg
	@echo "sudo losetup -fP $(IMGDIR)/$(IMG)"
	$(eval LOOPDSK=$(shell sudo losetup --show -fP $(IMGDIR)/$(IMG)))

	sudo parted -s $(IMGDIR)/$(IMG) mktable gpt
	sudo parted -s $(LOOPDSK) mkpart OS 1MiB 100%
	sudo parted -s $(LOOPDSK) set 1 esp on

	sudo mkfs.fat -F 32 $(LOOPDSK)p1

# generate a blank disk image:
genimg : prepfs
	dd if=/dev/zero of=$(IMGDIR)/$(IMG) bs=1M count=$(IMGMB) oflag=sync

# prepare the file-system dir:
prepfs : prepdirs build_loader build_kernel loadfs

# load system into the file-system dir:
loadfs :
	\cp -f $(LOADRDIR)/$(BUILDDIR)/$(BOOTX64.EFI) $(EFIDIR)/
	\cp -f $(BINDIR)/$(BIN) $(FSDIR)/

# prepare build directory:
prepdirs :
	@mkdir -p $(BINDIR) $(MNTPNT) $(FSDIR) $(EFIDIR) $(IMGDIR)


# build kernel:
kernel       : build_kernel
build_kernel : prepdirs link_kernel

# link kernel:
link_kernel : $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $(BINDIR)/$(BIN)

# compile kernel:
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# build MuffinBoot bootloader:
bootloader   : build_loader

build_loader : compile_loader fetch_loader

compile_loader :
	\cp  -rf $(EDKCFGDIR)/* $(EDKDIR)/
	cd $(EDKDIR); bash build.sh

fetch_loader :
	mkdir -p $(LOADRDIR)/$(BUILDDIR)
	\cp -f $(EDKDIR)/Build/MuffinBootPkg/RELEASE_GCC5/X64/$(BOOTX64.EFI) $(LOADRDIR)/$(BUILDDIR)/




# RUN:
# run full system under QEMU from the disk image:
run :
	@clear
	qemu-system-x86_64 $(QEMUFLAGS)


# TESTING:
# test full system without generating disk images:
tstall   : testall
testall  : test_all
test_all : prepdirs build_loader kernel loadfs
	@echo "\nOutput: $(FSDIR)/\nmake testrun"

# test kernel without generating disk images:
tst  :  test
test : build_kernel loadfs
	@echo "\nOutput: $(FSDIR)/\nmake testrun"

# test bootloader without building kernel and generating disk images:
tl 		 : test_loader
test_ldr : test_loader
test_loader : prepdirs build_loader loadfs
	@echo "\nOutput: $(FSDIR)/\nmake testrun"

# run under QEMU from $(FSDIR):
tr      : testrun
testrun :
	@clear
	qemu-system-x86_64 $(QEMURUNFLAGS)


# CI:
ci : prepcidirs compile_loader link_kernel

prepcidirs :
	@mkdir -p $(BINDIR)


# init submodules (EDK2):
init_sub : init_submodules
init_submodules :
	rm -rf $(EDKDIR)
	git submodule update --init --recursive --progress




# clean:
clean :
	rm -rf $(BUILDDIR)
	rm -rf $(shell find ./ -type d \( -path ./edk2 \) -prune -false -o -name '*.o')
	rm -rf edk2/Build/

