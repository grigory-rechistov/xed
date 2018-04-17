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

// Helper functions to parse mnemonic representation into Xed encode request

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "xed-util.h"
#include "parse-helpers.h"

static void decorate_opcode_mnemonic(char* opcode, xed_uint_t max_len,
                                     const parser_state_t *s)
{
    /* Use _NEAR variants as default iclass for "call" and "ret".
       They will be substituted with _FAR versions later, if needed */
    if ( !strncmp(opcode, "CALL", max_len)
      || !strncmp(opcode, "RET", max_len)) {
        xed_strncat(opcode, "_NEAR", max_len);
        return;
    }

    if (opcode[0] == 'J') {/* all aliases for conditional jumps start with J */
        typedef struct {
            const char *from;
            const char *to;
        } jcc_aliases_t;

        const jcc_aliases_t aliases[] = {
           {"JNAE", "JB"},
           {"JC", "JB"},
           {"JNA", "JBE"},
           {"JNGE", "JL"},
           {"JNG", "JLE"},
           {"JAE", "JNB"},
           {"JNC", "JNB"},
           {"JA", "JNBE"},
           {"JGE", "JNL"},
           {"JG", "JNLE"},
           {"JPO", "JNP"},
           {"JNE", "JNZ"},
           {"JPE", "JP"},
           {"JE", "JZ"},
        };
        const size_t n_aliases = sizeof(aliases)/sizeof(aliases[0]);

        /* Resolve conditional jumps aliases */
        for (int i = 0; i < n_aliases; i++) {
            const char *from = aliases[i].from;
            const char *to = aliases[i].to;
            if(!strncmp(opcode, from, max_len)) {
                strncpy(opcode, to, max_len);
                return;
            }
        }
    }

    /* Sometimes prefixes are encoded inside iclass. We've seen all prefixes
       now and can act on them */
    xed_bool_t has_front_prefix = s->repe_seen || s->repne_seen;
    xed_bool_t has_post_prefix = s->lock_seen;

    if (!has_front_prefix && !has_post_prefix)
        return;

    /* make sure string length was passed correctly */
    xed_assert(max_len > sizeof(char*));
    char tmp[max_len];
    tmp[0] = '\0';

    if (has_front_prefix) {
        /* TODO iclasses contain all three forms: REP_, REPE_ and REPNE_.
           To properly form the new mnemonic, classification of
           original_iclass is needed to tell REP_ from REPE_ */
        //    xed_iclass_t original_iclass = str2xed_iclass_enum_t(opcode);
        const char *forward_prefix = s->repe_seen? "REPE_":
                                     s->repne_seen ? "REPNE_": 0;
        xed_strncpy(tmp, forward_prefix, max_len);
    }
    xed_strncat(tmp, opcode, max_len);

    if (has_post_prefix) {
        const char *post_prefix = "_LOCK";
        xed_strncat(tmp, post_prefix, max_len);
    }

    xed_strncpy(opcode, tmp, max_len);
}

void handle_ambiguous_iclasses(xed_encoder_request_t *req, parser_state_t *s)
{
    /* Resolve naming ambiguities between mnemonics and iclasses.
       Adjustment of iclass is done for the following cases:
       TODO:
       FXRSTOR vs FXRSTOR64 and other *SAVE/ *RSTR(64)
       PEXTRW PEXTRW_SSE4
       VPEXTRW VPEXTRW_c5
       Long NOPs: XED_ICLASS_NOP2 - NOP9
     */
    switch (xed3_operand_get_iclass(req)) {
    case XED_ICLASS_MOV: /* moves to control/debug registers */
        if (s->seen_cr)
            xed_encoder_request_set_iclass(req, XED_ICLASS_MOV_CR);
        else if (s->seen_dr)
            xed_encoder_request_set_iclass(req, XED_ICLASS_MOV_DR);
        break;
    case XED_ICLASS_MOVSD: /* string or SSE opcode? */
        if (s->deduced_vector_length == 0) /* There are XMM operands */
            xed_encoder_request_set_iclass(req, XED_ICLASS_MOVSD_XMM);
        break;
    case XED_ICLASS_CMPSD:
        if (s->deduced_vector_length == 0) /* There are XMM operands */
            xed_encoder_request_set_iclass(req, XED_ICLASS_CMPSD_XMM);
        break;
    case XED_ICLASS_CALL_NEAR: /* convert near to far if operands state that */
        if (s->seen_far_ptr) {
            xed_encoder_request_set_iclass(req, XED_ICLASS_CALL_FAR);
        }
        break;
    case XED_ICLASS_RET_NEAR:
         if (s->seen_far_ptr) {
                 xed_encoder_request_set_iclass(req, XED_ICLASS_RET_FAR);
         }
         break;
    case XED_ICLASS_JMP:
        if (s->seen_far_ptr) {
            xed_encoder_request_set_iclass(req, XED_ICLASS_JMP_FAR);
        }
        break;


     /* TODO handle other cases */
     default:
         break;
     }
}


