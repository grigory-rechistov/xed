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
    xed_state_t* dstate;
    xed_uint_t operand_index; /* sequential number of all operands */
    xed_uint_t regnum; /* sequential number of register operand */
    xed_uint_t memop; /* sequential number of memory operand */

    xed_uint_t deduced_operand_size; /* From register size: AX, EAX, RAX etc. */
    xed_int_t deduced_vector_length; /* (XYZ)MM, -1 for absent */
    xed_uint_t memory_operand_bytes; /* From BYTE PTR, WORD PTR etc. */

    bool repe_seen;
    bool repne_seen;
    bool lock_seen;
} parser_state_t;

void decorate_opcode_mnemonic(char* opcode, xed_uint_t len, const parser_state_t *s);

void deduce_operand_width_gpr(xed_encoder_request_t* req, parser_state_t *s,
                              xed_reg_enum_t reg);

void deduce_operand_width_vector(xed_encoder_request_t* req, parser_state_t *s,
                                 xed_reg_enum_t reg);

xed_reg_enum_t parse_single_register(const char* txt);


#endif // PARSE_HELPERS_H
