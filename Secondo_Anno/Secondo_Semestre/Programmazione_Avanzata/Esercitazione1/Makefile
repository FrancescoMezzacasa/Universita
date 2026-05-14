CC = gcc
CFLAGS = -Wall -O3
SHARED = yes
HEADERS = ${wildcard *.h}

all: main
ifeq "${SHARED}" "no"
main: main.o libbst.a
	echo "sto facendo la statica"
	${CC} ${CFLAGS} -L. -o main main.o -lbst
else
tree.o: tree.c tree.h nodes.h
	${CC} ${CFLAGS} -fPIC -c tree.c

nodes.o: nodes.c nodes.h
	${CC} ${CFLAGS} -fPIC -c nodes.c

main: main.c libbst.so
	echo "sto facendo la dinamica"
	${CC} ${CFLAGS} -o $@ $< -L. -lbst


endif

libbst.a: tree.o nodes.o
	ar rs $@ $^

libbst.so: tree.o nodes.o
	${CC} ${CFLAGS} -o $@ -shared $^

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $<
.PHONY: clean
clean:
	rm -f *.o
	rm -f main
	rm -f *.a
	rm -f *.so