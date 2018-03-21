#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "calc.tab.h"
#include "lexer.h"

int main(int argc, char **argv) {

    xed_encoder_request_t *req = NULL;
    if (argc >1 ) {
        char* input = argv[1];
        YY_BUFFER_STATE buffer = yy_scan_string(input);
        yyparse(req);
        yy_delete_buffer(buffer);
    } else {
        yyin = stdin;
        do {
            yyparse(req);
        } while(!feof(yyin));
    }
    return 0;
}

void yyerror(xed_encoder_request_t *req, const char* s) {
    fprintf(stderr, "Parsing error: %s\n", s);
    exit(1);
}

