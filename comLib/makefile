CC = g++

PROJECT = libparaShm.so

INCLUDE = -I../include

target : $(PROJECT)

$(PROJECT) : shareMemory.cpp
	$(CC) -shared -o $@ $< $(INCLUDE)
	cp $@ ../lib
	cp $@ /lib

.PHONY : clean
clean:
	rm *.so -f 