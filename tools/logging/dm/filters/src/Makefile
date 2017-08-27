CC=gcc
CFLAGS=-I.
DEPS = crc.h
OBJ = ba.o crc.o load.o save.o test.o edit.o
LIB = -L ./
%.o: $(SRCDIR)%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
    
libsqf.a: $(OBJ)
	ar rc $@ $(OBJ)
	ranlib $@

test:
	$(CC) -o $@ $(CFLAGS) -lsqf $(LIB)

clean:
	rm -f *.a *.o test
