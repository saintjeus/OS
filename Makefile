# Makefile for CS474-program 1
# Jesus Barba - 9/9/21

all: program1.c
	gcc -g -Wall -o program1 program1.c

clean: 
	rm program1
