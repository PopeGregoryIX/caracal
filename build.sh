#!/bin/bash

# delete old image - if we fail, don't want to run an old image
rm ./images/caracal64.img

# build the kernel
cd kernel
make all
cd ..

# create the initrd
cp ./kernel/bin/pc/x86_64-elf/kernel.sys ./filesystem/sys/core

./tools/bootboot/mkbootimg caracal.json ./images/caracal64.img
#genisoimage -iso-level 1 -b "bootboot.bin" -boot-info-table -no-emul-boot -boot-load-size 4 -input-charset utf-8 -o ./images/caracal.iso ./filesystem