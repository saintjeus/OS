all: program1.c
	gcc -g -Wall -o program1 program1.c

clean: 
	rm program1
