# makefile
#-------------------------------------------------------------------------------
# if CPU is not specified, default to i386
# otherwise, accept only i386 or arm9 or ppc
#
# For arm9 and powerpc, platform specific toolchains are used.
# These must be installed to the $(HOME) directory once and can be used for all
# SDK workspaces. If they do not exist,
#   make CPU=arm9
#   make CPU=ppc
# will simply fail.
#
#-------------------------------------------------------------------------------

# default to i386, set others as required
#
CC = gcc

ifeq ($(strip $(CPU)),)
  CPU = i386
endif
ifeq ($(CPU),arm)
  CC      := $(HOME)/toolchain/arm/bin/arm-none-linux-gnueabi-gcc
  IDIR    := -I$(HOME)/toolchain/arm/arm-none-linux-gnueabi/libc/usr/include
  LDFLAGS := -L$(HOME)/toolchain/arm/lib/gcc/arm-none-linux-gnueabi/4.2.0/
  CFLAGS  := -c -D ARM
endif
#
# If LTIB (Linux Target Image Build) is installed from
# the Freescale installation DVD, use:
# /opt/freescale/usr/local/gcc-4.3.74-eglibc-2.8.74-dp-2/powerpc-none-linux-gnuspe/
# or a similar path, otherwise the toolchain should be copied to:
#  $(HOME)/toolchain/ppc/...
#
ifeq ($(CPU),ppc)
  CC      := $(HOME)/toolchain/ppc/powerpc-none-linux-gnuspe/bin/powerpc-none-linux-gnuspe-gcc
  IDIR    := -I$(HOME)/toolchain/ppc/powerpc-none-linux-gnuspe/powerpc-none-linux-gnuspe/libc/usr/include
  LDFLAGS := -L$(HOME)/toolchain/ppc/powerpc-none-linux-gnuspe/powerpc-none-linux-gnuspe/libc/usr/lib
  CFLAGS  := -c -D PPC
endif

#
# Downloaded from:
# http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/lite-edition
# Note: the original file is an installer file.
#       This is an unzipped tarballed copy of the installed files.
# Note: uclibc is used here. Some applications may need libc and is overridden
#       in the make file for those.
#
ifeq ($(CPU),mips)
  CC            := $(HOME)/toolchain/mips/bin/mips-linux-gnu-gcc
  IDIR          := -I.. $(QMI_INCLUDE) -I$(HOME)/toolchain/mips/mips-linux-gnu/libc/ulibc/usr/include
  LDFLAGS       := -L$(HOME)/toolchain/mips/mips-linux-gnu/libc/uclibc/lib -muclibc
  CFLAGS        := -c -D MIPS
endif

ifeq ($(CPU),mipsel)
  CC            := $(HOME)/toolchain/mips/bin/mips-linux-gnu-gcc
  IDIR          := -I.. $(QMI_INCLUDE) -I$(HOME)/toolchain/mips/mips-linux-gnu/libc/ulibc/usr/include
  LDFLAGS       := -L$(HOME)/toolchain/mips/mips-linux-gnu/libc/uclibc/lib -muclibc -EL
  CFLAGS        := -c -D MIPSEL -EL
endif

#raspberry pi
ifeq ($(CPU),rpi)
  CC := $(HOME)/tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-gcc
endif

CFLAGS  = -I $(IDIR) -static -lrt -lpthread
VPATH   =
SRCDIR  = src/
IDIR    = .
ODIR    = obj/$(CPU)/
LDIR    =
BINDIR  = bin/

_OBJ = remserial.o stty.o split-sqf.o
OBJ  = $(patsubst %, $(ODIR)%, $(_OBJ))

all: $(BINDIR)remserial$(CPU) $(BINDIR)split-sqf$(CPU)

$(ODIR)%.o: 	$(SRCDIR)%.c $(DEPS)
	@if [ ! -d $(ODIR) ] ;\
        then mkdir -p $(ODIR);\
        fi;
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)remserial$(CPU): $(ODIR)/remserial.o $(ODIR)/stty.o
	@if [ ! -d $(BINDIR) ] ;\
        then mkdir -p $(BINDIR);\
	fi;

	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(BINDIR)split-sqf$(CPU): $(ODIR)/split-sqf.o
	@if [ ! -d $(BINDIR) ] ;\
        then mkdir -p $(BINDIR);\
	fi;

	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY clean:
	rm -fr ./obj ./bin
