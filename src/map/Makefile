INC = ../../include
CFLAGS = -fPIC -c -g
LDFLAGS = -L/home/mobis/jpg/mywiki/lib -lmenus -lhash -lpthread -ldl
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
	rm *.o lib*.so main