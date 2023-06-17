#!/bin/bash
qemu-system-x86_64 \
-m 8G \
-boot d \
-smp 2 \
-usb \
-drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
-drive file=$HOME/caracal/images/caracal.iso,if=ide,index=1,media=cdrom
