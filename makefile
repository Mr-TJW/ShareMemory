
.PHONY:clean target 

target : 
	make -C test

clean:
	make clean -C test
