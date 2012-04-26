CFLAGS=-O2 -g
CPPFLAGS=

.PHONY: clean all

all: mbchk.c multiboot.h
	gcc $(CFLAGS) mbchk.c -o mbchk
clean:
	rm -f *.o mbchk
