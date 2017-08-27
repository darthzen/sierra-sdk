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

# Get version tag
#$(shell svn info > mkver.txt)
#VERSION_TAG := $(shell python mkver.py)
VERSION_TAG := 1.0
#$(shell	rm mkver.txt)

# default to i386, set others as required
#
CC      := gcc -g -DVERSION_TAG='"$(VERSION_TAG)"'
CPU     := i386
SYMBOLS := ON
APPIDIR := inc/
VPATH   :=
SRCDIR  := src/
BINDIR  := bin/

ifeq ($(CPU),arm9)
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

ifeq ($(CPU),rspro)
	CC := /home/calvin/o/openwrt-15-05/staging_dir/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-uclibc-gcc
endif

# Compile for windows on Cygwin
ifeq ($(CPU),win32)
$(shell cp cygwindll/* $(BINDIR))
# The window registry handler requires g++ to compile/link since
# windows registry code uses C++ format
  CC      := g++ -g -DVERSION_TAG='"$(VERSION_TAG)"'
  WLIBS    := -lsetupapi
  CFLAGS  := -m32 -DRAMDUMP_WIN32
  _OBJ    := serialcomm_windows.o registry_windows.o
endif

LIBS    =  $(WLIBS) -static -L $(LDIR) -lpthread -lrt
LDIR    := ../../build/lib/$(CPU)/ $(LDFLAGS)
ODIR    := obj/$(CPU)/
CFLAGS  := -Wall $(CFLAGS) $(ARMIDIR) $(PPCIDIR)

_OBJ := serialcomm.o common.o crc.o ramdumptool.o $(_OBJ)
OBJ  := $(patsubst %, $(ODIR)%, $(_OBJ))

$(ODIR)%.o:	$(SRCDIR)%.c
	@if [ -e $(ODIR) ] ;\
        then echo "$(ODIR) exists - good." ;\
        else mkdir -p $(ODIR);\
        fi;
	@echo Compiling $<

	$(CC) -c -o $@ $< $(CFLAGS) -I $(APPIDIR)

$(ODIR)%.o:	$(SRCDIR)%.cpp
	@if [ -e $(ODIR) ] ;\
        then echo "$(ODIR) exists - good." ;\
        else mkdir -p $(ODIR);\
        fi;
	@echo Compiling $<

	$(CC) -c -o $@ $< $(CFLAGS) -I $(APPIDIR)

$(BINDIR)ramdumptool_$(CPU): $(OBJ)
	@if [ -e $(BINDIR) ] ;\
        then echo "$(BINDIR) exists - good." ;\
        else mkdir -p $(BINDIR);\
        fi;
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY clean:
	rm -f $(ODIR)*.o *~ $(BINDIR)*
