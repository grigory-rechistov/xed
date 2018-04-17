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

/* Parser for assembly in Intel notation for XED */

/* TODO add support and tests for the following syntax.
 - unspecified memory width "mem ptr" without word/dword etc.: xsave, sgdt etc.
 - disp8*N.
 - invlpg, invpcid, invept etc
 - VCMPPD k1 {k2}, zmm2, zmm3/m512/m64bcst{sae}, imm8 <- k1{k2}
 - string operations with segment overrides
 - shifts with constant/CL
 - V4FMADDPS zmm1{k1}{z}, zmm2+3, m128 - a pair of zmm registers 2+3
*/

/* Produce reentrant parser */
%define api.pure full

/* Track locations */
%locations

%lex-param {void* lexer_state}
%lex-param {xed_encoder_request_t *req}
%lex-param {parser_state_t *s}

%parse-param {void* lexer_state}
%parse-param {xed_encoder_request_t *req}
%parse-param {parser_state_t *s}

// %define parse.error verbose /* Only works with newer Bison versions */
%code requires {
#include <stdbool.h>
#include "xed-encode.h"
#include "parse-helpers.h"

/* Define scanner function for lexer */
#define YY_DECL int yylex ( YYSTYPE * yylval_param, YYLTYPE * yylloc_param, \
      yyscan_t yyscanner, xed_encoder_request_t *req, parser_state_t *s)

/* Epilogue for every non-empty action that might signal a parsing error 
   in functions that it calls. Used for location tracking.
   loc points to the first bad token, typically @1 */
#define HANDLE_ERROR(loc) if (s->error_found) { \
                              s->error_position = loc.first_column; \
                              YYABORT; \
                          }

} // code requires

%{

#include <stdio.h>
#include <stdlib.h>
#include "xed-encode.h"
#include "intel-syntax.parser.h"
#include "intel-syntax.lexer.h"

int yylex (YYSTYPE * yylval_param, YYLTYPE * yylloc_param , yyscan_t yyscanner,
           xed_encoder_request_t *req, parser_state_t *s);
static void yyerror(YYLTYPE *locp, void *lexer_state,
            xed_encoder_request_t *req, parser_state_t *state, const char* msg);

%}

/* Structure to keep tokens' semantic values */
%union {
    xed_reg_enum_t regname;
    struct {
        xed_uint64_t value;
        unsigned int width_bits;
    } literal;

    int rounding_mode;
    char opcode_string[100]; /* saved here for late mangling */
}

%token<opcode_string> TOK_OPCODE

%token TOK_REPE_PREF
%token TOK_REPNE_PREF
%token TOK_LOCK_PREF

%token<regname> TOK_GPR
%token<regname> TOK_VEC_REG
%token<regname> TOK_SEG_REG
%token<regname> TOK_FPU_REG
%token<regname> TOK_MMX_REG
%token<regname> TOK_CONTROL_REG
%token<regname> TOK_DEBUG_REG
%token<regname> TOK_BOUND_REG
%token<regname> TOK_OPMASK_REG

%token<literal> TOK_CONSTANT
%token TOK_MEMWIDTH

%token TOK_FAR
%token TOK_COMMA
%token TOK_LSQBR
%token TOK_RSQBR
%token TOK_PLUS
%token TOK_MINUS
%token TOK_MULTI
%token TOK_COLON
%token TOK_LCUBR
%token TOK_RCUBR
%token TOK_ZEROING
%token TOK_BCAST
%token<rounding_mode> TOK_SAE_ROUNDING

%token TOK_GARBAGE /* user by lexer to report problems */

%start asmline

%%

asmline: prefixes opcode operands
;

prefixes: /* empty*/
         | prefixes prefix

prefix:    TOK_REPE_PREF
         | TOK_REPNE_PREF
         | TOK_LOCK_PREF
;

opcode: TOK_OPCODE {
        fill_mnemonic_opcode(req, s, $1, sizeof($1));
        HANDLE_ERROR(@1);
}
      | TOK_OPCODE TOK_FAR { /* far call/ret/jmp */
        fill_mnemonic_opcode(req, s, $1, sizeof($1));
        s->seen_far_ptr = true;
        HANDLE_ERROR(@1);
};

operands: /* no operands */
        | operand
        | operands TOK_COMMA operand
;

operand:  general_purpose_register
        | vector_register
        | control_register
        | debug_register
        | bound_register
        | fpu_or_mmx_register
        | opmask_register
        | literal_const
        | signed_literal
        | far_pointer
        | lea_spec
        | mem_spec
        | vec_register_filtered
;

general_purpose_register: TOK_GPR {
        fill_register_operand(req, s, $1);
        HANDLE_ERROR(@1);
};

vector_register: TOK_VEC_REG {
        fill_register_operand(req, s, $1);
        deduce_operand_width_vector(req, s, $1);
        HANDLE_ERROR(@1);
};

control_register: TOK_CONTROL_REG {
        fill_register_operand(req, s, $1);
        s->seen_cr = true;
        HANDLE_ERROR(@1);
};

