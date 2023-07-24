SRCBASE:= ./src
TRIPLETSDIR:= ./triplets
BINARY?= cboot.elf

RUSTSRC := $(shell find $(SRCBASE) -type f -name "*.rs" -not -path "*arch*") \
			$(shell find $(SRCBASE)/arch/machine/$(ARCH) -type f -name "*.rs") \
			$(shell find $(SRCBASE)/arch/cpu/$(CPU) -type f -name "*.rs")

.phony: all install clean doc run debug

all: makedirs ./target/$(TRIPLET)-target/debug/$(BINARY)
	
clean:
	@rm -rf target

./target/$(TRIPLET)-target/debug/$(BINARY): $(SRCBASE)/** Makefile $(TRIPLETSDIR)/$(TRIPLET)-link.ld $(RUSTSRC)
	@objcopy -O $(OBJCOPY_FORMAT) -B $(OBJCOPY_PLATFORM) -I binary $(SRCBASE)/data/font.psf $(SRCBASE)/data/font.psf.o
	@cargo +nightly build --target $(TRIPLETSDIR)/$(TRIPLET)-target.json
	@cp ./target/$(TRIPLET)-target/debug/$(BINARY) $(BINDIR)/$(BINARY)

makedirs:
	@mkdir -p $(BINDIR)
