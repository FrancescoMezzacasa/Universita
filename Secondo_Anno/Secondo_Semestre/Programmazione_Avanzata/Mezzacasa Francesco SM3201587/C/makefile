#FRANCESCO MEZZACASA SM3201587
CC = gcc
CFLAGS = -g -O3 -Wall -std=c17 -pedantic -fopenmp
HEADERS = ${wildcard *.h}
OBJECTS = main.o tensor.o stack.o operations.o interpreter.o

all: tensorforth

tensorforth: ${OBJECTS} ${HEADERS}
	${CC} ${CFLAGS} ${OBJECTS} -o $@

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $<


.PHONY: all clean
clean:
	rm -f *.o
	rm -f tensorforth