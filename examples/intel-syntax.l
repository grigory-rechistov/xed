/*BEGIN_LEGAL

Copyright (c) 2018 Intel Corporation

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

END_LEGAL */

/* Lexer for assembly in Intel notation for XED */

/* Do not use global variables to communicate with Bison */
%option reentrant

/* Construct equivalence classes for memory savings */
%option ecs

/* Do not expect more than one input file */
%option noyywrap

/* Describe modern interface for Bison */
%option bison-bridge
%option bison-locations
%option yylineno

/* Do not use istty() */
%option never-interactive
/* Do not generate unused functions */
%option noinput
%option nounput

/* do not expect input/output attached to stdin/stdout */
%option nostdinit

%{

#include <stdbool.h>
#include <stdio.h>

#include "xed-examples-util.h"
#include "parse-helpers.h"
#include "intel-syntax.parser.h"

/* Use RETURN_TOKEN instead of return if an action calls more scanning
   functions that may signal an error */
#define RETURN_TOKEN(tok) if (s->error_found) { \
                              s->error_position = yylloc_param->first_column; \
                              return TOK_GARBAGE; \
                          } else {return (tok);}

/* Location tracking for error reporting */
#define YY_USER_ACTION update_location_information(yylloc, yytext);

static void update_location_information(YYLTYPE *yylloc, const char *txt) {
    /* No need to track line number, only column */
    yylloc->first_column = yylloc->last_column;
    yylloc->last_column += xed_strlen(txt);
}
%}

/* Definitions of some patterns */

 /* Instruction pointers */
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

 /* Includes RIP which is not GPR */
ALLGPR {PROG_CNTR}|{GPR_1}|{GPR_2}|{GPR_3}|{GPR_4}|{GPR_5}|{GPR_6}|{GPR_7}

 /* Segment registers */
SEG_REG [CDEFGS]S

 /* Alpha-numeric sequences. Note that it also accepts underscore.
    Though SDM does not have instructions with underscores in mnemonics,
    XED accepts several such forms */
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
                return TOK_REPE_PREF;
}
REPNE|REPNZ { xed_encoder_request_set_repne(req);
              /* NOTE does not affect string instructions as presence of
                 prefixes is encoded in their iclass */
              s->repne_seen = true;
              return TOK_REPNE_PREF;
}
XACQUIRE { xed_encoder_request_set_repne(req);
              /* NOTE currently not fully supported by xed_encode() */
           s->repne_seen = true;
           return TOK_REPNE_PREF;
}
XRELEASE { xed_encoder_request_set_rep(req);
           /* NOTE currently not fully supported by xed_encode() */
           s->repe_seen = true;
           return TOK_REPE_PREF;
}

LOCK { /* To be encoded into iclass */
       s->lock_seen = true;
       return TOK_LOCK_PREF;
}

 /* GPRs and RIP */
{ALLGPR} {
        yylval->regname = parse_single_register(s, yytext);
        RETURN_TOKEN(TOK_GPR);
}

 /* Segment registers */
{SEG_REG} {
        yylval->regname = parse_single_register(s, yytext);
        RETURN_TOKEN(TOK_SEG_REG);
}


 /* x87 FPU regs */
 /* XXX XED recognizes only lowercase names for x87 registers: st(0) - st(7) */

ST {
        yylval->regname = parse_single_register(s, "st(0)"); /* Top of stack */
        RETURN_TOKEN(TOK_FPU_REG);
}

 /* Intel SDM variant */
ST"("[0-7]")" {
        char mangled_name[] = "st(0)";
        mangled_name[3] += yytext[3] - '0';
        yylval->regname = parse_single_register(s, mangled_name);
        RETURN_TOKEN(TOK_FPU_REG);
}

 /* Variants without braces: ST0-ST7 */
ST[0-7] {
        char mangled_name[] = "st(0)";
        mangled_name[3] += yytext[2] - '0';
        yylval->regname = parse_single_register(s, mangled_name);
        RETURN_TOKEN(TOK_FPU_REG);
}

 /* MMX registers, XED notation */
MMX[0-7]+ {
        yylval->regname = parse_single_register(s, yytext);
        RETURN_TOKEN(TOK_MMX_REG);
}

 /* Proper names from SDM: MM0-MM7 */