/* The same mnemonic may have operands of different width. Adjust operands
    width when a GPR operand is known */
void deduce_operand_width_gpr(xed_encoder_request_t* req, parser_state_t *s,
                              xed_reg_enum_t reg)
{
    xed_reg_class_enum_t rc = xed_gpr_reg_class(reg);
    switch (rc) {
    case XED_REG_CLASS_GPR8:
            xed_encoder_request_set_effective_operand_width(req, 8);
            s->deduced_operand_size = 8;
            break;
    case XED_REG_CLASS_GPR16:
            xed_encoder_request_set_effective_operand_width(req, 16);
            s->deduced_operand_size = 16;
            break;
    case XED_REG_CLASS_GPR32:
            xed_encoder_request_set_effective_operand_width(req, 32);
            s->deduced_operand_size = 32;
            break;
    case XED_REG_CLASS_GPR64:
            xed_encoder_request_set_effective_operand_width(req, 64);
            s->deduced_operand_size = 64;
            break;
    default:
        break;
    }
}

void deduce_operand_width_vector(xed_encoder_request_t* req, parser_state_t *s,
                                 xed_reg_enum_t reg)
{
    xed_reg_class_enum_t rc = xed_reg_class(reg);
    switch (rc) {
    case XED_REG_CLASS_XMM:
            s->deduced_vector_length = 0;
            xed3_operand_set_vl(req, 0);
            break;
    case XED_REG_CLASS_YMM:
            s->deduced_vector_length = 1;
            xed3_operand_set_vl(req, 1);
            break;
    case XED_REG_CLASS_ZMM:
            s->deduced_vector_length = 2;
            xed3_operand_set_vl(req, 2);
            break;
    default:
        break;
    }
}

xed_reg_enum_t parse_single_register(parser_state_t *s, const char* txt)
{
    xed_reg_enum_t reg = str2xed_reg_enum_t(txt);
    if (reg == XED_REG_INVALID) {
        fprintf(stderr,
             "[XED CLIENT ERROR] Bad register name: %s\n", txt);
        s->error_found = 1;
        return XED_REG_INVALID;
    }
    return reg;
}

void fill_register_operand(xed_encoder_request_t* req, parser_state_t *s, xed_reg_enum_t reg_name)
{
    if (s->dstate->mmode != XED_MACHINE_MODE_LONG_64)
        if (reg_name == XED_REG_DIL || reg_name == XED_REG_SPL
        ||  reg_name == XED_REG_BPL || reg_name == XED_REG_SIL)
    {
        fprintf(stderr,
            "[XED CLIENT ERROR] Cannot use DIL/SPL/BPL/SIL outside of 64b mode\n");
        s->error_found = 1;
        return;
    }
    if (s->reg_num > 8) {
        fprintf(stderr,
              "[XED CLIENT ERROR] Only up to nine register operands allowed\n");
        s->error_found = 1;
        return;
    }
    // The registers operands are numbered starting from the first one
    // as XED_OPERAND_REG0. We increment reg_num (below) every time we
    // add a register operand.
    xed_operand_enum_t reg_pos = XED_CAST(xed_operand_enum_t,
                                          XED_OPERAND_REG0 + s->reg_num);
    // store the register identifier in the operand storage field
    xed_encoder_request_set_reg(req, reg_pos, reg_name);
    // store the operand storage field name in the encode-order array
    xed_encoder_request_set_operand_order(req, s->operand_index, reg_pos);
    deduce_operand_width_gpr(req, s, reg_name);

    s->operand_index++;
    s->reg_num++;
}

