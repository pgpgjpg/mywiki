INC = ../../include
CFLAGS = -fPIC -c -g
LDFLAGS = -L/home/mobis/jpg/mywiki/lib -lprofile -lhash -linput -larray -ldl
EXES = main
OBJS = main.o db.o
SRCS = $(OBJS:.o=.c)
CC = gcc

all : main $(TARGET)

main : $(OBJS)
	$(CC) -o $@ $(OBJS) ../finder/finder.o $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean :
	rm *.o lib*.so main