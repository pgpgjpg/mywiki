DEFAULT_PATH = /home/mobis/jpg
INC = $(DEFAULT_PATH)/mywiki/include/
CFLAGS = -fPIC -c -g
LDFLAGS = -L$(DEFAULT_PATH)/mywiki/lib -lmenus -lhash -lpthread -ldl
EXES = main
OBJS = main.o map.o
SRCS = $(OBJS:.o=.c)
CC = gcc

all : main $(TARGET)

main : $(OBJS)
	$(CC) -o $@ $(OBJS) 

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean :
	rm *.o main