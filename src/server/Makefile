DEFAULT_PATH = /home/mobis/jpg
BIN = $(DEFAULT_PATH)/mywiki/bin
SRC = $(DEFAULT_PATH)/mywiki/src
INC = $(DEFAULT_PATH)/mywiki/include/ -I$(SRC)/map/ -I$(SRC)/db/ -I$(SRC)/finder/
CFLAGS = -fPIC -c -g
LDFLAGS = -L$(DEFAULT_PATH)/mywiki/lib -lmenus -lhash -lpthread -larray -ldl
OBJS = main.o server.o $(SRC)/db/db.o $(SRC)/finder/finder.o
SRCS = $(OBJS:.o=.c)
CC = gcc

all : server $(TARGET)

server : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	cp $@ $(BIN)

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean :
	rm *.o server