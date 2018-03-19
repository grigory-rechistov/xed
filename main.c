#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "lexer.h"
#include "calc.tab.h"

int main(int argc, char **argv) {

    yyin = stdin;
    do {
        yyparse();
    } while(!feof(yyin));
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parsing error: %s\n", s);
    exit(1);
}

