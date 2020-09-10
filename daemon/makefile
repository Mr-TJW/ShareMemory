CC = g++

PROJECT = taskd

LIB = -lphread -L../lib/ -lpublic -lparaShm

INCLUDE = -I./include -I../include

CLFLAGS = -fPIC -W

SRC = $(wildcard %.c) $(wildcard %.cpp)
OBJ = $(patsubst %.cpp,%.o,$(filter %.c,$(SRC))) $(patsubst %.c,%.o,$(filter %.cpp,$(SRC)))

target : $(PROJECT)

$(PROJECT) : $(OBJ)
	$(CC) $(CLFLAGS) -o $@ $^ $(INCLUDE) $(LIB) 
	-cp $@ ../target

.c.o .cpp.o :
	$(CC) -c $^ (INCLUDE)

.PHONY : clean 
clean : 
	-rm -f $(PROJECT) 