#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "lexer.h"
#include "calc.tab.h"

int main(int argc, char **argv) {

    if (argc >1 ) {
    char* input = argv[1];
    YY_BUFFER_STATE buffer = yy_scan_string(input);
    yyparse();
    yy_delete_buffer(buffer);

    } else {
        yyin = stdin;
        do {
            yyparse();
        } while(!feof(yyin));
    }
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parsing error: %s\n", s);
    exit(1);
}

