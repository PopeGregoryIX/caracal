#	General Makefile for all architectures
MACHINE?= pc
CPU?= x86_64
BINFORMAT?= elf
TRIPLET?= $(CPU)-$(BINFORMAT)
OBJCOPY_FORMAT?= elf64-x86-64
OBJCOPY_PLATFORM?= i386
BASEDIR?= $(shell pwd)
OBJBASE?= $(BASEDIR)/obj
BINBASE?= $(BASEDIR)/bin
IMGDIR?= $(BASEDIR)/img
FSDIR?= $(BASEDIR)/filesystem
CONFIGDIR?= $(BASEDIR)/config
CPPVER?= 13.2.0
CPPDIR?= ~/opt/cross/bin

#	System Binaries
QEMU?= qemu-system-$(CPU)
MKBOOTIMG?= ./tools/bootboot/mkbootimg
AS= $(CPPDIR)/$(TRIPLET)-as
AR?= $(CPPDIR)/$(TRIPLET)-ar
CXX= $(CPPDIR)/$(TRIPLET)-gcc-$(CPPVER)	# CXX is set by the system as g++ - we need to override this regardles of its status
LD= $(CPPDIR)/$(TRIPLET)-ld
OBJCOPY?= $(CPPDIR)/$(TRIPLET)-objcopy
OBJDUMP?= $(CPPDIR)/$(TRIPLET)-objdump
RANLIB?= $(CPPDIR)/$(TRIPLET)-ranlib

#	Make System
MAKE_EXPORTS:= ARCH=$(ARCH) MACHINE=$(MACHINE) CPU=$(CPU) BINFORMAT=$(BINFORMAT)
MAKE_EXPORTS+= TRIPLET=$(TRIPLET) BASEDIR=$(BASEDIR) BINBASE=$(BINBASE) OBJBASE=$(OBJBASE) 
MAKE_EXPORTS+= IMGDIR=$(IMGDIR)
MAKE_EXPORTS+= CPPDIR=$(CPPDIR)

ifndef VERBOSE
.SILENT:
endif

.phony: all install clean doc run debug all-boot clean-boot gdb

all: all-lib install-lib all-boot all-kernel

install:
	@echo Installing...
	@cp $(BINBASE)/cboot/$(TRIPLET)/cboot.sys ./filesystem/sys/core
	@cp $(BINBASE)/kernel/$(TRIPLET)/caracal.sys ./filesystem/sys/kernel
	@$(MKBOOTIMG) $(CONFIGDIR)/$(TRIPLET)-bootboot.json $(IMGDIR)/$(TRIPLET)-caracal.img
	
clean: clean-boot clean-lib clean-kernel
	@rm -rf $(BINBASE)
	@rm -rf $(OBJBASE)
	@rm -rf $(IMGDIR)

doc:
	@echo Making Documentation...
	
run:
	@$(QEMU) -m 8G -boot d -smp 4 -usb -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
	-drive file="$(IMGDIR)/$(TRIPLET)-caracal.img",if=ide,index=1,format=raw

debug:
	@$(QEMU) -S -s -m 8G -boot d -smp 4 -usb -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
	-drive file="$(IMGDIR)/$(TRIPLET)-caracal.img",if=ide,index=1,format=raw

all-boot: makedirs
	make -s -C cboot $(MAKE_EXPORTS)

install-boot: makedirs
	make install -s -C cboot $(MAKE_EXPORTS)

clean-boot:
	@make clean -s -C cboot $(MAKE_EXPORTS)

all-kernel: makedirs
	make -s -C kernel $(MAKE_EXPORTS)

clean-kernel:
	make clean -s -C kernel $(MAKE_EXPORTS)

all-lib: makedirs
	make -s -C libkernel $(MAKE_EXPORTS)

install-lib: makedirs
	make install -s -C libkernel $(MAKE_EXPORTS)

clean-lib:
	make clean -s -C libkernel $(MAKE_EXPORTS)

makedirs:
	@mkdir -p $(OBJBASE)
	@mkdir -p $(BINBASE)
	@mkdir -p $(IMGDIR)
	@mkdir -p $(FSDIR)

gdb:
	gdb -s ./bin/cboot/x86_64-elf/cboot.sys.sym -ex "target remote localhost:1234"
