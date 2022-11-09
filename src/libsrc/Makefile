DEFAULT_PATH = /home/mobis/jpg
INC = $(DEFAULT_PATH)/mywiki/include
LIB = $(DEFAULT_PATH)/mywiki/lib
SRC = $(DEFAULT_PATH)/mywiki/src
CFLAGS = -fPIC -c -g
OBJS = menu.o profile.o input.o $(SRC)/map/map.o
TARGET = libmenus.so libhash.so libarray.so libprofile.so libinput.so

all: $(TARGET)

libmenus.so: $(OBJS) 
	$(LD) -shared -o $@ $(OBJS)
	cp $@ $(LIB)
#	rm $@ $(OBJS)

libhash.so: hash.o 
	$(LD) -shared -o $@ $<
	cp $@ $(LIB)
	rm $@ $<

libarray.so: array.o 
	$(LD) -shared -o $@ $<
	cp $@ $(LIB)
	rm $@ $<

libprofile.so: profile.o 
	$(LD) -shared -o $@ $<
	cp $@ $(LIB)
	rm $@ $<

libinput.so: input.o 
	$(LD) -shared -o $@ $<
	cp $@ $(LIB)
	rm $@ $<

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean:
	rm *.o lib*.so $(LIB)/*.so