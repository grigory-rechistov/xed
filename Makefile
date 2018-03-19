FLEX := flex
BISON := bison

all: calc

lex.yy.c: calc.l calc.tab.h
	$(FLEX) calc.l

calc.tab.c calc.tab.h:	calc.y
	$(BISON) --debug -d calc.y

calc: main.c lex.yy.c calc.tab.c calc.tab.h
	$(CC) -o calc calc.tab.c lex.yy.c main.c

clean:
	rm -f calc calc.tab.c lex.yy.c calc.tab.h

test: calc
	cat test.s | ./calc
