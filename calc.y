/* Parser for assembly in Intel notation for XED */

%code requires {
#include <stdbool.h>
#include "xed-encode.h"
#include "parse-helpers.h"

// remove when done debugging
#undef YYDEBUG
#define YYDEBUG 1


#define YY_DECL int yylex(xed_encoder_request_t *req, parser_state_t *s)

int yylex(xed_encoder_request_t *req, parser_state_t *s);
void yyerror(xed_encoder_request_t *req, parser_state_t *state, const char* str);


} // code requires

%lex-param { xed_encoder_request_t *req}
%lex-param { parser_state_t *s}
%parse-param { xed_encoder_request_t *req}
%parse-param { parser_state_t *s}


%{

#include <stdio.h>
#include <stdlib.h>
#include "xed-encode.h"

//extern void yyerror(xed_encoder_request_t *req, parser_state_t *state, const char* str);

%}

%union {
//    xed_iclass_enum_t opcode;
    xed_reg_enum_t regname;
    xed_uint64_t constant;
    bool lock_seen;
    bool repe_seen;
    bool repne_seen;

    char memwidth;
    char broadcastspec;
    char roundingspec;
    char opcode_string[100];
    char garbage[100];
}

%token<opcode_string> TOK_OPCODE
%token<regname> TOK_GPR

%token TOK_REPE_PREF
%token TOK_REPNE_PREF
%token TOK_LOCK_PREF

%token TOK_VEC_REG
%token TOK_MASK_REG
%token TOK_SEG_REG
%token TOK_FPU_REG

%token TOK_CONSTANT
%token TOK_MEMWIDTH


%token T_NEWLINE
%token TOK_COMMA
%token TOK_LSQBR
%token TOK_RSQBR
%token TOK_PLUS
%token TOK_MULTI
%token TOK_COLON
%token TOK_LCUBR
%token TOK_RCUBR
%token TOK_ZEROING
%token TOK_ER
%token TOK_SAE

%token TOK_BCAST
%token TOK_ROUNDING


%token TOK_GARBAGE

%start toplevelexpr

%%

toplevelexpr:
    | toplevelexpr line
;

line: T_NEWLINE
     | asmline T_NEWLINE { printf("assembly line\n");}
;

asmline: prefixes opcode operands
;

prefixes: /* empty */
         | TOK_REPE_PREF
         | TOK_REPNE_PREF
         | TOK_LOCK_PREF
         /* TODO support multiple single-byte prefixes? */
;

opcode: TOK_OPCODE {
        xed_iclass_enum_t iclass = XED_ICLASS_INVALID;
        /* Sometimes prefixes are encoded inside iclass. We've seen all prefixes
           now and can act on them */
        decorate_opcode_mnemonic($1, sizeof($1), s);
        iclass =  str2xed_iclass_enum_t($1);
        if (iclass == XED_ICLASS_INVALID) {
            if (s->repne_seen || s->repe_seen || s->lock_seen)
                fprintf(stderr,
                "[XED CLIENT ERROR] Bad instruction name or incompatible"
                " prefixes: '%s'\n", $1);
            else
                fprintf(stderr,
                "[XED CLIENT ERROR] Bad instruction name: '%s'\n", $1);
            exit(1);
        }
        xed_encoder_request_set_iclass(req, iclass);
}
;

operands: /* no operands */
        | operand
        | operands TOK_COMMA operand
;

operand:  register
        | vector_register
        | immediate
        | lea_spec
        | mem_spec
        | vec_register_masked
        | broadcast_expr
        // | far_ptr_spec
        // | segment reg, debug reg, control reg, x87 fpu, bnd reg
        // | {kreg}, {sae}, {er}
;

