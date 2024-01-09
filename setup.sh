#! /usr/bin/env bash

# build edk2 with libc support
git clone https://github.com/tianocore/edk2.git
git clone https://github.com/tianocore/edk2-libc.git
cd edk2
git submodule update --init
make -C BaseTools
source ./edksetup.sh
### change the Conf/target.txt as specified in the README
build
### change the Conf/target.txt as specified in the README
build

cd ..
export WORKSPACE="$PWD/edk2"
export PACKAGES_PATH="$PWD/edk2-libc:$PWD/edk2"
export EDK_TOOLS_PATH="$PWD/edk2/BaseTools"


cd "$WORKSPACE" || exit 1

source ./edksetup.sh --reconfig

cp Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd ../hello/ovmf.flash
cd ../hello || exit 1
if [ ! -d mnt_app ]; then
    mkdir mnt_app
fi
dd if=/dev/zero of=app.disk bs=1 count=1 seek=$(( (16 * 1024 * 1024) - 1))
sudo mkfs.vfat app.disk

sudo mount app.disk mnt_app
sudo cp ../edk2/Build/AppPkg/DEBUG_GCC5/X64/Snake.efi mnt_app
sudo umount mnt_app

