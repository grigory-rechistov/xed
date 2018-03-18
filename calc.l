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

(e|r)?[abcd]x {yylval.regname = yytext; return TOK_GPR;} /* RAX-RBX */
(e|r)?[sd]i {yylval.regname = yytext; return TOK_GPR;} /* RSI, RDI */
(e|r)?[bs]p {yylval.regname = yytext; return TOK_GPR;} /* RBP, RSP */
r[0-9]+ {yylval.regname = yytext; return TOK_GPR;} /* R8-R15, TODO limit number range */
r[0-9][lwd]+ {yylval.regname = yytext; return TOK_GPR;} /* R8L-R15D, TODO limit number range */
(e|r)?ip {yylval.regname = yytext; return TOK_GPR;} /* Not exactly a GPR */
[abcd][hl] {yylval.regname = yytext; return TOK_GPR;} /* AH-DL */
spl|bpl|sil|dil {yylval.regname = yytext; return TOK_GPR;} /* Weird 8 bit parts */

[cdefgs]s {yylval.regname = yytext; return TOK_SEG_REG;} /* Segment registers */

st[0-7] {yylval.regname = yytext; return TOK_FPU_REG;} /* x87 FPU regs */
(x|y|z)?mm[0-9]+ {yylval.regname = yytext; return TOK_VEC_REG;} /* MMX, XMM, YMM, ZMM TODO limit number range */


k[0-7] {yylval.regname = yytext; return TOK_MASK_REG;} /* opmask registers */
z {return TOK_ZEROING;} /* vector mask zeroing specifier */
er {return TOK_ER;} /* embedded rounding */
sae {return TOK_SAE;} /* suppress all exceptions */

"{1to"[0-9]+"}" {yylval.broadcastspec = yytext; return TOK_BCAST;}  /* broadcast {1to16} */
"{r"[nduz]"-sae}" {yylval.roundingspec = yytext; return TOK_ROUNDING;}  /* embedded rounding
                                                                           with suppress all exceptions {rn-sae} */

"{" {return TOK_LCUBR;} /* should go after tokens above that include curly braces */
"}" {return TOK_RCUBR;}


[+-]?0x[0-9a-f]+ {yylval.constant = yytext; return TOK_CONSTANT;} /* hexadecimal signed number */
[+-]?[0-9]+ {yylval.constant = yytext; return TOK_CONSTANT;} /* decimal signed number */

byte|([dq]?word) {yylval.memwidth = yytext; return TOK_MEMWIDTH;} /* memory width specifier */
([xyz]mmword) {yylval.memwidth = yytext; return TOK_MEMWIDTH;} /* memory width specifier for vector commands */

[a-z][a-z0-9]* {yylval.opcode = yytext; return TOK_OPCODE;} /* this rule must be the last one as it matches all words */


\n        {return T_NEWLINE;} /* TODO is it needed? */


%%

