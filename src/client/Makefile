EXES = main
OBJS = main.o client.o
SRCS = $(OBJS1:.o=.c)
CC = gcc
CFLAG = -c -g

all : main

main : $(OBJS)
	$(CC) -o $@ $(OBJS) -lpthread

main.o : main.c
	$(CC) $(CFLAG) $<

client.o : client.c
	$(CC) $(CFLAG) $<

clean :
	rm $(EXES) $(OBJS)