debug_register: TOK_DEBUG_REG {
        fill_register_operand(req, s, $1);
        s->seen_dr = true;
        HANDLE_ERROR(@1);
};

bound_register: TOK_BOUND_REG {
        fill_register_operand(req, s, $1);
        HANDLE_ERROR(@1);
};

fpu_or_mmx_register: TOK_FPU_REG {
        fill_register_operand(req, s, $1);
        HANDLE_ERROR(@1);
}
                   | TOK_MMX_REG {
        fill_register_operand(req, s, $1);
        HANDLE_ERROR(@1);
};

opmask_register: TOK_OPMASK_REG {
        fill_register_operand(req, s, $1);
        HANDLE_ERROR(@1);
};

 /* Either immediate or relative branch displacement */
literal_const: TOK_CONSTANT {
        if (instr_category_uses_rel_branch(s->early_category))
            fill_relative_offset_operand(req, s, $1.value, $1.width_bits);
        else
            fill_immediate_operand(req, s, $1.value, $1.width_bits);
        HANDLE_ERROR(@1);
};

signed_literal: TOK_MINUS TOK_CONSTANT {
        if (instr_category_uses_rel_branch(s->early_category))
            fill_relative_offset_operand(req, s, -$2.value, $2.width_bits);
        else
            fill_immediate_operand(req, s, -$2.value, $2.width_bits);
        HANDLE_ERROR(@1);
}
              | TOK_PLUS TOK_CONSTANT {
        if (instr_category_uses_rel_branch(s->early_category))
            fill_relative_offset_operand(req, s, +$2.value, $2.width_bits);
        else
            fill_immediate_operand(req, s, +$2.value, $2.width_bits);
        HANDLE_ERROR(@1);
};

far_pointer: TOK_CONSTANT TOK_COLON TOK_CONSTANT {
       fill_far_pointer_operand(req, s, $1.value, $1.width_bits, $3.value, $3.width_bits);
       s->seen_far_ptr = true;
       HANDLE_ERROR(@1);
}
          | TOK_CONSTANT TOK_COLON TOK_MINUS TOK_CONSTANT {
       /* Negate the offset */
       fill_far_pointer_operand(req, s, $1.value, $1.width_bits, -$4.value, $4.width_bits);
       s->seen_far_ptr = true;
       HANDLE_ERROR(@1);
};

lea_spec: TOK_LSQBR agen_expr TOK_RSQBR { /* LEA does not have "mem ptr" */
        fill_agen_operand(req, s);
        HANDLE_ERROR(@2);
};

mem_spec:  segment_override_mem_spec
         | default_segment_mem_spec
;

segment_override_mem_spec: TOK_MEMWIDTH segment_override TOK_LSQBR mem_expr TOK_RSQBR broadcast_modifier {
        fill_memory_operand(req, s);
        HANDLE_ERROR(@4);
};

default_segment_mem_spec: TOK_MEMWIDTH TOK_LSQBR mem_expr TOK_RSQBR broadcast_modifier {
        fill_memory_operand(req, s);
        HANDLE_ERROR(@3);
};

segment_override: TOK_SEG_REG TOK_COLON {
    s->segment_reg = $1;
    // TODO s->segno ++;
    HANDLE_ERROR(@1);
};

 /* AGEN does not have VSIB variant compared to memory */
agen_expr:indirect_addr_gpr
        | indirect_addr_gpr_plus_offset
        | bx_bp_si_di
        | base_index_scale
        | base_index_scale_disp
;

mem_expr: indirect_addr_gpr
        | indirect_addr_gpr_plus_offset
        | bx_bp_si_di
        | base_index_scale
        | base_index_scale_disp
        | vsib_mem_expr
;

/* single indirect GPR */
indirect_addr_gpr: TOK_GPR {
        s->base_reg = $1;
        HANDLE_ERROR(@1);
};

/* GPR +/- const offset */
indirect_addr_gpr_plus_offset: TOK_GPR TOK_PLUS TOK_CONSTANT {
        s->base_reg = $1;
        s->disp_valid = 1;
        s->disp_val = $3.value;
        s->disp_width_bits = $3.width_bits;
        HANDLE_ERROR(@1);
}
                            | TOK_GPR TOK_MINUS TOK_CONSTANT {
        s->base_reg = $1;
        s->disp_valid = 1;
        s->disp_val = XED_CAST(xed_uint64_t, -$3.value); /* Use negative constant */
        s->disp_width_bits = $3.width_bits;
        HANDLE_ERROR(@1);

};

 /* BX/BP + SI/DI (16-bit specific) */
bx_bp_si_di: TOK_GPR TOK_PLUS TOK_GPR {
        s->base_reg = $1;
        s->index_reg = $3;
        HANDLE_ERROR(@1);
};

 /* Base + Index GPR * Scale */
