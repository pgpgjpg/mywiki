EXES = main
OBJS = main.o server.o map.o array.o hash.o input.o menu.o profile.o
SRCS = $(OBJS1:.o=.c)
CC = gcc
CFLAG = -c -g

all : main

main : $(OBJS)
	$(CC) -o $@ $(OBJS) -lpthread -ldl

main.o : main.c
	$(CC) $(CFLAG) $<

server.o : server.c
	$(CC) $(CFLAG) $<

map.o : ../map/map.c
	$(CC) $(CFLAG) $<

array.o : ../../lib/array.c
	$(CC) $(CFLAG) $<

hash.o : ../../lib/hash.c
	$(CC) $(CFLAG) $<

input.o : ../../lib/input.c
	$(CC) $(CFLAG) $<

menu.o : ../../lib/menu.c
	$(CC) $(CFLAG) $<

profile.o : ../../lib/profile.c
	$(CC) $(CFLAG) $<

clean :
	rm $(EXES) $(OBJS)