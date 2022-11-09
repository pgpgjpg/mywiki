CC = gcc
DEFAULT_PATH = /home/mobis/jpg
INC = $(DEFAULT_PATH)/mywiki/include
CFLAGS = -fPIC -c -g
LDFLAGS = -L$(DEFAULT_PATH)/mywiki/lib -lprofile -lhash -linput -larray -ldl
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