/* Bring displacement width to a value that Xed actually can accept */
static int drag_to_accepted_literal_width(int width_bits) {
        if (width_bits <= 8)
            return 8;
        else if (width_bits <= 16 /* && TODO valid in 16 bit mode only! */)
            return 16;
        else if (width_bits <= 32)
            return 32;
        return width_bits;
}

void fill_memory_operand(xed_encoder_request_t* req, parser_state_t *s)
{
    // FIXME: add MEM(immed) for the OC1_A and OC1_O types????
    xed_reg_class_enum_t rc = XED_REG_CLASS_INVALID;
    xed_reg_class_enum_t rci = XED_REG_CLASS_INVALID;

    if (s->memory_operand_bytes == 0) {
        s->memory_operand_bytes  = guess_memory_operand_bytes(req, s);
    }

    if (s->memop_num == 0) {
         // Tell XED that we have a memory operand
         xed_encoder_request_set_mem0(req);
         // Tell XED that the mem0 operand is the next operand:
         xed_encoder_request_set_operand_order(
             req, s->operand_index, XED_OPERAND_MEM0);
    } else if (s->memop_num == 1) {
        xed_encoder_request_set_mem1(req);
        // Tell XED that the mem1 operand is the next operand:
        xed_encoder_request_set_operand_order(
                req, s->operand_index, XED_OPERAND_MEM1);
    } else {
        fprintf(stderr,
            "[XED CLIENT ERROR] Only up to two memory operands allowed\n");
        s->error_found = 1;
        return;
    }

    rc = xed_gpr_reg_class(s->base_reg);
    rci = xed_gpr_reg_class(s->index_reg);

    if (s->base_reg == XED_REG_EIP)
        xed_encoder_request_set_effective_address_size(req, 32);
    else if (rc == XED_REG_CLASS_GPR32 || rci == XED_REG_CLASS_GPR32)
        xed_encoder_request_set_effective_address_size(req, 32);
    else if (rc == XED_REG_CLASS_GPR16 || rci == XED_REG_CLASS_GPR16)
        xed_encoder_request_set_effective_address_size(req, 16);

    // fill in the memory fields collected by parser
    xed_encoder_request_set_base0(req, s->base_reg);
    xed_encoder_request_set_index(req, s->index_reg);
    xed_encoder_request_set_scale(req, s->scale_val);
    xed_encoder_request_set_seg0(req,  s->segment_reg);

    if (s->memory_operand_bytes)
        xed_encoder_request_set_memory_operand_length(
            req,
            s->memory_operand_bytes); // BYTES
    if (s->disp_valid) {
        unsigned int width_bytes =
                        drag_to_accepted_literal_width(s->disp_width_bits)/8;
        xed_encoder_request_set_memory_displacement(req, s->disp_val,
                                                    width_bytes);
    }

    s->memop_num++;
    s->operand_index++;
}


