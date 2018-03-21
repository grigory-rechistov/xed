
%option noinput
%option nounput
%option noyywrap
%option never-interactive

%{

#include <stdio.h>

int fileno(FILE *stream);

#include "calc.tab.h"

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


"REP"|"REPNE"|"REPZ"|"REPE" {yylval.prefix = yytext; return TOK_REP_PREF;}

"LOCK" {yylval.prefix = yytext; return TOK_LOCK_PREF;}

(E|R)?[ABCD]X {yylval.regname = yytext; return TOK_GPR;} /* RAX-RBX */
(E|R)?[SD]I {yylval.regname = yytext; return TOK_GPR;} /* RSI, RDI */
(E|R)?[BS]P {yylval.regname = yytext; return TOK_GPR;} /* RBP, RSP */
R[0-9]+ {yylval.regname = yytext; return TOK_GPR;} /* R8-R15, TODO limit number range */
R[0-9][LWD]+ {yylval.regname = yytext; return TOK_GPR;} /* R8L-R15D, TODO limit number range */
(E|R)?IP {yylval.regname = yytext; return TOK_GPR;} /* Not exactly a GPR */
[ABCD][HL] {yylval.regname = yytext; return TOK_GPR;} /* AH-DL */
SPL|BPL|SIL|DIL {yylval.regname = yytext; return TOK_GPR;} /* Weird 8 bit parts */

[CDEFGS]S {yylval.regname = yytext; return TOK_SEG_REG;} /* Segment registers */

ST[0-7] {yylval.regname = yytext; return TOK_FPU_REG;} /* x87 FPU regs */
(X|Y|Z)?MM[0-9]+ {yylval.regname = yytext; return TOK_VEC_REG;} /* MMX, XMM, YMM, ZMM TODO limit number range */


K[0-7] {yylval.regname = yytext; return TOK_MASK_REG;} /* opmask registers */
"{Z}" {return TOK_ZEROING;} /* vector mask zeroing specifier */
"{ER}" {return TOK_ER;} /* embedded rounding */
"{SAE}" {return TOK_SAE;} /* suppress all exceptions */

"{1TO"[0-9]+"}" {yylval.broadcastspec = yytext; return TOK_BCAST;}  /* broadcast {1to16} */
"{R"[NDUZ]"-SAE}" {yylval.roundingspec = yytext; return TOK_ROUNDING;}  /* embedded rounding
                                                                           with suppress all exceptions {rn-sae} */

"{" {return TOK_LCUBR;} /* should go after tokens above that include curly braces */
"}" {return TOK_RCUBR;}


[+-]?0X[0-9A-F]+ {yylval.constant = yytext; return TOK_CONSTANT;} /* hexadecimal signed number */
[+-]?[0-9]+ {yylval.constant = yytext; return TOK_CONSTANT;} /* decimal signed number */

BYTE|([DQ]?WORD) {yylval.memwidth = yytext; return TOK_MEMWIDTH;} /* memory width specifier */
([XYZ]MMWORD) {yylval.memwidth = yytext; return TOK_MEMWIDTH;} /* memory width specifier for vector commands */

[A-Z][A-Z0-9]* {yylval.opcode = yytext; return TOK_OPCODE;} /* this rule must be the last one as it matches all words */


\n        {return T_NEWLINE;}
<<EOF>>   {static int once = 0;
           return once++? 0: T_NEWLINE;} /* Hack to turn the first EOF into newline */

;        {yylval.garbage = yytext;
           printf("flex garbage: %s\n", yytext);
           return TOK_GARBAGE;} /* rule for unmatched input */

%%

