#!/bin/bash
set -e
./build.sh

qemu-system-x86_64 \
-m 8G \
-boot d \
-smp 2 \
-usb \
-drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
-drive file=$HOME/caracal/images/caracal64.img,if=ide,index=1,format=raw

#-d int,cpu_reset \