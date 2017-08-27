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
CC      := gcc -g
CPU     := i386
SYMBOLS := ON
APPIDIR := inc/
VPATH   :=
SRCDIR  := src/
BINDIR  := bin/

ifeq ($(CPU),arm)
  CC      := $(HOME)/toolchain/arm/bin/arm-none-linux-gnueabi-gcc
  ARMIDIR := -I$(HOME)/toolchain/arm/arm-none-linux-gnueabi/libc/usr/include
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
  PPCIDIR := $(QMI_INCLUDE) -I$(HOME)/toolchain/ppc/powerpc-none-linux-gnuspe/powerpc-none-linux-gnuspe/libc/usr/include
  LDFLAGS := -L$(HOME)/toolchain/ppc/powerpc-none-linux-gnuspe/powerpc-none-linux-gnuspe/libc/usr/lib
  CFLAGS  := -c -D PPC
endif

ifeq ($(CPU),mips)
  CROSS_COMPILE := $(HOME)/toolchain/mips/bin/mips-linux-gnu-
  INCLUDE       += -I$(HOME)/toolchain/mips/mips-linux-gnu/libc/ulibc/usr/include
  LDFLAGS       += -L$(HOME)/toolchain/mips/mips-linux-gnu/libc/uclibc/lib -muclibc
  #default mips toolchain from Sourcery does not support sscanf
  CFLAGS        += -D OS_NO_SSCANF
endif

ifeq ($(CPU),mipsel)
  CROSS_COMPILE := $(HOME)/toolchain/mips/bin/mips-linux-gnu-
  INCLUDE       += -I$(HOME)/toolchain/mips/mips-linux-gnu/libc/ulibc/usr/include
  LDFLAGS       += -L$(HOME)/toolchain/mips/mips-linux-gnu/libc/uclibc/lib -muclibc -EL
  CFLAGS        += -EL
  #default mips toolchain from Sourcery does not support sscanf
  CFLAGS        += -D OS_NO_SSCANF
endif

LIBS    =  -static -L $(LDIR) -lpthread -lrt
LDIR    := ../../build/lib/$(CPU)/ $(LDFLAGS)
ODIR    := obj/$(CPU)/
CFLAGS  := -Wall $(CFLAGS) $(ARMIDIR) $(PPCIDIR)

_OBJ = serialcomm.o common.o crc.o ramdumptool.o
OBJ  := $(patsubst %, $(ODIR)%, $(_OBJ))

$(ODIR)%.o:	$(SRCDIR)%.c
	@if [ -e $(ODIR) ] ;\
        then echo "$(ODIR) exists - good." ;\
        else mkdir -p $(ODIR);\
        fi;
	@echo Compiling $<

	$(CC) -c -o $@ $< $(CFLAGS) -I $(APPIDIR)

$(BINDIR)ramdumptool$(CPU): $(OBJ)
	@if [ -e $(BINDIR) ] ;\
        then echo "$(BINDIR) exists - good." ;\
        else mkdir -p $(BINDIR);\
        fi;
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY clean:
	rm -f $(ODIR)*.o *~ $(BINDIR)*
	rmdir $(ODIR) $(BINDIR) --parents
