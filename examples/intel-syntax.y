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

// %define parse.error verbose /* Only works with newer Bison versions */


%{

#include <stdio.h>
#include <stdlib.h>
#include "xed-encode.h"

%}

%union {
    xed_reg_enum_t regname;
    struct {
        xed_uint64_t value;
        unsigned int width_bits;
    } literal;

    char memwidth;
    char roundingspec;
    char opcode_string[100];
    char garbage[100];
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
%token TOK_ER
%token TOK_SAE

%token TOK_BCAST
%token TOK_ROUNDING

%token TOK_GARBAGE

%start asmline

%%

asmline: prefixes opcode operands
;

prefixes: /* empty */
         | TOK_REPE_PREF
         | TOK_REPNE_PREF
         | TOK_LOCK_PREF
         /* TODO support multiple single-byte prefixes? */
;

opcode: TOK_OPCODE {
        fill_mnemonic_opcode(req, s, $1);
}
      | TOK_OPCODE TOK_FAR { /* far call/ret/jmp */
        fill_mnemonic_opcode(req, s, $1);
        s->seen_far_ptr = true;
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
        | negative_literal
        | far_pointer
        | lea_spec
        | mem_spec
        | vec_register_filtered
        //  x87 fpu, bnd reg
        // | {sae}, {er}
;

general_purpose_register: TOK_GPR {
        fill_register_operand(req, s, $1);
};

vector_register: TOK_VEC_REG {
        fill_register_operand(req, s, $1);
        deduce_operand_width_vector(req, s, $1);
};

control_register: TOK_CONTROL_REG {
        fill_register_operand(req, s, $1);
        s->seen_cr = true;
};

debug_register: TOK_DEBUG_REG {
        fill_register_operand(req, s, $1);
        s->seen_dr = true;
};

bound_register: TOK_BOUND_REG {
        fill_register_operand(req, s, $1);
};

fpu_or_mmx_register: TOK_FPU_REG {
        fill_register_operand(req, s, $1);
}
                   | TOK_MMX_REG {
        fill_register_operand(req, s, $1);
};

opmask_register: TOK_OPMASK_REG {
        fill_register_operand(req, s, $1);
};

literal_const: TOK_CONSTANT {
        fill_immediate_operand(req, s, $1.value, $1.width_bits);
};

negative_literal: TOK_MINUS TOK_CONSTANT {
        fill_immediate_operand(req, s, -$2.value, $2.width_bits);
};

far_pointer: TOK_CONSTANT TOK_COLON TOK_CONSTANT {
       fill_far_pointer_operand(req, s, $1.value, $1.width_bits, $3.value, $3.width_bits);
       s->seen_far_ptr = true;
}
          | TOK_CONSTANT TOK_COLON TOK_MINUS TOK_CONSTANT {
       /* Negate the offset */
       fill_far_pointer_operand(req, s, $1.value, $1.width_bits, -$4.value, $4.width_bits);
       s->seen_far_ptr = true;
};

lea_spec: TOK_LSQBR mem_expr TOK_RSQBR {  /* LEA does not use "mem ptr" */
    printf("TODO lea_spec\n"); 
    
    // Tell XED we have an AGEN
    xed_encoder_request_set_agen(req);
    // The AGEN is the next operand
    xed_encoder_request_set_operand_order(
        req, s->operand_index, XED_OPERAND_AGEN);
    s->operand_index++;
};

mem_spec:  segment_override_mem_spec
         | default_segment_mem_spec
;

segment_override_mem_spec: TOK_MEMWIDTH segment_override TOK_LSQBR mem_expr TOK_RSQBR broadcast_expr {
          fill_memory_operand(req, s);
};
      
default_segment_mem_spec: TOK_MEMWIDTH TOK_LSQBR mem_expr TOK_RSQBR broadcast_expr {
        fill_memory_operand(req, s);
};

segment_override: TOK_SEG_REG TOK_COLON {
    s->segment_reg = $1;
    // TODO s->segno ++;
};


// mov    DWORD PTR [rbx+rdx*4+0x20],ecx
// lea    ecx,[rsi*8+0x0]
// nop    WORD PTR cs:[rax+rax*1+0x0]
// vgatherdpd zmm30{k1},qword [r14+ymm31*8+0x7b]

/* TODO support these notations: */
// VADDPD zmm0 {k1},zmm1,zmm3,{rz-sae}
// vaddps zmm7 {k6}, zmm2, zmm4, {rd-sae}
//  disp8*N.
//  vpcompressd [rdi] {k1}, zmm1 - memory and masking from here: https://blogs.msdn.microsoft.com/vcblog/2017/07/11/microsoft-visual-studio-2017-supports-intel-avx-512/
// invlpg, invpcid, invept etc


/* TODO add support and tests for:
unspecified memory width "mem ptr" without word/dword etc.: xsave, sgdt etc.
bnd registers, mib memory operands
debug registers
control registers
long pointers
mm, xmm instructions: movq2dq
mixed operand width: movsx
string operations with segment overrides
shifts with constant/CL
bound operands
*/

// V4FMADDPS zmm1{k1}{z}, zmm2+3, m128 - a pair of zmm registers


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
};

/* GPR +/- const offset */
indirect_addr_gpr_plus_offset: TOK_GPR TOK_PLUS TOK_CONSTANT {
     s->base_reg = $1;
     s->disp_valid = 1;
     s->disp_val = $3.value;
     s->disp_width_bits = $3.width_bits;
}
                            | TOK_GPR TOK_MINUS TOK_CONSTANT {
     s->base_reg = $1;
     s->disp_valid = 1;
     s->disp_val = XED_CAST(xed_uint64_t, -$3.value); /* Use negative constant */
     s->disp_width_bits = $3.width_bits;

};

 /* BX/BP + SI/DI (16-bit specific) */
bx_bp_si_di: TOK_GPR TOK_PLUS TOK_GPR {
     s->base_reg = $1;
     s->index_reg = $3;
};

 /* Base + Index GPR * Scale */
base_index_scale: TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT {
     s->base_reg = $1;
     s->index_reg = $3;
     s->scale_val = $5.value;
     /* TODO: accept only valid numerical scale factors? */
};

 /* Base + Index GPR * Scale +/- constant */
base_index_scale_disp: TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT TOK_PLUS TOK_CONSTANT {
     s->base_reg = $1;
     s->index_reg = $3;
     s->scale_val = $5.value;
     s->disp_valid = 1;
     s->disp_val = $7.value;
     s->disp_width_bits = $7.width_bits;
}
                     | TOK_GPR TOK_PLUS TOK_GPR TOK_MULTI TOK_CONSTANT TOK_MINUS TOK_CONSTANT {
     s->base_reg = $1;
     s->index_reg = $3;
     s->scale_val = $5.value;
     s->disp_valid = 1;
     s->disp_val = XED_CAST(xed_uint64_t, -$7.value); /* Use negative constant */
     s->disp_width_bits = $7.width_bits;
};


 /* TODO implement VSIB */
vsib_mem_expr:
        | TOK_GPR TOK_PLUS TOK_VEC_REG TOK_MULTI TOK_CONSTANT /* Base + Index Vector * Scale */
        | TOK_GPR TOK_PLUS TOK_VEC_REG TOK_MULTI TOK_CONSTANT TOK_PLUS TOK_CONSTANT /* Base + Index Vector * Scale + constant */
;

vec_register_filtered: vec_register_masked
                     | vec_register_masked_zeroed
;

 /* zmm30 {k3} */
vec_register_masked: TOK_VEC_REG TOK_LCUBR TOK_OPMASK_REG TOK_RCUBR {
        /* TODO check that used only once */
        fill_register_operand(req, s, $1); // main register
        deduce_operand_width_vector(req, s, $1);
        fill_register_operand(req, s, $3); // opmask register
};

 /* zmm30 {k3} {z} */
vec_register_masked_zeroed: TOK_VEC_REG TOK_LCUBR TOK_OPMASK_REG TOK_RCUBR TOK_ZEROING {
        /* TODO check that used only once */
        fill_register_operand(req, s, $1); // main register
        deduce_operand_width_vector(req, s, $1);
        fill_register_operand(req, s, $3); // opmask register
        xed3_set_generic_operand(req, XED_OPERAND_ZEROING, 1);
};

 /* 1toX memory broadcast */
broadcast_expr: /* empty */
                | TOK_BCAST {
             /* TODO also support MASM notation: "bcast ptr" */
             xed3_set_generic_operand(req, XED_OPERAND_BCAST, 1);
};

%%
