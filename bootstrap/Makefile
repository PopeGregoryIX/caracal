.phony: all install clean doc run debug

SRCBASE:= ./src
BINDIR:= ./target/pc-x86_64/debug

all:
	@cargo build

clean:
	@rm -rf target

dump:
	@$(CPU)-$(BINFORMAT)-objdump -xD $(BINDIR)/bootstrap.elf > $(BINDIR)/bootstrap.txt
