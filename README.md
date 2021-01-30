# MuffinOS
An experimental x86_64 hobby OS project.
&nbsp;  
&nbsp;  

![](https://travis-ci.com/Sayan505/MuffinOS.svg?branch=master)

&nbsp;
## Features
#####  LEGEND:  
💙: On my list.   
💛: In-progress.  
💚: Completed.  
&nbsp;  
### Progress:
💚 Boot Protocol. (Stiletto)  
💚 UEFI Booting with Identity Paging (MuffinBoot; ELF64 kernel).  
💚 Serial Port.  
💚 GOP Video.  
💛 Text Renderer on GOP. (stdout)    
💛 PIT Timer.  
💛 pcspkr.   
And, so on...   

&nbsp;

## Build MuffinOS
### &nbsp; Get the source
&nbsp; &nbsp; `git clone` to test the kernel with the pre-build bootloader or,  
&nbsp; &nbsp; `git clone --recursive` during cloning to fetch this repo and EDK2 + all its submodules or,   
&nbsp; &nbsp; `make init_submodules`, which is `git submodule update --init --recursive` after `git clone` to get EDK2 and its submodules.
### &nbsp; Requirements
* clang (for Kernel)
* GCC (build-essential; for the MuffinBoot Bootloader)
* GNU LD
* nasm
* GNU Parted
* dosfstools
* EDK II (optional; binary included, to build the MuffinBoot bootloader)

### &nbsp; Process
&nbsp; Build full system with disk images (losetup requires root):  
&nbsp; &nbsp; `make`  
&nbsp; Build full system in $(FSDIR) without generating disk images:  
&nbsp; &nbsp; `make testall`  
&nbsp; Build to test the kernel only, in $(FSDIR) without generating disk images:  
&nbsp; &nbsp; `make test`, requires `make bootloader`, once.

&nbsp;

## Run MuffinOS
&nbsp; Run the full system from the disk image:  
&nbsp; &nbsp; `make run`  
&nbsp; Run the full system from $(FSDIR) in RW mode:  
&nbsp; &nbsp; `make testrun`  

&nbsp;

## Build MuffinBoot UEFI bootloader
### &nbsp; Requirements
* Atleast GCC5 (build-essential)
* uuid-dev
* iasl
* nasm
* python3-distutils
* [EDK II](https://github.com/tianocore/edk2): `git submodule update --init --recursive`  
###### &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Dependencies of EDK II are subject to change.

### &nbsp; Process
+ Build with MuffinBootPkg. (Platform and inf included)
+ just do `make bootloader`.
