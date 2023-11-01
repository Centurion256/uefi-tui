# uefi-tui <br> 
# A simple port of ncurses to UEFI. 

## Prerequisites
* QEMU(in particular qemu-system-x86_64)
* Clang
* LLD - The LLVM Linker
* GNU Make

## Setup and compilation
After cloning the repository, you need to enter the program directory and run the `make` command. <br>
```shell
cd hello
make
```

Otherwise, you can just use Clion to set up build configuration. <br>

## Running
You can run your UEFI application using QEMU:
### In general case:
```shell
cp bootx64.efi root/efi/boot
qemu-system-x86_64 -machine ubuntu \
-drive if=pflash,format=raw,readonly=on,file=OVMF_CODE-pure-efi.fd \
-drive format=raw,file=UefiShell.iso \
-drive format=raw,file=fat:rw:root
```
In UEFI Shell, navigate to the `fs1:\efi\boot` and run the `bootx64.efi` file.

