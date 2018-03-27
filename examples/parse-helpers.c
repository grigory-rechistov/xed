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

//    xed_iclass_t original_iclass = str2xed_iclass_enum_t(opcode);

    if (has_front_prefix) {
        /* TODO iclasses contain all three forms: REP_, REPE_ and REPNE_.
           To properly form the new mnemonic, classification of
           original_iclass is needed to tell REP_ from REPE_ */
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