void fill_agen_operand(xed_encoder_request_t* req, parser_state_t *s)
{
    /* People confuse AGEN and MEM syntax easily, warn them */
    if (xed3_operand_get_iclass(req) != XED_ICLASS_LEA) {
        printf("AGEN operand is only expected for LEA."
               " Are you sure you did not omit \"mem ptr\" specifier?\n");
    }

    xed_reg_class_enum_t rc = XED_REG_CLASS_INVALID;
    xed_reg_class_enum_t rci = XED_REG_CLASS_INVALID;

    // Tell XED we have an AGEN
    xed_encoder_request_set_agen(req);
    // The AGEN is the next operand
    xed_encoder_request_set_operand_order(
                req, s->operand_index, XED_OPERAND_AGEN);

    rc = xed_gpr_reg_class(s->base_reg);
    rci = xed_gpr_reg_class(s->index_reg);

    if (s->base_reg == XED_REG_EIP)
        xed_encoder_request_set_effective_address_size(req, 32);
    else if (rc == XED_REG_CLASS_GPR32 || rci == XED_REG_CLASS_GPR32)
        xed_encoder_request_set_effective_address_size(req, 32);
    else if (rc == XED_REG_CLASS_GPR16 || rci == XED_REG_CLASS_GPR16)
        xed_encoder_request_set_effective_address_size(req, 16);

    // fill in the memory fields collected by parser
    xed_encoder_request_set_base0(req, s->base_reg);
    xed_encoder_request_set_index(req, s->index_reg);
    xed_encoder_request_set_scale(req, s->scale_val);
    xed_encoder_request_set_seg0(req,  s->segment_reg);

    if (s->memory_operand_bytes)
        xed_encoder_request_set_memory_operand_length(
            req,
            s->memory_operand_bytes);
    if (s->disp_valid) {
        unsigned int width_bytes =
                        drag_to_accepted_literal_width(s->disp_width_bits)/8;
        xed_encoder_request_set_memory_displacement(req, s->disp_val,
                                                    width_bytes);
    }

    s->operand_index++;
}

void fill_immediate_operand(xed_encoder_request_t* req, parser_state_t *s,
                xed_uint64_t value, unsigned orig_bits)
{
    unsigned width_bits = drag_to_accepted_literal_width(orig_bits);
    if (s->immed_num == 0) {
        xed_encoder_request_set_uimm0_bits(req, value, width_bits);
        xed_encoder_request_set_operand_order(req,
                                              s->operand_index,
                                              XED_OPERAND_IMM0);
        /* TODO what to do with signed immediate? Dispatching based on opcode:
           expecting signed, expecting relbranch, abs pointer etc. */
        //xed_encoder_request_set_simm(
        //req,
        //XED_STATIC_CAST(xed_int32_t,simm.immed_val),
        //simm.width_bits/8); //FIXME
    } else if (s->immed_num == 1) {
       if (width_bits != 8) {
           fprintf(stderr, "[XED CLIENT ERROR] The second immediate may only"
                           " be 8 bits wide\n");
           s->error_found = 1;
           return;
       }
       xed_encoder_request_set_uimm1(req, value);
       xed_encoder_request_set_operand_order(req,
                                             s->operand_index,
                                             XED_OPERAND_IMM1);
   } else {
        fprintf(stderr,
            "[XED CLIENT ERROR] Only up to two immediate operands allowed\n");
        s->error_found = 1;
        return;
   }
    s->immed_num++;
    s->operand_index++;
}

void fill_far_pointer_operand(xed_encoder_request_t* req, parser_state_t *s,
                xed_uint64_t seg_value, unsigned segment_bits,
                xed_uint64_t offset_value, unsigned offset_bits)
{
    if (offset_bits > 32) {
        fprintf(stderr, "[XED CLIENT ERROR] Far pointer offset is too wide\n");
        s->error_found = 1;
        return;
    }
    offset_bits = offset_bits < 16? 16:32;

    xed_encoder_request_set_branch_displacement(req,
                    XED_STATIC_CAST(xed_uint32_t,offset_value),
                    offset_bits/8);
    xed_encoder_request_set_operand_order(req, s->operand_index,
                                          XED_OPERAND_PTR);
    xed_encoder_request_set_ptr(req);

    s->operand_index++;

    /* segment is encoded as immediate */
    if (s->immed_num != 0) {
        fprintf(stderr, "[XED CLIENT ERROR] Far pointer cannot"
                        " follow immediate operand\n");
        s->error_found = 1;
        return;
    }
    if (segment_bits > 16) {
        fprintf(stderr, "[XED CLIENT ERROR] Far pointer"
                        " segment is too wide\n");
        s->error_found = 1;
        return;
    }
    xed_encoder_request_set_uimm0_bits(req, seg_value, 16);
    xed_encoder_request_set_operand_order(req, s->operand_index,
                                          XED_OPERAND_IMM0);
    s->immed_num++;
    s->operand_index++;
}


