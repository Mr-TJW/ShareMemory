CC = g++

INCLUDE = -I../include

target : libparaShm.so libpublic.so

libparaShm.so : shareMemory.cpp
	$(CC) -shared -o $@ $< $(INCLUDE)
	cp $@ ../lib
	cp $@ /lib

libpublic.so : public.cpp
	$(CC) -shared -o $@ $< $(INCLUDE)
	cp $@ ../lib
	cp $@ /lib

.PHONY : clean
clean:
	rm *.so -f 