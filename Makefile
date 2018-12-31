CC = gcc
CFLAGS = -Wall -std=c99 -g

all: ultraposh

ultraposh: ultraposh.o ultraposhLib.a
	$(CC) $(CFLAGS) -o ultraposh ultraposh.o -L. ultraposhLib.a

ultraposhLib.a: change_my_dir.o execute_command.o help.o history.o
	ar rcs ultraposhLib.a change_my_dir.o execute_command.o help.o history.o

ultraposh.o: ultraposh.c ultraposh.h change_my_dir.c help.c history.c
	$(CC) $(CFLAGS) -c ultraposh.c

change_my_dir.o: change_my_dir.c
	$(CC) $(CFLAGS) -c change_my_dir.c

execute_command.o: execute_command.c
	$(CC) $(CFLAGS) -c execute_command.c

help.o: help.c
	$(CC) $(CFLAGS) -c help.c

history.o: history.c
	$(CC) $(CFLAGS) -c history.c

clean:
	rm -f ultraposh *.a *.o
