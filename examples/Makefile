FLEX := flex
BISON := bison

all: parser-files

RESULT = intel-syntax.lexer.c intel-syntax.lexer.h intel-syntax.parser.c intel-syntax.parser.h

parser-files: $(RESULT)

intel-syntax.lexer.c: intel-syntax.l intel-syntax.parser.h
	$(FLEX) -d --header-file=intel-syntax.lexer.h --outfile=intel-syntax.lexer.c intel-syntax.l

intel-syntax.parser.c intel-syntax.parser.h: intel-syntax.y
	$(BISON) --output=intel-syntax.parser.c --defines -t intel-syntax.y

clean:
	rm -f $(RESULT)