objects = source/abi.o source/abi_bf.o

abi : $(objects)
	cc -o abi $(objects)

abi.o : include/abi_bf.h

abi_bf.o : include/abi_bf.h


.PHONY : clean
	rm abi $(objects)

