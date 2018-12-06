CABECERA1 = ./grilla.h
CC=gcc
FLAGS= -lm -Wall -I. -pthread

all: $(CABECERA1)
	$(CC) ./main.c -o main.o $(FLAGS)

#run:
#	./test.o

clean:
	-rm -f *.o
