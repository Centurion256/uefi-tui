#! /usr/bin/env bash

################BUILD#####################

source ./edksetup.sh
#build -a X64 -t GCC5 -p MdeModulePkg/MdeModulePkg.dsc
build -a X64 -t GCC5 -p AppPkg/AppPkg.dsc

###############COPY ELF###################

cd ../hello || exit 1
sudo -S mount app.disk mnt_app
sudo -S cp ../edk2/Build/AppPkg/DEBUG_GCC5/X64/Snake.efi mnt_app
#sudo -S cp ../edk2/Build/AppPkg/DEBUG_GCC5/X64/TestNcurses.efi mnt_app
#sudo -S cp ../edk2/Build/MdeModule/DEBUG_GCC5/X64/MdeModulePkg/Application/BootManagerMenuApp/BootManagerMenuApp/DEBUG/BootManagerMenuApp.efi mnt_app
sudo -S umount mnt_app

###############RUN########################
#
#qemu-system-x86_64 -machine ubuntu \
#-drive if=pflash,format=raw,readonly=on,file=ovmf.flash \
#-drive format=raw,file=UefiShell.iso \
#-drive file=app.disk,index=1,media=disk,format=raw
