CC=gcc

all: compile_all

compile_all: driver.o customcompiler.o customcompiler.h
	gcc -o compiler driver.o customcompiler.o

driver.o: driver.c
	gcc -c driver.c

customcompiler.o: customcompiler.c
	gcc -c customcompiler.c

clean:
	rm *.o
