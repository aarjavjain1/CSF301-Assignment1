CC=gcc

all: compile_all

compile_all: driver.o
	gcc -o compiler driver.o

driver.o: driver.c
	gcc -c driver.c

clean:
	rm *.o