/* Convert important iclasses to category, return invalid for the rest */
static xed_category_enum_t early_categorize_iclass(xed_iclass_enum_t iclass) {

    /* Currently we are interested in control flow instructions */
    switch (iclass) {
    case XED_ICLASS_JMP:
    case XED_ICLASS_JMP_FAR:
        return XED_CATEGORY_UNCOND_BR;
        break;
    case XED_ICLASS_JB:
    case XED_ICLASS_JBE:
    case XED_ICLASS_JCXZ:
    case XED_ICLASS_JECXZ:
    case XED_ICLASS_JL:
    case XED_ICLASS_JLE:
    case XED_ICLASS_JNB:
    case XED_ICLASS_JNBE:
    case XED_ICLASS_JNL:
    case XED_ICLASS_JNLE:
    case XED_ICLASS_JNO:
    case XED_ICLASS_JNP:
    case XED_ICLASS_JNS:
    case XED_ICLASS_JNZ:
    case XED_ICLASS_JO:
    case XED_ICLASS_JP:
    case XED_ICLASS_JRCXZ:
    case XED_ICLASS_JS:
    case XED_ICLASS_JZ:
    case XED_ICLASS_XBEGIN:
         return XED_CATEGORY_COND_BR;
         break;
    case XED_ICLASS_CALL_FAR:
    case XED_ICLASS_CALL_NEAR:
         return XED_CATEGORY_CALL;
         break;
    case XED_ICLASS_RET_FAR:
    case XED_ICLASS_RET_NEAR:
         return XED_CATEGORY_RET;
         break;
    case XED_ICLASS_MOVSD:
    case XED_ICLASS_REP_MOVSD:
    case XED_ICLASS_SCASB:
    case XED_ICLASS_REPE_SCASB:
    case XED_ICLASS_REPNE_SCASB:
    case XED_ICLASS_SCASD:
    /* TODO list all string iclasses */
         return XED_CATEGORY_STRINGOP;
         break;
    default:
        break;
    }
    return XED_CATEGORY_INVALID; /* iclass does not have special treatment */
}

void fill_mnemonic_opcode(xed_encoder_request_t* req, parser_state_t *s,
                          char* opcode, unsigned max_len) {
    /* Sometimes prefixes are encoded inside iclass. We've seen all prefixes
      now and can act on them */
    decorate_opcode_mnemonic(opcode, max_len, s);
    xed_iclass_enum_t iclass =  str2xed_iclass_enum_t(opcode);
    if (iclass == XED_ICLASS_INVALID) {
        if (s->repne_seen || s->repe_seen || s->lock_seen)
           fprintf(stderr,
           "[XED CLIENT ERROR] Bad instruction name or incompatible"
           " prefixes: '%s'\n", opcode);
        else
            fprintf(stderr,
                "[XED CLIENT ERROR] Bad instruction name: '%s'\n", opcode);
        s->error_found = 1;
        return;
    }
    xed_encoder_request_set_iclass(req, iclass);

    s->early_category = early_categorize_iclass(iclass);
}

void fill_relative_offset_operand(xed_encoder_request_t* req, parser_state_t *s,
                                  xed_uint64_t value, unsigned orig_bits)
{
    if (s->relbr_num > 0) {
        fprintf(stderr,
             "[XED CLIENT ERROR] Only one relative branch allowed\n");
        s->error_found = 1;
        return;
    }
    unsigned width_bits = drag_to_accepted_literal_width(orig_bits);

    xed_encoder_request_set_branch_displacement(
        req,
        XED_STATIC_CAST(xed_uint32_t,value),
        width_bits/8);
    xed_encoder_request_set_operand_order(req,
                                          s->operand_index,
                                          XED_OPERAND_RELBR);
    xed_encoder_request_set_relbr(req);

    s->relbr_num++;
    s->operand_index++;
}

xed_bool_t instr_category_uses_rel_branch(xed_category_enum_t cat) {
    switch (cat) {
    case XED_CATEGORY_CALL:
    case XED_CATEGORY_COND_BR:
    case XED_CATEGORY_UNCOND_BR:
        return 1;
    default:
        return 0;
    }
    return 0;
}

/* For instructions that use m16:32, m16&32, m16&64 and m16:64 notations */
static xed_uint_t mem6_or_10(const xed_state_t *dstate)
{
    /* TODO account for address size override somehow */
    switch(dstate->mmode) {
    case XED_MACHINE_MODE_LONG_64:
        return 10;
    default:
        return 6;
    }
}

