#	Global Definitions
CCDIR:= ~/opt/cross/bin/
BINDIR:= ./bin/$(ARCH)/$(CPU)
SRCDIR:= ./$(ARCH)/$(CPU)/src
DEBUG:= DEBUG
VERBOSE:= NOTVERBOSE
FILESYSDIR:= ../filesystem
FILESYSBOOTDIR:= $(FILESYSDIR)/BOOTBOOT
FILESYSBINARY:= INITRD
ISODIR:= ../images
ISOFILE:= caracal.iso

#	Basic File Definitions
CPPSRC := $(shell find $(SRCDIR) -type f -name "*.cpp" -not -path "*arch*" -not -path "*include*")

ASMSRC := $(shell find $(SRCDIR) -type f -name "*.s" -not -path "*include*" -not -path "*crt*")

FONTSRC:= $(shell find $(SRCDIR) -type f -name "*.psf")

ALLSRC := $(CPPSRC) $(ASMSRC) $(NASMSRC) $(FONTSRC)

HDRFILES := $(shell find $(SRCDIRS) -type f -name "*.h" -not -path "*arch*")

SRCDIRS :=$(shell find $(SRCDIR) -type d -not -path "*arch*" -not -path "*include*")

CPPVER:=13.1.0
CRTI_OBJ=$(BINDIR)/runtime/crti.o
CRTBEGIN_OBJ:=$(shell $(CPU)-$(BINFORMAT)-gcc-$(CPPVER) $(CPPFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CPU)-$(BINFORMAT)-gcc-$(CPPVER) $(CPPFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=$(BINDIR)/runtime/crtn.o

BINDIRS := $(patsubst $(SRCDIR)%,$(BINDIR)%,$(SRCDIRS))
OBJFILES := $(patsubst $(SRCDIR)%.cpp,$(BINDIR)%.cpp.o,$(CPPSRC)) $(patsubst $(SRCDIR)%.s,$(BINDIR)%.s.o,$(ASMSRC)) \
			$(patsubst $(SRCDIR)%.asm,$(BINDIR)%.asm.o,$(NASMSRC)) $(patsubst $(SRCDIR)%.psf,$(BINDIR)%.psf.o,$(FONTSRC))
			
DEPFILES := $(patsubst %.cpp,%.cpp.d,$(CPPSRC))

#	Flags for Assembler - See arch/cpu/Makefile for specific arch-based definitions
ASFLAGS:= --fatal-warnings

#	Flags for C++ start here - See arch/cpu/Makefile for specific arch-based definitions
WFLAGS:= -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
WFLAGS+= -Wwrite-strings -Wredundant-decls -Winline -Wuninitialized -Wconversion
WFLAGS+= -Wmissing-declarations -Wno-long-long -Werror

INCLUDES:= -I $(SRCDIR)/../include -I $(SRCDIR)/../include/arch/machine -I $(SRCDIR)/../include/arch/cpu
INCLUDES+= -I $(SRCDIR)/../include/arch/cpu/$(CPU) -I $(SRCDIR)/../include/arch/machine/$(ARCH)
INCLUDES+= -I $(HOME)/opt/cross/lib/gcc/$(CPU)-$(BINFORMAT)/$(CPPVER)/include
INCLUDES+= -I $(HOME)/opt/cross/lib/gcc/$(CPU)-$(BINFORMAT)/$(CPPVER)/include-fixed
INCLUDES+= -I $(HOME)/caracal/libkernel/include -I $(HOME)/caracal/libkernel/include/arch/machine/$(ARCH)
INCLUDES+= -I $(HOME)/caracal/libkernel/include/arch/machine -I $(HOME)/caracal/libkernel/include/arch/cpu/$(CPU)
INCLUDES+= -I $(HOME)/caracal/libkernel/include/arch/cpu

CPPFLAGS:= $(WFLAGS) $(INCLUDES) -std=c++11 -O0 -pedantic -pedantic-errors -fpic
CPPFLAGS+= -nostartfiles -ffreestanding -nodefaultlibs -fno-rtti -fno-exceptions
CPPFLAGS+= -fno-builtin -nostdlib -nostdinc -fomit-frame-pointer -g
CPPFLAGS+= -D $(DEBUG) -D $(VERBOSE) -D __CPU=$(CPU) -D __ARCH=$(ARCH) -D __BINFORMAT=$(BINFORMAT)

#	Flags for LD start here
LDFLAGS:= -T $(SRCDIR)/../link.ld -nostdlib $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJFILES) $(CRTEND_OBJ) $(CRTN_OBJ)
LDADD:= --start-group -L $(HOME)/opt/cross/lib/gcc/$(CPU)-$(BINFORMAT)/$(CPPVER) -lgcc --end-group
LDADD:= --start-group -L $(HOME)/caracal/libkernel/bin/$(ARCH)/$(CPU) -lkernel --end-group

-include $(SRCDIR)/../Makefile
