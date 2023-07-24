#	General Makefile for all architectures
MACHINE?= pc
CPU?= x86_64
BINFORMAT?= elf
TRIPLET?= $(CPU)-$(BINFORMAT)
OBJCOPY_FORMAT?= elf64-x86-64
OBJCOPY_PLATFORM?= i386
BASEDIR?= $(shell pwd)
OBJDIR?= $(BASEDIR)/obj
BINDIR?= $(BASEDIR)/bin
IMGDIR?= $(BASEDIR)/img
CONFIGDIR?= $(BASEDIR)/config

#	System Binaries
QEMU?= qemu-system-$(CPU)
MKBOOTIMG?= ./tools/bootboot/mkbootimg
MAKE_EXPORTS:= ARCH=$(ARCH) MACHINE=$(MACHINE) CPU=$(CPU) BINFORMAT=$(BINFORMAT)
MAKE_EXPORTS+= TRIPLET=$(TRIPLET) BASEDIR=$(BASEDIR) OBJDIR=$(OBJDIR) BINDIR=$(BINDIR) 
MAKE_EXPORTS+= IMGDIR=$(IMGDIR) OBJCOPY_FORMAT=$(OBJCOPY_FORMAT) OBJCOPY_PLATFORM=$(OBJCOPY_PLATFORM)

ifndef VERBOSE
.SILENT:
endif

.phony: all install clean doc run debug boot

all: all-boot

install:
	@echo Installing...
	@cp $(BINDIR)/cboot.elf ./filesystem/sys/core
	@$(MKBOOTIMG) $(CONFIGDIR)/$(TRIPLET)-bootboot.json $(IMGDIR)/$(TRIPLET)-caracal.img
	
clean: clean-boot
	@rm -rf $(BINDIR)
	@rm -rf $(OBJDIR)
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
	@make -s -C cboot $(MAKE_EXPORTS)

clean-boot:
	@make clean -s -C cboot $(MAKE_EXPORTS)

makedirs:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(IMGDIR)

gdb:
	@make -s gdb -C kernel $(MAKE_EXPORTS)