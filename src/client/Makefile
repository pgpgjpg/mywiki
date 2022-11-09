DEFAULT_PATH = /home/mobis/jpg
BIN = $(DEFAULT_PATH)/mywiki/bin
INC = $(DEFAULT_PATH)/mywiki/include/
CFLAGS = -c -g
LDFLAGS = -L$(DEFAULT_PATH)/mywiki/lib -lpthread
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