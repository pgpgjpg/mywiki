INC = /home/mobis/jpg/mywiki/include/
CFLAGS = -fPIC -c -g
LDFLAGS = -L/home/mobis/jpg/mywiki/lib -lhash -larray -ldl
OBJS = main.o finder.o
SRCS = $(OBJS:.o=.c)
CC = gcc

all : main

main : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean :
	rm *.o main