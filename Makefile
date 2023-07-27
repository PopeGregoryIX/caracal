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
CONFIGDIR?= $(BASEDIR)/config
CPPVER?= 13.1.0
CPPDIR?= ~/opt/cross/bin

#	System Binaries
QEMU?= qemu-system-$(CPU)
MKBOOTIMG?= ./tools/bootboot/mkbootimg


#	Make System
MAKE_EXPORTS:= ARCH=$(ARCH) MACHINE=$(MACHINE) CPU=$(CPU) BINFORMAT=$(BINFORMAT)
MAKE_EXPORTS+= TRIPLET=$(TRIPLET) BASEDIR=$(BASEDIR) BINBASE=$(BINBASE) OBJBASE=$(OBJBASE) 
MAKE_EXPORTS+= IMGDIR=$(IMGDIR)
MAKE_EXPORTS+= CPPDIR=$(CPPDIR)

ifndef VERBOSE
.SILENT:
endif

.phony: all install clean doc run debug all-boot clean-boot gdb

all: all-lib install-lib all-boot

install:
	@echo Installing...
	@cp $(BINBASE)/cboot/$(TRIPLET)/cboot.sys ./filesystem/sys/core
	@$(MKBOOTIMG) $(CONFIGDIR)/$(TRIPLET)-bootboot.json $(IMGDIR)/$(TRIPLET)-caracal.img
	
clean: clean-boot
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
	make -C cboot $(MAKE_EXPORTS)

install-boot: makedirs
	make install -C cboot $(MAKE_EXPORTS)

clean-boot:
	@make clean -s -C cboot $(MAKE_EXPORTS)

all-lib: makedirs
	make -C libkernel $(MAKE_EXPORTS)

install-lib: makedirs
	make install -C libkernel $(MAKE_EXPORTS)

makedirs:
	@mkdir -p $(OBJBASE)
	@mkdir -p $(BINBASE)
	@mkdir -p $(IMGDIR)

gdb:
	gdb -s ./bin/cboot/x86_64-elf/cboot.sys.sym -ex "target remote localhost:1234"