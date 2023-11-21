BINARY:= libkernel.a

CPPFLAGS+= -mno-red-zone -mcmodel=large
LDFLAGS+= --no-warn-rwx-segments

.PHONY:	all	makedirs clean dump dist doc

-include $(DEPFILES)

all: makedirs $(OBJFILES) Makefile
	$(CPU)-$(BINFORMAT)-ar ru $(BINDIR)/$(BINARY) $(OBJFILES)
	$(CPU)-$(BINFORMAT)-ranlib $(BINDIR)/$(BINARY)

dist:
	@rm -f $(FILESYSDIR)/$(FILESYSBINARY)
	@cp $(BINDIR)/$(BINARY) $(FILESYSBOOTDIR)/$(FILESYSBINARY)
	mkdir -p $(ISODIR)
	rm -f $(ISODIR)/$(ISOFILE)
	xorriso -as mkisofs -R -f -no-emul-boot -boot-info-table -boot-load-size 4 -e /BOOT/BOOTX64.EFI  -o $(ISODIR)/$(ISOFILE) $(FILESYSDIR)

dump:
	@$(CPU)-$(BINFORMAT)-objdump -xD $(BINDIR)/$(BINARY) > $(BINDIR)/$(DUMPFILE)

$(BINDIR)/%.cpp.o: $(SRCDIR)/%.cpp
	$(CPU)-$(BINFORMAT)-gcc-$(CPPVER) $(CPPFLAGS) -MP -MMD -c $< -o $@

$(BINDIR)/%.s.o: $(SRCDIR)/%.s
	$(CPU)-$(BINFORMAT)-as $(ASFLAGS) -o $@ $<

$(BINDIR)/%.asm.o: $(SRCDIR)/%.asm
	@nasm -o $@ -f elf64 $<

$(BINDIR)/%.psf.o: $(SRCDIR)/%.psf
	$(CPU)-$(BINFORMAT)-ld -r -b binary -o $@ $<

doc:
	@doxygen kernel.doxyfile > /dev/null

clean:
	@rm -rf $(BINDIR)
		
makedirs:
	@mkdir -p $(BINDIRS)

gdb:
	@gdb -s $(BINDIR)/$(SYMFILE) -ex "target remote localhost:1234"