static xed_uint_t mem94_or_108(const xed_state_t *dstate)
{
    /* TODO account for address size override somehow */
    switch(dstate->mmode) {
    case XED_MACHINE_MODE_REAL_16:
    case XED_MACHINE_MODE_LEGACY_16:
    case XED_MACHINE_MODE_LONG_COMPAT_16:
        return 94;
    default:
        return 108;
    }
}

static xed_uint_t mem14_or_28(const xed_state_t *dstate)
{
    /* TODO account for address size override somehow */
    switch(dstate->mmode) {
    case XED_MACHINE_MODE_REAL_16:
    case XED_MACHINE_MODE_LEGACY_16:
    case XED_MACHINE_MODE_LONG_COMPAT_16:
        return 14;
    default:
        return 28;
    }
}

/* When "mem ptr" is specified, memory operand width is defined by opcode */
xed_uint_t guess_memory_operand_bytes(xed_encoder_request_t* req,
                                      parser_state_t *s)
{
    /* Non-standard memory operand sizes are listed in xed-operand-width.txt */
    switch (xed3_operand_get_iclass(req)) {
    case XED_ICLASS_JMP_FAR:
    case XED_ICLASS_CALL_FAR:
        return mem6_or_10(s->dstate);
    case XED_ICLASS_JMP:
    case XED_ICLASS_CALL_NEAR:
        if (s->seen_far_ptr) /* Resolve ambiguity between near and far calls */
            return mem6_or_10(s->dstate);
        break;
    case XED_ICLASS_XSAVE:
    case XED_ICLASS_XSAVE64:
    case XED_ICLASS_XSAVEC:
    case XED_ICLASS_XSAVEC64:
    case XED_ICLASS_XSAVEOPT:
    case XED_ICLASS_XSAVEOPT64:
    case XED_ICLASS_XSAVES:
    case XED_ICLASS_XSAVES64:
    case XED_ICLASS_XRSTOR:
    case XED_ICLASS_XRSTOR64:
    case XED_ICLASS_XRSTORS:
    case XED_ICLASS_XRSTORS64:
        return 576; /* That is what XED uses */

    case XED_ICLASS_FXSAVE:
    case XED_ICLASS_FXSAVE64:
    case XED_ICLASS_FXRSTOR:
    case XED_ICLASS_FXRSTOR64:
        return 512;

    case XED_ICLASS_FNSAVE:
    case XED_ICLASS_FRSTOR:
        return mem94_or_108(s->dstate);
    case XED_ICLASS_FLDENV:
    case XED_ICLASS_FNSTENV:
         return mem14_or_28(s->dstate);

    case XED_ICLASS_LGDT:
    case XED_ICLASS_LIDT:
        return mem6_or_10(s->dstate);
    case XED_ICLASS_LDS:
    case XED_ICLASS_LES:
        return 6; /* Not present in 64-bit mode */
    case XED_ICLASS_LSS:
    case XED_ICLASS_LFS:
    case XED_ICLASS_LGS:
        return mem6_or_10(s->dstate);
    default:
        break;
    }

    /* We do not want guessing otherwise */
    fprintf(stderr,"Cannot guess memory operand size. Specify it explicitly\n");
    return 0;
}

void specify_opsize_override(xed_encoder_request_t* req, parser_state_t *s,
                             xed_uint_t opsize)
{
    switch (opsize) {
    case 8:
        xed_encoder_request_set_effective_operand_width(req, 8);
        break;
    case 16:
        xed_encoder_request_set_effective_operand_width(req, 16);
        break;
    case 32:
        xed_encoder_request_set_effective_operand_width(req, 32);
        break;
    case 64:
        xed_encoder_request_set_effective_operand_width(req, 64);
        break;
    /* TODO support vector element size overrides: 128, 256, 512? */
    default:
            fprintf(stderr,
                    "[XED CLIENT ERROR] Unsupported operand size value: %u\n",
                    opsize);
        s->error_found = 1;
        break;
    }
}
