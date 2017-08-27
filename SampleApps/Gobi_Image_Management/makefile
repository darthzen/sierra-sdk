# makefile

include ../SampleApps.mak

_OBJ := gobidispmngmt.o gobiimgmgmt.o
OBJ  := $(patsubst %, $(ODIR)%, $(_OBJ))

$(ODIR)%.o:	$(SRCDIR)%.c
	@if [ -e $(ODIR) ] ;\
        then echo "$(ODIR) exists - good." ;\
        else mkdir -p $(ODIR);\
        fi;
	@echo Compiling $<

	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)gobiimgmgmt$(CPU): $(OBJ)
	@if [ -e $(BINDIR) ] ;\
        then echo "$(BINDIR) exists - good." ;\
        else mkdir -p $(BINDIR);\
        fi;
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY clean:
	rm -rf $(ODIR) 
	rm -rf $(BINDIR)gobiimgmgmt$(CPU)
