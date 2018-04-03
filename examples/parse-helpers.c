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

void decorate_opcode_mnemonic(char* opcode, xed_uint_t len,
                              const parser_state_t *s)
{
    /* Use _NEAR variants as default iclass for "call" and "ret".
       They will be substituted with _FAR versions later, if needed */
    if ( !strncmp(opcode, "CALL", 4)
      || !strncmp(opcode, "RET", 4)) {
        xed_strncat(opcode, "_NEAR", len);
        return;
    }


    /* Sometimes prefixes are encoded inside iclass. We've seen all prefixes
       now and can act on them */
    bool has_front_prefix = s->repe_seen || s->repne_seen;
    bool has_post_prefix = s->lock_seen;

    if (!has_front_prefix && !has_post_prefix)
        return;

    /* make sure string length was passed correctly */
    xed_assert(len > sizeof(char*));
    char tmp[len];
    tmp[0] = '\0';


    if (has_front_prefix) {
        /* TODO iclasses contain all three forms: REP_, REPE_ and REPNE_.
           To properly form the new mnemonic, classification of
           original_iclass is needed to tell REP_ from REPE_ */
        //    xed_iclass_t original_iclass = str2xed_iclass_enum_t(opcode);
        const char *forward_prefix = s->repe_seen? "REPE_":
                                     s->repne_seen ? "REPNE_": 0;
        xed_strncpy(tmp, forward_prefix, len);
    }
    xed_strncat(tmp, opcode, len);

    if (has_post_prefix) {
        const char *post_prefix = "_LOCK";
        xed_strncat(tmp, post_prefix, len);
    }

    xed_strncpy(opcode, tmp, len);
}

void handle_ambiguous_iclasses(xed_encoder_request_t *req, parser_state_t *s)
{
    /* Resolve naming ambiguities between mnemonics and iclasses.
       Adjustment of iclass is done for the following cases:
       TODO:
       FXRSTOR vs FXRSTOR64 and other *SAVE/ *RSTR(64)
       PEXTRW PEXTRW_SSE4
       VPEXTRW VPEXTRW_c5
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

xed_reg_enum_t parse_single_register(const char* txt)
{
    xed_reg_enum_t reg = str2xed_reg_enum_t(txt);
    if (reg == XED_REG_INVALID) {
        fprintf(stderr,
             "[XED CLIENT ERROR] Bad register name: %s\n", txt);
        exit(1);
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
        exit(1);
    }
    if (s->regnum > 8) {
        fprintf(stderr,
              "[XED CLIENT ERROR] Only up to nine register operands allowed\n");
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

/* Bring displacement width to a value that Xed actually can accept */
static int drag_to_accepted_immediate_width(int width_bits) {
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

    if (s->memop == 0) {
         // Tell XED that we have a memory operand
         xed_encoder_request_set_mem0(req);
         // Tell XED that the mem0 operand is the next operand:
         xed_encoder_request_set_operand_order(
             req, s->operand_index, XED_OPERAND_MEM0);
    } else if (s->memop == 1) {
        xed_encoder_request_set_mem1(req);
        // Tell XED that the mem1 operand is the next operand:
        xed_encoder_request_set_operand_order(
                req, s->operand_index, XED_OPERAND_MEM1);
    } else {
         fprintf(stderr,
             "[XED CLIENT ERROR] Only up to two memory operands allowed\n");
         exit(1);
    }

//    else if (mem_bis.agen) {
//        // Tell XED we have an AGEN
//        xed_encoder_request_set_agen(&req);
//        // The AGEN is the next operand
//        xed_encoder_request_set_operand_order(
//            &req,operand_index, XED_OPERAND_AGEN);
//    }


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

    // TODO reenable later
//    find_vl(s->index_reg, &vl); // for scatter/gather

    if (s->memory_operand_bytes)
        xed_encoder_request_set_memory_operand_length(
            req,
            s->memory_operand_bytes); // BYTES
    if (s->disp_valid) {
        unsigned int width_bytes =
                        drag_to_accepted_immediate_width(s->disp_width_bits)/8;
        xed_encoder_request_set_memory_displacement(req, s->disp_val,
                                                    width_bytes);
    }

    s->memop++;
    s->operand_index++;
}

void fill_immediate_operand(xed_encoder_request_t* req, parser_state_t *s,
                xed_uint64_t value, unsigned orig_bits)
{
    unsigned width_bits = drag_to_accepted_immediate_width(orig_bits);
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
           exit(1);
       }
       xed_encoder_request_set_uimm1(req, value);
       xed_encoder_request_set_operand_order(req,
                                             s->operand_index,
                                             XED_OPERAND_IMM1);
   } else {
       fprintf(stderr,
            "[XED CLIENT ERROR] Only up to two immediate operands allowed\n");
       exit(1);
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
        exit(1);
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
        exit(1);
    }
    if (segment_bits > 16) {
            fprintf(stderr, "[XED CLIENT ERROR] Far pointer"
                            " segment is too wide\n");
            exit(1);
    }
    xed_encoder_request_set_uimm0_bits(req, seg_value, 16);
    xed_encoder_request_set_operand_order(req, s->operand_index,
                                          XED_OPERAND_IMM0);

    s->immed_num++;
    s->operand_index++;
}
