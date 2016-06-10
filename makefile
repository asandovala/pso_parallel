CC = gcc
CFLAGS = -I -W
DEPS = src/freq/freq.h src/pso.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: maon.o pso.o freq.o
	gcc -o main main.o pso.o freq.o -I.
