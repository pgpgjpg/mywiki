BIN = /home/mobis/jpg/mywiki/bin
INC = /home/mobis/jpg/mywiki/include/
CFLAGS = -c -g
LDFLAGS = -L/home/mobis/jpg/mywiki/lib -lpthread
OBJS = main.o client.o
SRCS = $(OBJS:.o=.c)
CC = gcc

all : client

client : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	cp $@ $(BIN)

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean :
	rm *.o client