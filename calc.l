/* Parser for assembly in Intel notation for XED */

/* Do not generate unused functions */
%option noinput
%option nounput
%option noyywrap

/* Do not use istty() */
%option never-interactive

%{

#include <stdio.h>

#include "calc.tab.h"

#define DUP_YYTEXT() do {xed_strncpy(yylval.garbage, yytext, 100);} while(0)

%}

%%

[ \t]   ; // ignore all whitespace and tabs
"PTR"   ; // ignore "ptr" part in memory specifiers. Not exactly correct
"," {return TOK_COMMA;}
"[" {return TOK_LSQBR;}
"]" {return TOK_RSQBR;}
"+" {return TOK_PLUS;}
"*" {return TOK_MULTI;}
":" {return TOK_COLON;}


REP|REPE|REPZ { xed_encoder_request_set_rep(req);
                return TOK_REPE_PREF;}
REPNE|REPNZ { xed_encoder_request_set_repne(req);
                return TOK_REPNE_PREF;}
LOCK { printf("FIXME lock prefix not handled\n");
      return TOK_LOCK_PREF;}

 /* RAX-RBX and their classic subparts */
(E|R)?[ABCD]X { xed_reg_enum_t reg = str2xed_reg_enum_t(yytext);
                if (reg == XED_REG_INVALID) {
                    fprintf(stderr,
                        "[XED CLIENT ERROR] Bad register name: %s\n", yytext);
                    exit(1);
                }
                yylval.regname = reg;
                return TOK_GPR;}

(E|R)?[SD]I {DUP_YYTEXT(); return TOK_GPR;} /* RSI, RDI */
(E|R)?[BS]P {DUP_YYTEXT(); return TOK_GPR;} /* RBP, RSP */
R[0-9]+ {DUP_YYTEXT(); return TOK_GPR;} /* R8-R15, TODO limit number range */
R[0-9][LWD]+ {DUP_YYTEXT(); return TOK_GPR;} /* R8L-R15D, TODO limit number range */
(E|R)?IP {DUP_YYTEXT(); return TOK_GPR;} /* Not exactly a GPR */
[ABCD][HL] {DUP_YYTEXT(); return TOK_GPR;} /* AH-DL */
SPL|BPL|SIL|DIL {DUP_YYTEXT(); return TOK_GPR;} /* Weird 8 bit parts */

[CDEFGS]S {DUP_YYTEXT(); return TOK_SEG_REG;} /* Segment registers */

ST[0-7] {DUP_YYTEXT(); return TOK_FPU_REG;} /* x87 FPU regs */
(X|Y|Z)?MM[0-9]+ {DUP_YYTEXT(); return TOK_VEC_REG;} /* MMX, XMM, YMM, ZMM TODO limit number range */


K[0-7] {DUP_YYTEXT(); return TOK_MASK_REG;} /* opmask registers */
"{Z}" {return TOK_ZEROING;} /* vector mask zeroing specifier */
"{ER}" {return TOK_ER;} /* embedded rounding */
"{SAE}" {return TOK_SAE;} /* suppress all exceptions */

"{1TO"[0-9]+"}" {DUP_YYTEXT(); return TOK_BCAST;}  /* broadcast {1to16} */
"{R"[NDUZ]"-SAE}" {DUP_YYTEXT(); return TOK_ROUNDING;}  /* embedded rounding
                                                           with suppress all exceptions {rn-sae} */

"{" {return TOK_LCUBR;} /* should go after tokens above that include curly braces */
"}" {return TOK_RCUBR;}


[+-]?0X[0-9A-F]+ {DUP_YYTEXT(); return TOK_CONSTANT;} /* hexadecimal signed number */
[+-]?[0-9]+ {DUP_YYTEXT(); return TOK_CONSTANT;} /* decimal signed number */

BYTE|([DQ]?WORD) {DUP_YYTEXT(); return TOK_MEMWIDTH;} /* memory width specifier */
([XYZ]MMWORD) {DUP_YYTEXT(); return TOK_MEMWIDTH;} /* memory width specifier for vector commands */


 /* This rule must be the last one as it matches all words */
[A-Z][A-Z0-9]* {
                xed_iclass_enum_t iclass = XED_ICLASS_INVALID;
                printf("lex opcode = '%s'\n", yytext);
                iclass =  str2xed_iclass_enum_t(yytext);
                if (iclass == XED_ICLASS_INVALID) {
                    fprintf(stderr,
                    "[XED CLIENT ERROR] Bad instruction name: '%s'\n", yytext);
                    exit(1);
            }
            xed_encoder_request_set_iclass(req, iclass);
            yylval.opcode = iclass;
            return TOK_OPCODE;}


\n        {return T_NEWLINE;}
<<EOF>>   {static int once = 0;
           return once++? 0: T_NEWLINE;} /* Hack to turn the first EOF into newline */

;        { DUP_YYTEXT();
           printf("flex garbage: %s\n", yytext);
           return TOK_GARBAGE;} /* rule for unmatched input */

%%

