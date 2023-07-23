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

ifeq ($(DUMPFILE),)
export DUMPFILE:= cboot.txt
endif

ifeq ($(BINARY),)
export BINARY:= cboot.elf
endif


SRCBASE:= ./src

.phony: all install clean doc run debug

all: makedirs cboot.x86_64.elf
	
clean:
	@rm -rf target
	@rm -rf $(BINDIR)

cboot.x86_64.elf: src/**
	@objcopy -O elf64-x86-64 -B i386 -I binary ./src/font.psf font.o
	@cargo build --target ./targets/cboot-x86_64.json
	@cp ./target/cboot-$(CPU)/debug/$(BINARY) $(BINDIR)/$(BINARY)
	@$(CPU)-$(BINFORMAT)-objdump -xD $(BINDIR)/$(BINARY) > $(BINDIR)/$(DUMPFILE)

makedirs:
	@mkdir -p $(BINDIR)
