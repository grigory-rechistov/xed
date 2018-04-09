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
    xed_reg_enum_t regname;
    struct {
        xed_uint64_t value;
        unsigned int width_bits;
    } literal;

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

%token<regname> TOK_VEC_REG
%token<regname> TOK_MASK_REG
%token<regname> TOK_SEG_REG
%token<regname> TOK_FPU_REG
%token<regname> TOK_MMX_REG

%token<literal> TOK_CONSTANT
%token TOK_MEMWIDTH


%token T_NEWLINE
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
        fill_register_operand(req, s, $1);
};

vector_register: TOK_VEC_REG {
        xed_reg_enum_t reg_name = $1;
        printf("TODO add the rest of vector register logic \n");
        deduce_operand_width_vector(req, s, reg_name);
}
;

immediate: TOK_CONSTANT // all types of literals
;

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

segment_override_mem_spec: TOK_MEMWIDTH segment_override TOK_LSQBR mem_expr TOK_RSQBR {
          fill_memory_operand(req, s);
};
      
default_segment_mem_spec: TOK_MEMWIDTH TOK_LSQBR mem_expr TOK_RSQBR {
        fill_memory_operand(req, s);
};

segment_override: TOK_SEG_REG TOK_COLON {
    s->segment_reg = $1;
    // TODO s->segno ++;
};


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
        | TOK_GPR TOK_PLUS TOK_GPR /* GPR + GPR (16-bit specific) */
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
