INC = ../../include
CFLAGS = -fPIC -c -g
OBJS = menu.o profile.o input.o map.o
TARGET = libmenus.so libhash.so libarray.so libprofile.so libinput.so

all: $(TARGET)

libmenus.so: $(OBJS) 
	$(LD) -shared -o $@ $(OBJS)
	cp $@ ../../lib/
#	rm $@ $(OBJS)

libhash.so: hash.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
	rm $@ $<

libarray.so: array.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
	rm $@ $<

libprofile.so: profile.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
	rm $@ $<

libinput.so: input.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
	rm $@ $<

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean:
	rm *.o lib*.so ../../lib/libmenus.so