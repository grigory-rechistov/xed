CC := gcc
FLEX := flex
BISON := bison
CFLAGS := -std=c99 -Wall -Werror

all: calc

lex.yy.c: calc.l calc.tab.h
	$(FLEX) --header-file=lexer.h calc.l

calc.tab.c calc.tab.h:	calc.y
	$(BISON) -d calc.y

calc: main.c lex.yy.c calc.tab.c calc.tab.h
	$(CC) $(CFLAGS) -o calc calc.tab.c lex.yy.c main.c

clean:
	rm -f calc calc.tab.c lex.yy.c calc.tab.h lexer.h *.o

test: calc
	cat test.s | ./calc
