
include ../SampleApps.mak

#
# BEGIN EXTRA CHECK
# This Sample app uses glibc functions, not uclibc for MIPs
#
# END EXTRA CHECK
#
_OBJ = SMSSampleApp.o
OBJ  = $(patsubst %, $(ODIR)%, $(_OBJ))

$(ODIR)%.o:     $(SRCDIR)%.c
	@if [ -e $(ODIR) ] ;\
        then echo "$(ODIR) exists - good." ;\
        else mkdir -p $(ODIR);\
        fi;
	@echo Compiling $<

	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)SMSSampleApp$(CPU):	$(OBJ)
	@if [ -e $(BINDIR) ] ;\
        then echo "$(BINDIR) exists - good." ;\
        else mkdir -p $(BINDIR);\
        fi;
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS) 

.PHONY clean:
	rm -rf $(ODIR)
	rm -rf $(BINDIR)SMSSampleApp$(CPU)