MM[0-7]+ {
        char mangled_name[] = "MMX0";
        mangled_name[3] += yytext[2] - '0';
        yylval->regname = parse_single_register(s, mangled_name);
        RETURN_TOKEN(TOK_MMX_REG);
}


 /* XMM, YMM, ZMM */
(X|Y|Z)MM[0-9]+ {
            yylval->regname = parse_single_register(s, yytext);
            RETURN_TOKEN(TOK_VEC_REG);
}

 /* Control registers */
CR[0-9]+ {
            yylval->regname = parse_single_register(s, yytext);
            RETURN_TOKEN(TOK_CONTROL_REG);
}

 /* Debug registers */
DR[0-9]+ {
            yylval->regname = parse_single_register(s, yytext);
            RETURN_TOKEN(TOK_DEBUG_REG);
}

 /* Bound registers */
BND[0-9]+ {
            yylval->regname = parse_single_register(s, yytext);
            RETURN_TOKEN(TOK_BOUND_REG);
}

K[0-7]  {
            yylval->regname = parse_single_register(s, yytext);
            RETURN_TOKEN(TOK_OPMASK_REG);
}

"{Z}" {return TOK_ZEROING;} /* vector mask zeroing specifier */

 /* broadcast {1toX}. TODO it ignores actual number of items */
"{1TO"[0-9]+"}" {return TOK_BCAST;}

 /* suppress all exceptions {sae} */
"SAE" {
        yylval->rounding_mode = -1; /* not specified */
        return TOK_SAE_ROUNDING;
}

 /* embedded rounding with suppress all exceptions {rne-sae} etc. */
"R"("NE"|D|U|Z)"-SAE" {
        /* Use EVEX.L'L encoding */
        switch (yytext[1]) {
        case 'N':
            yylval->rounding_mode = 0;
            break;
        case 'D':
            yylval->rounding_mode = 1;
            break;
        case 'U':
            yylval->rounding_mode = 2;
            break;
        case 'Z':
            yylval->rounding_mode = 3;
            break;
        default:
            s->error_found = 1;
            break;
        }
        RETURN_TOKEN(TOK_SAE_ROUNDING);
}

"{" {return TOK_LCUBR;} /* should go after tokens above that include curly braces */
"}" {return TOK_RCUBR;}

 /* hexadecimal signed number */
0X[0-9A-F]+ {
        const char* digits = yytext+2; /* skip "0x" */
        yylval->literal.value = xed_strtoll(digits, 16); /* TODO overflow detection? */
        yylval->literal.width_bits = XED_CAST(unsigned int, strlen(digits)*4);
        RETURN_TOKEN(TOK_CONSTANT);
}


 /* decimal signed number */
[0-9]+ {
        const char* digits = yytext;
        yylval->literal.value = xed_strtoll(digits, 10); /* TODO overflow detection? */
        yylval->literal.width_bits = XED_CAST(unsigned int, strlen(digits)*4); // XXX blatantly wrong for decimals!
        RETURN_TOKEN(TOK_CONSTANT);
}

 /* far control flow instructions may have this standalone word */
 "FAR" {
     /* TODO decoder returns operand width = 48 for jmp far, but there
        is no API to encode it */
     // xed_encoder_request_set_effective_operand_width(req, 48);
     RETURN_TOKEN(TOK_FAR);
}

 /* Memory width specifiers */
 /* TODO support several spaces between 'byte' and 'ptr' */

"MEM PTR" {
    //s->memory_operand_bytes = 0; // ???
    RETURN_TOKEN(TOK_MEMWIDTH);
}

 /* MASM notation for embedded broadcast */
"BCAST PTR" {
    xed3_set_generic_operand(req, XED_OPERAND_BCAST, 1);
    RETURN_TOKEN(TOK_MEMWIDTH);
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
                              yytext[0] == 'Z'? 64:
                              (s->error_found = 1, -1);

    RETURN_TOKEN(TOK_MEMWIDTH);
}


 /* This rule must follow after all keywords as it matches all words */
{ALPHANUMWORD} {
                xed_strncpy(yylval->opcode_string, yytext,
                            sizeof(yylval->opcode_string));
                RETURN_TOKEN(TOK_OPCODE);
}

 /* Any unrecognized symbol */
.        { fprintf(stderr,"[XED CLIENT ERROR] Unexpected symbol '%s' at %d\n",
                        yytext,
                        yylloc_param->first_column);
           s->error_found = 1;
           RETURN_TOKEN(TOK_GARBAGE);
}

%%

