CFLAGS=-ggdb -Wall -Wextra -pedantic -Werror -O3
LIBS=$(shell pkg-config --libs raylib)

frac: frac.c
	cc -o frac frac.c $(CFLAGS) $(LIBS)

run: frac
	./frac
