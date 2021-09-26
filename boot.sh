#! /bin/bash    
qemu-system-x86_64 \
-m 8G \
-boot d \
-smp 2 \
-usb \
-drive if=pflash,format=raw,readonly,file=/usr/share/OVMF/OVMF_CODE.fd \
-drive file=$HOME/projects/caracal/images/caracal.iso,if=ide,index=1,media=cdrom
