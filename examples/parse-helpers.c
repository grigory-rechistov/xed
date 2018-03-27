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

// Helper functions to parse menmonic representation into Xed encode request


#include "xed-util.h"
#include "parse-helpers.h"

/* Sometimes prefixes are encoded inside iclass. We've seen all prefixes
   now and can act on them */
void decorate_opcode_mnemonic(char* opcode, xed_uint_t len,
                              const parser_state_t *s)
{
    /* make sure string length was passed correctly */
    xed_assert(len > sizeof(char*));
    return;
}

