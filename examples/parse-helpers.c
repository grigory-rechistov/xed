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

#include "xed-util.h"
#include "parse-helpers.h"

/* Sometimes prefixes are encoded inside iclass. We've seen all prefixes
   now and can act on them */
void decorate_opcode_mnemonic(char* opcode, xed_uint_t len,
                              const parser_state_t *s)
{
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
    /* TODO do other Xed-specific transformations of mnemonic to iclass
        E.g.: MOVSD -> MOVSD_XMM */

    xed_strncpy(opcode, tmp, len);
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
            break;
    case XED_REG_CLASS_GPR16:
            xed_encoder_request_set_effective_operand_width(req, 16);
            break;
    case XED_REG_CLASS_GPR32:
            xed_encoder_request_set_effective_operand_width(req, 32);
            break;
    case XED_REG_CLASS_GPR64:
            xed_encoder_request_set_effective_operand_width(req, 64);
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

