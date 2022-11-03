INC = ../../include
CFLAGS = -fPIC -c -g
LDFLAGS = -L/home/mobis/jpg/mywiki/lib -lhash -larray -ldl
EXES = main
OBJS = main.o finder.o
SRCS = $(OBJS:.o=.c)
CC = gcc

all : main $(TARGET)

main : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean :
	rm *.o lib*.so main