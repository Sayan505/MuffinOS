# MuffinOS
An experimental x86_64 hobby OS project.

&nbsp;
## Road Map
#####  LEGEND:  
💙: On my list.   
💛: In-progress.  
💚: Completed.  
&nbsp;  
### Progress:
💚 UEFI Booting.  
💚 Serial Port.  
💛 GDT.  
💛 Re-configure Paging.  
💛 Higher-Half Kernel.  
💛 Text Renderer on GOP.  
💛 Shell. (to be developed further as I progress)  
💙 Load IDT.   
💙 Keyboard Driver.  
💙 pcspkr.  
💙 PMM.  
💙 VMM.  
💙 Parse ACPI tables. (MADT for APIC)  
💙 ACPI Shutdown.  
💙 APICs.  
💙 LAPIC Timer.  
💙 HPET Timer.  
💙 A Basic Graphics Library. (GOP)  
💙 PCI Driver.  
💙 SMP & SMT.  
💙 Scheduler.  
💙 Syscalls and API.  
💙 Storage Driver. (AHCI, FAT32)  
💙 ELF64 binary loading and exec as "Processes".  
💙 IPC.  
💙 libc.  

&nbsp;

## Build MuffinOS
### &nbsp; Get the source:
&nbsp; &nbsp; `git clone --recursive` during cloning or,   
&nbsp; &nbsp; `git submodule update --init --recursive` after cloning to get EDK2 and it's submodules
### &nbsp; Requirements
* GCC (cross-compiler coming up soon)
* GNU LD
* GNU Parted
* dosfstools
* EDK II (optional; binary included; to build the MuffinBoot bootloader)

### &nbsp; Process
&nbsp; Build full system with disk images (losetup requires root):  
&nbsp; &nbsp; `make`  
&nbsp; Build full system in $(FSDIR) without generating disk images:  
&nbsp; &nbsp; `make testall`  
&nbsp; Build and test kernel only, in $(FSDIR) without generating disk images:  
&nbsp; &nbsp; `make test` after `make bootloader`, once.

&nbsp;

## Run MuffinOS
&nbsp; Run the full system from the disk image:  
&nbsp; &nbsp; `make run`  
&nbsp; Run the full system from $(FSDIR) in RW mode:  
&nbsp; &nbsp; `make testrun` 

&nbsp;

## Build MuffinBoot UEFI bootloader
### &nbsp; Requirements
* GCC
* EDK II

### &nbsp; Process
+ Build with MdeModulePkg. (MuffinBoot.inf included)  
+ Place the BootX64.efi file in boot/build  
