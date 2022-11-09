DEFAULT_PATH = /home/mobis/jpg
SRC = $(DEFAULT_PATH)/mywiki/src
INC = $(DEFAULT_PATH)/mywiki/include
CFLAGS = -fPIC -c -g
LDFLAGS = -L$(DEFAULT_PATH)/mywiki/lib -lprofile -lhash -linput -larray -ldl
EXES = main
OBJS = main.o db.o
SRCS = $(OBJS:.o=.c)
CC = gcc

all : main

main : $(OBJS)
	$(CC) -o $@ $(OBJS) $(SRC)/finder/finder.o $(LDFLAGS)	

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean :
	rm *.o main