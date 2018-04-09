%{
#include <stdio.h>

#define YY_DECL int yylex()

#include "calc.tab.h"

int yywrap(void){ return 1;}

%}

%%

[ \t]   ; // ignore all whitespace
"ptr"   ; // ignore "ptr" part in memory specifiers. Not exactly correct
"," {return TOK_COMMA;}
"[" {return TOK_LSQBR;}
"]" {return TOK_RSQBR;}
"+" {return TOK_PLUS;}
"*" {return TOK_MULTI;}
":" {return TOK_COLON;}

"rep"|"repne"|"repz"|"repe" {yylval.prefix = yytext; return TOK_REP_PREF;}

"lock" {yylval.prefix = yytext; return TOK_LOCK_PREF;}

(e|r)?[abcd]x {yylval.regname = yytext; return TOK_GPR;}
(e|r)?[sd]i {yylval.regname = yytext; return TOK_GPR;}
(e|r)?[bs]p {yylval.regname = yytext; return TOK_GPR;}
r[0-9]+ {yylval.regname = yytext; return TOK_GPR;} /* TODO add all GPRs */

[cdefgs]s {yylval.regname = yytext; return TOK_SEGREG;}

(x|y|z)?mm[0-9]+ {yylval.regname = yytext; return TOK_VEC_REG;}

[+-]?0x[0-9a-f]+ {yylval.constant = yytext; return TOK_CONSTANT;}
[+-]?[0-9]+ {yylval.constant = yytext; return TOK_CONSTANT;}

byte|([dq]?word) {yylval.memwidth = yytext; return TOK_MEMWIDTH;}


[a-z][a-z0-9]* {yylval.opcode = yytext; return TOK_OPCODE;} // this rule must be the last one as it matches all words


\n        {return T_NEWLINE;}


%%

