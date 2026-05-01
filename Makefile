all: myshell looper Printers

myshell: myshell.c LineParser.o
	gcc -Wall -g -o myshell myshell.c LineParser.o

looper: looper.c
	gcc -Wall -g -o looper looper.c

Printers: Printers.c
	gcc -Wall -g -o Printers Printers.c

LineParser.o: LineParser.c LineParser.h
	gcc -Wall -g -c LineParser.c

clean:
	rm -f myshell looper Printers LineParser.o
