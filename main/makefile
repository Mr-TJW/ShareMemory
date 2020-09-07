CC = g++

PROJECT = main
LOCALDIR = .
#CFLAGS =  -fPIC #-w
INCLUDE = -I$(LOCALDIR)/include  \
          -I$(LOCALDIR)/../include
LIB = -lpthread -L$(LOCALDIR)/../lib/ -lparaShm

SRC = $(wildcard *.cpp *.c)
OBJ = $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRC)))  $(patsubst %.c,%.o,$(filter %.c,$(SRC)))

target : $(PROJECT)

$(PROJECT) :  $(OBJ)
	$(CC) -g -o $@ $(CFLAGS) $($(CPPFLAGS)) $^ $(INCLUDE)  $(LIB) 
	-cp $@ $(LOCALDIR)/../target

.cpp.o .c.o:
	$(CC) -g -c $^  $(INCLUDE) 

.PHONY:clean
clean:
	-rm  $(PROJECT) -f
	-rm *.o -f
#	-rm $(LOCALDIR)/../target/* -f

