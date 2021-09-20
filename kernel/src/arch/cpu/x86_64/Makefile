BINARY:= kernel.sys

CPPFLAGS+= -mno-red-zone -mcmodel=large

.PHONY:	all	makedirs clean dump dist

-include $(DEPFILES)

all: makedirs $(OBJFILES) Makefile
	$(CPU)-$(BINFORMAT)-ld $(LDFLAGS) -o $(BINDIR)/$(BINARY) $(LDADD)
	@$(CPU)-$(BINFORMAT)-objdump -xD $(BINDIR)/$(BINARY) > $(BINDIR)/$(BINARY).txt

dist:
	@rm -f $(FILESYSDIR)/$(FILESYSBINARY)
	@cp $(BINDIR)/$(BINARY) $(FILESYSBOOTDIR)/$(FILESYSBINARY)
	mkdir -p $(ISODIR)
	rm -f $(ISODIR)/$(ISOFILE)
	xorriso -as mkisofs -R -f -no-emul-boot -o $(ISODIR)/$(ISOFILE) $(FILESYSDIR)

dump:
	@$(CPU)-$(BINFORMAT)-objdump -xD $(BINDIR)/$(BINARY) > $(BINDIR)/$(BINARY).txt

$(BINDIR)/%.cpp.o: $(SRCDIR)/%.cpp
	@$(CPU)-$(BINFORMAT)-gcc-$(CPPVER) $(CPPFLAGS) -MP -MMD -c $< -o $@
	@x86_64-elf-objdump -xD $@ > $@.txt
	
$(BINDIR)/%.s.o: $(SRCDIR)/%.s
	@$(CPU)-$(BINFORMAT)-as $(ASFLAGS) -o $@ $<
	@x86_64-elf-objdump -xD $@ > $@.txt

$(BINDIR)/%.psf.o: $(SRCDIR)/%.psf
	@x86_64-elf-ld -r -b binary -o $@ $<
	@x86_64-elf-objdump -xD $@ > $@.txt

clean:
	@rm -rf $(BINDIR)
		
makedirs:
	@mkdir -p $(BINDIRS)