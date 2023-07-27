MACHINE?= pc
CPU?= x86_64
BINFORMAT?= elf
TRIPLET?= $(CPU)-$(BINFORMAT)
OBJCOPY_FORMAT?= elf64-x86-64
OBJCOPY_PLATFORM?= i386
BASEDIR?= $(shell pwd)/../
OBJBASE?= $(BASEDIR)/obj
OBJDIR?= $(OBJBASE)/libkernel/$(TRIPLET)
BINDIR?= $(BINBASE)/libkernel/$(TRIPLET)
IMGDIR?= $(BASEDIR)/img
CONFIGDIR?= $(BASEDIR)/config
CPPVER?= 13.1.0
CPPDIR?= ~/opt/cross/bin

SRCDIR?= ./src
TARGETSDIR?=./targets

# binaries
AS?= $(CPPDIR)/$(TRIPLET)-as
AR?= $(CPPDIR)/$(TRIPLET)-ar
CXX= $(CPPDIR)/$(TRIPLET)-gcc-$(CPPVER)
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

CPPFLAGS:= $(WFLAGS) $(INCLUDES) -std=c++11 -O0 -pedantic -pedantic-errors -fpic
CPPFLAGS+= -nostartfiles -ffreestanding -nodefaultlibs -fno-rtti -fno-exceptions
CPPFLAGS+= -fno-builtin -nostdlib -nostdinc -fomit-frame-pointer -g

-include $(TARGETSDIR)/$(TRIPLET)-Makefile
