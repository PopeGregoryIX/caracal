MACHINE?= pc
CPU?= x86_64
BINFORMAT?= elf
TRIPLET?= $(CPU)-$(BINFORMAT)
OBJCOPY_FORMAT?= elf64-x86-64
OBJCOPY_PLATFORM?= i386
BASEDIR?= $(shell pwd)/../
OBJBASE?= $(BASEDIR)/obj
OBJDIR:= $(OBJBASE)/cboot/$(TRIPLET)
BINDIR?= $(BINBASE)/cboot/$(TRIPLET)
IMGDIR?= $(BASEDIR)/img
CONFIGDIR?= $(BASEDIR)/config
CPPVER?= 13.1.0
CPPDIR?= ~/opt/cross/bin
LIBDIR?= $(BINBASE)/libkernel/$(TRIPLET)
SRCDIR?= ./src
TARGETSDIR?=./targets

# binaries
AS?= $(CPPDIR)/$(TRIPLET)-as
AR?= $(CPPDIR)/$(TRIPLET)-ar
CXX?= $(CPPDIR)/$(TRIPLET)-gcc-$(CPPVER)
LD?= $(CPPDIR)/$(TRIPLET)-ld
OBJCOPY?= $(CPPDIR)/$(TRIPLET)-objcopy
OBJDUMP?= $(CPPDIR)/$(TRIPLET)-objdump
RANLIB?= $(CPPDIR)/$(TRIPLET)-ranlib

# source paths and directories
SRCDIRS :=$(shell find $(SRCDIR) -type d -not -path "*arch*" -not -path "*include*") \
			$(shell find $(SRCDIR)/arch/$(CPU) -type d -not -path "*include*") \
			$(shell find $(SRCDIR)/arch/$(MACHINE) -type d -not -path "*include*")

CPPSRC:= $(shell find $(SRCDIR) -type f -name "*.cpp" -not -path "*arch*" -not -path "*include*") \
			$(shell find $(SRCDIR)/arch/$(CPU) -type f -name "*.cpp" -not -path "*include*") \
			$(shell find $(SRCDIR)/arch/$(MACHINE) -type f -name "*.cpp" -not -path "*include*")

ASMSRC := $(shell find $(SRCDIR)/arch/$(CPU) -type f -name "*.s" -not -path "*include*" -not -path "*crti.s" -not -path "*crtn.s")

FONTSRC:= $(shell find $(SRCDIR) -type f -name "*.psf" -not -path "*arch*" -not -path "*include*")

ALLSRC := $(CPPSRC) $(ASMSRC) $(FONTSRC)

# output paths and directories
OBJDIRS := $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(SRCDIRS))
OBJFILES := $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.cpp.o,$(CPPSRC)) \
			$(patsubst $(SRCDIR)%.s,$(OBJDIR)%.s.o,$(ASMSRC)) \
			$(patsubst $(SRCDIR)%.psf,$(OBJDIR)%.psf.o,$(FONTSRC))

# C++ Runtime
CRTI_OBJ=$(OBJDIR)/arch/$(CPU)/crti.o
CRTBEGIN_OBJ:=$(shell $(CXX) $(CPPFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CXX) $(CPPFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=$(OBJDIR)/arch/$(CPU)/crtn.o

# Linker and Compiler Flags
INCLUDES:= -I $(SRCDIR)/../include -I $(SRCDIR)/../include/arch/$(CPU) -I $(SRCDIR)/../include/arch/$(MACHINE)
INCLUDES+= -I $(HOME)/opt/cross/lib/gcc/$(CPU)-$(BINFORMAT)/$(CPPVER)/include
INCLUDES+= -I $(HOME)/opt/cross/lib/gcc/$(CPU)-$(BINFORMAT)/$(CPPVER)/include-fixed

LDFLAGS:= -T $(TARGETSDIR)/$(TRIPLET)-link.ld -nostdlib $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJFILES) $(CRTEND_OBJ) $(CRTN_OBJ)
LDADD:= --start-group -L $(HOME)/opt/cross/lib/gcc/$(CPU)-$(BINFORMAT)/$(CPPVER) -lgcc --end-group
LDADD:= --start-group -L $(LIBDIR) -lkernel --end-group

WFLAGS:= -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
WFLAGS+= -Wwrite-strings -Wredundant-decls -Winline -Wuninitialized -Wconversion
WFLAGS+= -Wmissing-declarations -Wno-long-long -Werror

CPPFLAGS:= $(WFLAGS) $(INCLUDES) -std=c++11 -O0 -pedantic -pedantic-errors -fpic
CPPFLAGS+= -nostartfiles -ffreestanding -nodefaultlibs -fno-rtti -fno-exceptions
CPPFLAGS+= -fno-builtin -nostdlib -nostdinc -fomit-frame-pointer -g

-include $(TARGETSDIR)/$(TRIPLET)-Makefile