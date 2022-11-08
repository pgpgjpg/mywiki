CC = gcc
INC = /home/mobis/jpg/mywiki/include
CFLAGS = -fPIC -c -g
LDFLAGS = -L/home/mobis/jpg/mywiki/lib -lprofile -lhash -linput -larray -ldl
TARGET = map libsrc finder db client server
.PHONY : $(TARGET)

all : $(TARGET)

map :
	cd $@; make
libsrc :
	cd $@; make
finder :
	cd $@; make
db :
	cd $@; make
client :	
	cd $@; make
server :
	cd $@; make

clean :
	cd client; make clean
	cd db; make clean
	cd finder; make clean
	cd libsrc; make clean
	cd map; make clean
	cd server; make clean