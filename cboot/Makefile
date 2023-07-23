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

RUSTSRC := $(shell find $(SRCBASE) -type f -name "*.rs" -not -path "*arch*") \
			$(shell find $(SRCBASE)/arch/machine/$(ARCH) -type f -name "*.rs") \
			$(shell find $(SRCBASE)/arch/cpu/$(CPU) -type f -name "*.rs")

.phony: all install clean doc run debug

all: makedirs ./target/cboot-$(CPU)/debug/$(BINARY)
	
clean:
	@rm -rf target
	@rm -rf $(BINDIR)

./target/cboot-$(CPU)/debug/$(BINARY): $(SRCBASE)/** Makefile $(SRCBASE)/arch/cpu/$(CPU)/link.ld $(RUSTSRC)
	@objcopy -O elf64-x86-64 -B i386 -I binary $(SRCBASE)/data/font.psf $(SRCBASE)/data/font.psf.o
	@$(CPU)-$(BINFORMAT)-objdump -xD $(SRCBASE)/data/font.psf.o > $(SRCBASE)/data/font.psf.o.txt
	@cargo build --target ./targets/cboot-x86_64.json
	@cp ./target/cboot-$(CPU)/debug/$(BINARY) $(BINDIR)/$(BINARY)
	@$(CPU)-$(BINFORMAT)-objdump -xD $(BINDIR)/$(BINARY) > $(BINDIR)/$(DUMPFILE)

makedirs:
	@mkdir -p $(BINDIR)
