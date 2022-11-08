BIN = /home/mobis/jpg/mywiki/bin
SRC = /home/mobis/jpg/mywiki/src
INC = /home/mobis/jpg/mywiki/include/ -I$(SRC)/map/ -I$(SRC)/db/ -I$(SRC)/finder/
CFLAGS = -fPIC -c -g
LDFLAGS = -L/home/mobis/jpg/mywiki/lib -lmenus -lhash -lpthread -larray -ldl
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
	rm *.o server $(BIN)/server