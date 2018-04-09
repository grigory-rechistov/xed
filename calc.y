%{

#include <stdio.h>
#include <stdlib.h>

#define YYDEBUG 1

extern FILE* yyin;
extern int yylex();
extern int yyparse();

void yyerror(const char* s);
%}

%union {
    char *prefix;
    char *regname;
    char *opcode;
    char *constant;
    char *memwidth;
}

%token<prefix> TOK_REP_PREF
%token<prefix> TOK_LOCK_PREF
%token<regname> TOK_GPR
%token<regname> TOK_VEC_REG
%token<opcode> TOK_OPCODE
%token<constant> TOK_CONSTANT
%token<memwidth> TOK_MEMWIDTH
%token<regname> TOK_SEGREG

%token T_NEWLINE
%token TOK_COMMA
%token TOK_LSQBR
%token TOK_RSQBR
%token TOK_PLUS
%token TOK_MULTI
%token TOK_COLON

%start toplevelexpr

%%

toplevelexpr:
    | toplevelexpr line
;

line: T_NEWLINE
    | asmline T_NEWLINE { printf("assembly line\n");}

;

asmline: TOK_OPCODE
      | TOK_REP_PREF TOK_OPCODE
      | TOK_LOCK_PREF TOK_OPCODE
      | TOK_OPCODE operands
;

operands: register
        | lea_spec
        | mem_spec
        | immediate
        | operands TOK_COMMA register
        | operands TOK_COMMA immediate
        | operands TOK_COMMA lea_spec
        | operands TOK_COMMA mem_spec
        // | far_ptr_spec
        // | segment reg, debug reg, control reg, x87 fpu, bnd reg
        // | {kreg}, {b}, {sae}, {er}
;

register: TOK_GPR
        | TOK_VEC_REG
;

immediate: TOK_CONSTANT // all types of literals
;

lea_spec: TOK_LSQBR mem_expr TOK_RSQBR { printf("lea_spec\n"); }
;


mem_spec:
      TOK_MEMWIDTH TOK_SEGREG TOK_COLON TOK_LSQBR mem_expr TOK_RSQBR { printf("seg:memory\n"); }// TODO support segment override
    | TOK_MEMWIDTH TOK_LSQBR mem_expr TOK_RSQBR { printf("def:memory\n"); }// TODO support segment override
;


// cmp    BYTE PTR [rdx+rax*1-0x1],0xa
// mov    DWORD PTR [rbx+rdx*4+0x20],ecx
// lea    ecx,[rsi*8+0x0]
// nop    WORD PTR cs:[rax+rax*1+0x0]

mem_expr: TOK_GPR
        | TOK_GPR TOK_PLUS TOK_GPR
        | TOK_GPR TOK_PLUS TOK_CONSTANT
        | TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT
        | TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT TOK_PLUS TOK_CONSTANT
        // | vsib_spec
;

%%

int main() {
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

