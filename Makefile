all: myshell mypipe looper Printers

myshell: myshell.c LineParser.o
	gcc -Wall -g -m32 -o myshell myshell.c LineParser.o

mypipe: mypipe.c
	gcc -Wall -g -m32 -o mypipe mypipe.c

looper: looper.c
	gcc -Wall -g -m32 -o looper looper.c

Printers: Printers.c
	gcc -Wall -g -m32 -o Printers Printers.c

LineParser.o: LineParser.c LineParser.h
	gcc -Wall -g -m32 -c LineParser.c

clean:
	rm -f myshell mypipe looper Printers LineParser.o
