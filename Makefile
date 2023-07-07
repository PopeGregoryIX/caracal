ifndef VERBOSE
.SILENT:
endif

.phony: all install clean doc run debug

all:
	@echo Building...
	@make -s all -C kernel

install:
	@echo Installing...
	@cp ./kernel/bin/pc/x86_64-elf/kernel.sys ./filesystem/sys/core
	@./tools/bootboot/mkbootimg ./config/caracal.json ./images/caracal64.img

clean:
	@echo Cleaning...
	@make -s clean -C kernel
	@rm -f images/*.img
	@rm -f ./filesystem/sys/core

doc:
	@echo Making Documentation...
	@make -s doc -C kernel

run:
	@qemu-system-x86_64 -m 8G -boot d -smp 4 -usb -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
	-drive file="$$HOME/caracal/images/caracal64.img",if=ide,index=1,format=raw

debug:
	@qemu-system-x86_64 -S -s -m 8G -boot d -smp 4 -usb -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
	-drive file="$$HOME/caracal/images/caracal64.img",if=ide,index=1,format=raw

gdb:
	@make -s gdb -C kernel