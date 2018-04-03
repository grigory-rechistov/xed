/* Parser for assembly in Intel notation for XED */

/* Do not generate unused functions */
%option noinput
%option nounput
%option noyywrap

/* Do not use istty() */
%option never-interactive

%{

#include <stdbool.h>
#include <stdio.h>

#include "xed-examples-util.h"
#include "parse-helpers.h"
#include "intel-syntax.parser.h"

/* Action for not-yet written rules */
#define DUP_YYTEXT() do {xed_strncpy(yylval.garbage, yytext, 100);} while(0)

%}

/* Definitions of some patterns */

 /* Not exactly a GPR */
PROG_CNTR (E|R)?IP

 /* RAX-RBX and their classic subparts */
GPR_1 (E|R)?[ABCD]X

 /* RSI, RDI */
GPR_2 (E|R)?[SD]I

 /* RBP, RSP */
GPR_3 (E|R)?[BS]P

 /* R8-R15, TODO limit number range */
GPR_4 R[0-9]+

 /* R8L-R15D, TODO limit number range */
GPR_5 R[0-9][LWD]+

 /* AH-DL */
GPR_6 [ABCD][HL]

 /* Weird 8-bit subparts */
GPR_7 SPL|BPL|SIL|DIL

ALLGPR {PROG_CNTR}|{GPR_1}|{GPR_2}|{GPR_3}|{GPR_4}|{GPR_5}|{GPR_6}|{GPR_7}

 /* Segment registers */
SEG_REG [CDEFGS]S

 /* Alpha-numeric sequences. Note that it also accepts underscore, even though
    Intel manuals do not have instructions with underscores in menmonics */
ALPHANUMWORD [A-Z][A-Z0-9_]*

%%

[ \t]   ; // ignore all whitespace and tabs
"," {return TOK_COMMA;}
"[" {return TOK_LSQBR;}
"]" {return TOK_RSQBR;}
"+" {return TOK_PLUS;}
"-" {return TOK_MINUS;}
"*" {return TOK_MULTI;}
":" {return TOK_COLON;}


REP|REPE|REPZ { xed_encoder_request_set_rep(req);
                 /* NOTE does not affect string instructions as presence of
                    prefixes is encoded in their iclass */
                s->repe_seen = true;
                return TOK_REPE_PREF;}
REPNE|REPNZ { xed_encoder_request_set_repne(req);
              /* NOTE does not affect string instructions as presence of
                 prefixes is encoded in their iclass */
              s->repne_seen = true;
              return TOK_REPNE_PREF;}

XACQUIRE { xed_encoder_request_set_repne(req);
              /* NOTE currently not fully supported by xed_encode() */
           s->repne_seen = true;
           return TOK_REPNE_PREF;}

XRELEASE { xed_encoder_request_set_rep(req);
           /* NOTE currently not fully supported by xed_encode() */
           s->repe_seen = true;
           return TOK_REPE_PREF;}

LOCK { /* To be encoded into iclass */
       s->lock_seen = true;
       return TOK_LOCK_PREF;}

 /* GPRs and RIP */
{ALLGPR} {
        yylval.regname = parse_single_register(yytext);
        return TOK_GPR;
}

 /* Segment registers */
{SEG_REG} {yylval.regname = parse_single_register(yytext);
           return TOK_SEG_REG;
}

 /* x87 FPU regs */
ST[0-7] {yylval.regname = parse_single_register(yytext);
         return TOK_FPU_REG;
 }

 /* MMX registers */
MM[0-7]+ {yylval.regname = parse_single_register(yytext);
          return TOK_MMX_REG;
}

 /* XMM, YMM, ZMM TODO limit number range */
(X|Y|Z)MM[0-9]+ {
            yylval.regname = parse_single_register(yytext);
            return TOK_VEC_REG;
}

 /* Control registers */
CR[0-9]+ {
            yylval.regname = parse_single_register(yytext);
            return TOK_CONTROL_REG;
}

 /* Debug registers */
DR[0-9]+ {
            yylval.regname = parse_single_register(yytext);
            return TOK_DEBUG_REG;
}

 /* Bound registers */
BND[0-9]+ {
            yylval.regname = parse_single_register(yytext);
            return TOK_BOUND_REG;
}

K[0-7]  {
            yylval.regname = parse_single_register(yytext);
            return TOK_OPMASK_REG;
}

"{Z}" {return TOK_ZEROING;} /* vector mask zeroing specifier */
"{ER}" {return TOK_ER;} /* embedded rounding */
"{SAE}" {return TOK_SAE;} /* suppress all exceptions */

"{1TO"[0-9]+"}" {DUP_YYTEXT(); return TOK_BCAST;}  /* broadcast {1to16} */
"{R"[NDUZ]"-SAE}" {DUP_YYTEXT(); return TOK_ROUNDING;}  /* embedded rounding
                                                           with suppress all exceptions {rn-sae} */

"{" {return TOK_LCUBR;} /* should go after tokens above that include curly braces */
"}" {return TOK_RCUBR;}


 /* hexadecimal signed number */
0X[0-9A-F]+ {
        const char* digits = yytext+2; /* skip "0x"*/
        yylval.literal.value = xed_strtoll(digits, 16);
        yylval.literal.width_bits = XED_CAST(unsigned int,strlen(digits)*4);
        return TOK_CONSTANT;
}


 /* decimal signed number */
[0-9]+ {
        const char* digits = yytext;
        yylval.literal.value = xed_strtoll(digits, 10);
        yylval.literal.width_bits = XED_CAST(unsigned int,strlen(digits)*4); // XXX blatantly wrong for decimals!
        return TOK_CONSTANT;
}

 /* Memory width specifiers */
 /* TODO support several spaces between 'byte' and 'ptr' */
"BYTE PTR" {
    s->memory_operand_bytes = 1;
    return TOK_MEMWIDTH;
}

"WORD PTR" {
    s->memory_operand_bytes = 2;
    return TOK_MEMWIDTH;
}

"DWORD PTR" {
    s->memory_operand_bytes = 4;
    return TOK_MEMWIDTH;
}

"QWORD PTR" {
    s->memory_operand_bytes = 8;
    return TOK_MEMWIDTH;
}

 /* Memory width specifier for vector operands  */
[XYZ]"MMWORD PTR" {
    s->memory_operand_bytes = yytext[0] == 'X'? 16:
                              yytext[0] == 'Y'? 32:
                              yytext[0] == 'Z'? 64: -1;
    printf("FIXME ?mmword ptr case not done\n");
    //xed3_operand_set_vl(req,(xed_uint_t)(yytext[0] - 'X'));
    return TOK_MEMWIDTH;
}


 /* This rule must be the last one as it matches all words */
{ALPHANUMWORD} {
                printf("lex opcode = '%s'\n", yytext);
                xed_strncpy(yylval.opcode_string, yytext,
                            sizeof(yylval.opcode_string));
                return TOK_OPCODE;}

\n        {return T_NEWLINE;}

 /* Remains of multi-line lexer */
<<EOF>>   {static int once = 0;
           return once++? 0: T_NEWLINE;} /* Hack to turn the first EOF into newline */

 /* Unrecognized symbols.
   TODO add tracking of current position for better error reporting */
.        { fprintf(stderr,
           "[XED CLIENT ERROR] Unexpected symbol: '%s'\n", yytext);
           exit(1);
           return TOK_GARBAGE;}

%%

