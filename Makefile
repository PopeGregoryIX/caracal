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

CURDIR:= $(shell pwd)

ifeq ($(BINDIR),)
export BINDIR:= $(CURDIR)/bin
endif

ifndef VERBOSE
.SILENT:
endif

.phony: all install clean doc run debug

all:
	@echo Building...
	@make -s -C cboot

install:
	@echo Installing...
	@cp ./bin/cboot.elf ./filesystem/sys/core
	@./tools/bootboot/mkbootimg ./config/caracal.json ./images/caracal64.img
	
clean:
	@echo Cleaning...	
	@rm -rf $(BINDIR)
	@make clean -s -C cboot

doc:
	@echo Making Documentation...
	
run:
	@qemu-system-x86_64 -m 8G -boot d -smp 4 -usb -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
	-drive file="$$HOME/caracal/images/caracal64.img",if=ide,index=1,format=raw

debug:
	@qemu-system-x86_64 -S -s -m 8G -boot d -smp 4 -usb -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
	-drive file="$$HOME/caracal/images/caracal64.img",if=ide,index=1,format=raw

makedirs:
	@mkdir -p $(BINDIR)

gdb:
	@make -s gdb -C kernel