base_index_scale: TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT {
        s->base_reg = $1;
        s->index_reg = $3;
        s->scale_val = $5.value;
        /* TODO: accept only valid numerical scale factors? */
        HANDLE_ERROR(@1);
};

 /* Base + Index GPR * Scale +/- constant */
base_index_scale_disp: TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT TOK_PLUS TOK_CONSTANT {
        s->base_reg = $1;
        s->index_reg = $3;
        s->scale_val = $5.value;
        s->disp_valid = 1;
        s->disp_val = $7.value;
        s->disp_width_bits = $7.width_bits;
        HANDLE_ERROR(@1);
}
                     | TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT TOK_MINUS TOK_CONSTANT {
        s->base_reg = $1;
        s->index_reg = $3;
        s->scale_val = $5.value;
        s->disp_valid = 1;
        s->disp_val = XED_CAST(xed_uint64_t, -$7.value); /* Use negative constant */
        s->disp_width_bits = $7.width_bits;
        HANDLE_ERROR(@1);
};


vsib_mem_expr: base_vec_index_scale
             | base_vec_index_scale_disp
;

base_vec_index_scale: TOK_GPR TOK_PLUS TOK_VEC_REG TOK_MULTI TOK_CONSTANT {
        s->base_reg = $1;
        s->index_reg = $3;
        s->scale_val = $5.value;
        HANDLE_ERROR(@1);
};

base_vec_index_scale_disp: TOK_GPR TOK_PLUS TOK_VEC_REG TOK_MULTI TOK_CONSTANT TOK_PLUS TOK_CONSTANT {
        s->base_reg = $1;
        s->index_reg = $3;
        s->scale_val = $5.value;
        s->disp_valid = 1;
        s->disp_val = $7.value;
        s->disp_width_bits = $7.width_bits;
        HANDLE_ERROR(@1);
}
                         | TOK_GPR TOK_PLUS TOK_VEC_REG TOK_MULTI TOK_CONSTANT TOK_MINUS TOK_CONSTANT {
        s->base_reg = $1;
        s->index_reg = $3;
        s->scale_val = $5.value;
        s->disp_valid = 1;
        s->disp_val = XED_CAST(xed_uint64_t, -$7.value); /* Use negative constant */
        s->disp_width_bits = $7.width_bits;
        HANDLE_ERROR(@1);
};

vec_register_filtered: vec_register_masked
                     | vec_register_masked_zeroed
                     | vec_register_sae
;

 /* zmm30 {k3} */
vec_register_masked: TOK_VEC_REG TOK_LCUBR TOK_OPMASK_REG TOK_RCUBR {
        /* TODO check that used only once */
        fill_register_operand(req, s, $1); // main register
        deduce_operand_width_vector(req, s, $1);
        fill_register_operand(req, s, $3); // opmask register
        HANDLE_ERROR(@1); // TODO separate the action into two to improve location tracking
};

 /* zmm30 {k3} {z} */
vec_register_masked_zeroed: TOK_VEC_REG TOK_LCUBR TOK_OPMASK_REG TOK_RCUBR TOK_ZEROING {
        /* TODO check that used only once */
        fill_register_operand(req, s, $1); // main register
        deduce_operand_width_vector(req, s, $1);
        fill_register_operand(req, s, $3); // opmask register
        xed3_set_generic_operand(req, XED_OPERAND_ZEROING, 1);
        HANDLE_ERROR(@1); // TODO separate the action into two to improve location tracking
};


vec_register_sae: TOK_VEC_REG TOK_LCUBR sae_modifiers TOK_RCUBR {
        fill_register_operand(req, s, $1); // main register
        deduce_operand_width_vector(req, s, $1);
        HANDLE_ERROR(@1);
};

 /* {sae} and {rn|d|u|z-sae} */
sae_modifiers: TOK_SAE_ROUNDING {
        xed3_set_generic_operand(req, XED_OPERAND_SAE, 1);
        if ($1 >= 0) {
            unsigned xed_rnding = $1+1; /* XED shifts it */
            xed3_set_generic_operand(req, XED_OPERAND_ROUNDC, xed_rnding);
        }
        HANDLE_ERROR(@1);
};

 /* 1toX memory broadcast */
broadcast_modifier: /* empty */
                  | TOK_BCAST {
        xed3_set_generic_operand(req, XED_OPERAND_BCAST, 1);
        HANDLE_ERROR(@1);
};

%%

/* The function is called when YYABORT happened or parser found syntax error */
static void yyerror(YYLTYPE *locp, void *lexer_state,
             xed_encoder_request_t *req, parser_state_t *state, const char* msg)
{
    state->error_found = 1;
    if (locp) {
        fprintf(stderr, "[XED_CLIENT_ERROR] Scanner parsing error:"
                        " %s (symbols [%d:%d])\n",
                        msg,
                        locp->first_column,
                        locp->last_column);
        state->error_position = locp->first_column;
    } else {
        fprintf(stderr, "[XED_CLIENT_ERROR] Scanner parsing error: %s\n", msg);
    }
}