register: TOK_GPR {
        xed_reg_enum_t reg_name = $1;

        if (s->dstate->mmode != XED_MACHINE_MODE_LONG_64)
            if (reg_name == XED_REG_DIL || reg_name == XED_REG_SPL
            ||  reg_name == XED_REG_BPL || reg_name == XED_REG_SIL)
        {
            fprintf(stderr,
                "[XED CLIENT ERROR] Cannot use DIL/SPL/BPL/SIL outside of 64b mode\n");
            exit(1);
        }
        


        // The registers operands are numbered starting from the first one
        // as XED_OPERAND_REG0. We increment regnum (below) every time we
        // add a register operand.
        xed_operand_enum_t reg_pos = XED_CAST(xed_operand_enum_t,
                                                XED_OPERAND_REG0 + s->regnum);
        
        /* TODO check for overflow for number of register operands */
        // store the register identifier in the operand storage field
        xed_encoder_request_set_reg(req, reg_pos, reg_name);
        
        // store the operand storage field name in the encode-order array
        xed_encoder_request_set_operand_order(req, s->operand_index, reg_pos);
        
        deduce_operand_width_gpr(req, s, reg_name);

        // find_vl(reg, &vl); FIXME reenable me
        
        s->operand_index++;
        s->regnum++;
}
;

vector_register: TOK_VEC_REG
;

immediate: TOK_CONSTANT // all types of literals
;

lea_spec: TOK_LSQBR mem_expr TOK_RSQBR { printf("lea_spec\n"); } /* LEA do not use "mem ptr" */
;

mem_spec:
      TOK_MEMWIDTH TOK_SEG_REG TOK_COLON TOK_LSQBR mem_expr TOK_RSQBR { printf("seg:memory\n"); } /* segment override */
    | TOK_MEMWIDTH TOK_LSQBR mem_expr TOK_RSQBR { printf("def:memory\n"); } /* default segment */
;


// cmp    BYTE PTR [rdx+rax*1-0x1],0xa
// mov    DWORD PTR [rbx+rdx*4+0x20],ecx
// lea    ecx,[rsi*8+0x0]
// nop    WORD PTR cs:[rax+rax*1+0x0]
// vgatherdpd zmm30{k1},qword [r14+ymm31*8+0x7b]

/* TODO support these notations: */
// enterq $0x1412,$0x5
// VADDPD zmm0 {k1},zmm1,zmm3,{rz-sae}
// vaddps zmm7 {k6}, zmm2, zmm4, {rd-sae}
//  disp8*N.
//  vpcompressd [rdi] {k1}, zmm1 - memory and masking from here: https://blogs.msdn.microsoft.com/vcblog/2017/07/11/microsoft-visual-studio-2017-supports-intel-avx-512/

// V4FMADDPS zmm1{k1}{z}, zmm2+3, m128 - a pair of zmm registers


mem_expr: TOK_GPR /* single GPR */
        | TOK_GPR TOK_PLUS TOK_GPR /* GPR + GPR (16-bit specific) */
        | TOK_GPR TOK_PLUS TOK_CONSTANT /* GPR + offset */
        | TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT /* Base + Index GPR * Scale */
        | TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT TOK_PLUS TOK_CONSTANT /* Base + Index GPR * Scale + constant */
        | vsib_mem_expr
;

vsib_mem_expr:
        | TOK_GPR TOK_PLUS TOK_VEC_REG TOK_MULTI TOK_CONSTANT /* Base + Index Vector * Scale */
        | TOK_GPR TOK_PLUS TOK_VEC_REG TOK_MULTI TOK_CONSTANT TOK_PLUS TOK_CONSTANT /* Base + Index Vector * Scale + constant */
;

vec_register_masked: TOK_VEC_REG TOK_LCUBR TOK_MASK_REG TOK_RCUBR /* zmm30 {k3}*/
                   | TOK_VEC_REG TOK_LCUBR TOK_MASK_REG TOK_RCUBR TOK_ZEROING /* zmm30 {k3} {z}*/
;

broadcast_expr: TOK_BCAST
;

%%
