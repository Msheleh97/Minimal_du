default: mdu 

# Compiler flags
CFLAGS=-pthread -g -std=gnu11 -Wall -Wextra -Wpedantic -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

# Force remove
RM=rm -f

# Main program compilation linking with child and commands modul.
mdu: mdu.o threads.o threads_free.o linkedList.o
	gcc $(CFLAGS) mdu.o threads.o threads_free.o linkedList.o -o mdu

threads_free: threads_free.c threads_free.h
	gcc $(CFLAGS) -c threads_free.c -o threads_free.o

# Separate file compilation for the command modul
threads: threads.c threads.h
	gcc $(CFLAGS) -c threads.c -o threads.o

linkedList: linkedList.c linkedList.h
	gcc $(CFLAGS) -c linkedList.c -o linkedList.o

# Clean workspace
clean:
	$(RM) main *.o
