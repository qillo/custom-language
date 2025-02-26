.SUFFIXES:

.SILENT:

Generador: main.o Sintact.tab.o semantico.o
	gcc -o Generador main.o Sintact.tab.o semantico.o

semantico.o: semantico.c
	gcc -c -o semantico.o semantico.c

Sintact.tab.o: Sintact.tab.c
	gcc -c Sintact.tab.c

main.o: main.c
	gcc -c main.c

Sintact.tab.c: Sintact.y lex.yy.c
	bison -t -v -d -o Sintact.tab.c Sintact.y

lex.yy.c: lex
	flex -l lex

clean:
	rm -f Generador main.o Sintact.tab.o Sintact.tab.c lex.yy.c Sintact.tab.h Sintact.output semantico.o codigo.c

all:
	make clean
	make sintact

