/* Parser for assembly in Intel notation for XED */
/* TODO: make lexer reentrant: https://www.lemoda.net/c/reentrant-parser/ */

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

 /* R8-R15, */
GPR_4 R[0-9]+

 /* R8L-R15D */
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
 /* XXX Xed recognizes only lowercase names for x87 registers: st(0) - st(7) */

ST {yylval.regname = parse_single_register("st(0)"); /* Top of stack */
         return TOK_FPU_REG;
}

 /* Intel SDM variant */
ST"("[0-7]")" {
        char mangled_name[] = "st(0)";
        mangled_name[3] += yytext[3] - '0';
        yylval.regname = parse_single_register(mangled_name);
        return TOK_FPU_REG;
}

 /* Variants without braces: ST0-ST7 */
ST[0-7] {
        char mangled_name[] = "st(0)";
        mangled_name[3] += yytext[2] - '0';
        yylval.regname = parse_single_register(mangled_name);
        return TOK_FPU_REG;
}

 /* MMX registers, XED notation */
MMX[0-7]+ {
        yylval.regname = parse_single_register(yytext);
        return TOK_MMX_REG;
}

 /* Proper names from SDM: MM0-MM7 */
MM[0-7]+ {
        char mangled_name[] = "MMX0";
        mangled_name[3] += yytext[2] - '0';
        yylval.regname = parse_single_register(mangled_name);
        return TOK_MMX_REG;
}


 /* XMM, YMM, ZMM */
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

 /* broadcast {1toX}. TODO it ignores actual number of items */
"{1TO"[0-9]+"}" {return TOK_BCAST;}

"{R"[NDUZ]"-SAE}" {DUP_YYTEXT(); return TOK_ROUNDING;}  /* embedded rounding
                                                           with suppress all exceptions {rn-sae} */

"{" {return TOK_LCUBR;} /* should go after tokens above that include curly braces */
"}" {return TOK_RCUBR;}

 /* hexadecimal signed number */
0X[0-9A-F]+ {
        const char* digits = yytext+2; /* skip "0x" */
        yylval.literal.value = xed_strtoll(digits, 16);
        yylval.literal.width_bits = XED_CAST(unsigned int, strlen(digits)*4);
        return TOK_CONSTANT;
}


 /* decimal signed number */
[0-9]+ {
        const char* digits = yytext;
        yylval.literal.value = xed_strtoll(digits, 10);
        yylval.literal.width_bits = XED_CAST(unsigned int,strlen(digits)*4); // XXX blatantly wrong for decimals!
        return TOK_CONSTANT;
}

 /* far control flow instructions may have this standalone word */
 "FAR" {
     /* TODO decoder returns operand width = 48 for jmp far, but there
        is no API to encode it */
     // xed_encoder_request_set_effective_operand_width(req, 48);
     return TOK_FAR;
}

 /* Memory width specifiers */
 /* TODO support several spaces between 'byte' and 'ptr' */

"MEM PTR" {
    //s->memory_operand_bytes = 0; // ???
    return TOK_MEMWIDTH;
}

 /* MASM notation for embedded broadcast */
"BCAST PTR" {
    xed3_set_generic_operand(req, XED_OPERAND_BCAST, 1);
    return TOK_MEMWIDTH;
}


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
    return TOK_MEMWIDTH;
}


 /* This rule must be the last one as it matches all words */
{ALPHANUMWORD} {
                xed_strncpy(yylval.opcode_string, yytext,
                            sizeof(yylval.opcode_string));
                return TOK_OPCODE;}

 /* Any unrecognized symbol
   TODO add tracking of current position for better error reporting */
.        { fprintf(stderr,
           "[XED CLIENT ERROR] Unexpected symbol: '%s'\n", yytext);
           s->error_found = 1;
           return TOK_GARBAGE;
}

%%

