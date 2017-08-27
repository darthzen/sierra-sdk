# makefile

include ../SampleApps.mak

_OBJ := fwDld_main.o fwDld_9x00.o fwDld_G3K.o fwDld_9x15.o fwDld_9x07.o
OBJ  := $(patsubst %, $(ODIR)%, $(_OBJ))

$(ODIR)%.o:	$(SRCDIR)%.c
	@if [ -e $(ODIR) ] ;\
        then echo "$(ODIR) exists - good." ;\
        else mkdir -p $(ODIR);\
        fi;
	@echo Compiling $<

	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)fwdld$(CPU): $(OBJ)
	@if [ -e $(BINDIR) ] ;\
        then echo "$(BINDIR) exists - good." ;\
        else mkdir -p $(BINDIR);\
        fi;
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY clean:
	rm -rf $(ODIR)
	rm -rf $(BINDIR)fwdld$(CPU)
