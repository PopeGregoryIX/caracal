#!/bin/bash
qemu-system-x86_64 \
-m 8G \
-boot d \
-smp 2 \
-usb \
-net none \
-drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd