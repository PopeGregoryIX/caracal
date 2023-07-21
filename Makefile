#	General Makefile for all architectures
ifeq ($(ARCH),)
export ARCH:= pc
endif

ifeq ($(CPU),)
export CPU:= x86_64
endif

ifeq ($(BINFORMAT),)
export BINFORMAT:= elf
endif

ifndef VERBOSE
.SILENT:
endif

.phony: all install clean doc run debug

all:
	@echo Building...
	@make -s -C bootstrap

install:
	@echo Installing...

clean:
	@echo Cleaning...	
	@make clean -s -C bootstrap

doc:
	@echo Making Documentation...
	
run:
	@qemu-system-x86_64 -m 8G -boot d -smp 4 -usb -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
	-drive file="$$HOME/caracal/images/caracal64.img",if=ide,index=1,format=raw

debug:
	@qemu-system-x86_64 -S -s -m 8G -boot d -smp 4 -usb -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
	-drive file="$$HOME/caracal/images/caracal64.img",if=ide,index=1,format=raw

gdb:
	@make -s gdb -C kernel