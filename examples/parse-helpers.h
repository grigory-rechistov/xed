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

/// @file parse-helpers.h

#if !defined(PARSE_HELPERS_H)
# define PARSE_HELPERS_H

#include <stdbool.h>
#include "xed/xed-interface.h"

typedef struct {
    xed_category_enum_t early_category; /* early classification of opcode */

    xed_state_t* dstate;
    xed_uint_t operand_index; /* sequential number of all operands */
    /* TODO give better names to members */
    xed_uint_t regnum; /* sequential number of register operand */
    xed_uint_t memop; /* sequential number of memory operand */
    xed_uint_t immed_num; /* sequential number of literal constant */
    xed_uint_t relbr_num; /* sequential number of relative branch literal */

    xed_uint_t deduced_operand_size; /* From register size: AX, EAX, RAX etc. */
    xed_int_t deduced_vector_length; /* (XYZ)MM, -1 for absent */
    xed_uint_t memory_operand_bytes; /* From BYTE PTR, WORD PTR etc. */

    /* Memory operands */
    xed_reg_enum_t segment_reg;
    xed_reg_enum_t base_reg;
    xed_reg_enum_t index_reg;
    xed_uint8_t scale_val;

    /* Literal displacement */
    xed_bool_t disp_valid;
    xed_int64_t disp_val;
    unsigned int disp_width_bits;

    /* Prefixes */
    xed_bool_t repe_seen;
    xed_bool_t repne_seen;
    xed_bool_t lock_seen;

    /* Special cases when both opcode name and operands affect iclass */
    xed_bool_t seen_cr;
    xed_bool_t seen_dr;
    xed_bool_t seen_far_ptr;
} parser_state_t;

void handle_ambiguous_iclasses(xed_encoder_request_t *req, parser_state_t *s);

void deduce_operand_width_gpr(xed_encoder_request_t* req, parser_state_t *s,
                              xed_reg_enum_t reg);

void deduce_operand_width_vector(xed_encoder_request_t* req, parser_state_t *s,
                                 xed_reg_enum_t reg);

xed_reg_enum_t parse_single_register(const char* txt);

void fill_mnemonic_opcode(xed_encoder_request_t* req, parser_state_t *s, char* opcode);
void fill_memory_operand(xed_encoder_request_t* req, parser_state_t *s);
void fill_register_operand(xed_encoder_request_t* req, parser_state_t *s, xed_reg_enum_t reg_name);
void fill_immediate_operand(xed_encoder_request_t* req, parser_state_t *s,
                xed_uint64_t value, unsigned width_bits);
void fill_far_pointer_operand(xed_encoder_request_t* req, parser_state_t *s,
                xed_uint64_t seg_value, unsigned segment_bits,
                xed_uint64_t offset_value, unsigned offset_bits);
void fill_relative_offset_operand(xed_encoder_request_t* req, parser_state_t *s,
                                  xed_uint64_t value, unsigned orig_bits);
xed_bool_t instr_category_uses_rel_branch(xed_category_enum_t cat);

#endif // PARSE_HELPERS_H
