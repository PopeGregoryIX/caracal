BINARY:= kernel.sys
SYMFILE:= kernel.sym
DUMPFILE:= kernel.txt

CPPFLAGS+= -mno-red-zone -mcmodel=large
LDFLAGS+= --no-warn-rwx-segments

.PHONY:	all	makedirs clean dump dist doc

-include $(DEPFILES)

all: makedirs $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(CRTEND_OBJ) $(CRTN_OBJ) $(OBJFILES) Makefile
	@$(CPU)-$(BINFORMAT)-ld $(LDFLAGS) -o $(BINDIR)/$(BINARY) $(LDADD)
	@$(CPU)-$(BINFORMAT)-objdump -xD $(BINDIR)/$(BINARY) > $(BINDIR)/$(DUMPFILE)
	@$(CPU)-$(BINFORMAT)-objcopy --only-keep-debug $(BINDIR)/$(BINARY) $(BINDIR)/$(SYMFILE)
	@$(CPU)-$(BINFORMAT)-objcopy --strip-debug $(BINDIR)/$(BINARY)

dist:
	@rm -f $(FILESYSDIR)/$(FILESYSBINARY)
	@cp $(BINDIR)/$(BINARY) $(FILESYSBOOTDIR)/$(FILESYSBINARY)
	mkdir -p $(ISODIR)
	rm -f $(ISODIR)/$(ISOFILE)
	xorriso -as mkisofs -R -f -no-emul-boot -boot-info-table -boot-load-size 4 -e /BOOT/BOOTX64.EFI  -o $(ISODIR)/$(ISOFILE) $(FILESYSDIR)

dump:
	@$(CPU)-$(BINFORMAT)-objdump -xD $(BINDIR)/$(BINARY) > $(BINDIR)/$(DUMPFILE)

$(BINDIR)/%.cpp.o: $(SRCDIR)/%.cpp
	@$(CPU)-$(BINFORMAT)-gcc-$(CPPVER) $(CPPFLAGS) -MP -MMD -c $< -o $@
	
$(BINDIR)/runtime/crti.o: $(SRCDIR)/runtime/crti.s
	@$(CPU)-$(BINFORMAT)-as $(ASFLAGS) -o $@ $<

$(BINDIR)/runtime/crtn.o: $(SRCDIR)/runtime/crtn.s
	@$(CPU)-$(BINFORMAT)-as $(ASFLAGS) -o $@ $<

$(BINDIR)/%.s.o: $(SRCDIR)/%.s
	@$(CPU)-$(BINFORMAT)-as $(ASFLAGS) -o $@ $<

$(BINDIR)/%.asm.o: $(SRCDIR)/%.asm
	@nasm -o $@ -f elf64 $<

$(BINDIR)/%.psf.o: $(SRCDIR)/%.psf
	@x86_64-elf-ld -r -b binary -o $@ $<

doc:
	@doxygen kernel.doxyfile > /dev/null

clean:
	@rm -rf $(BINDIR)
		
makedirs:
	@mkdir -p $(BINDIRS)