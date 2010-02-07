/*-----------------------------------------------------------------------------

   CDCTool
   Copyright 2010 Dave Fletcher
   <fletch@fletchtronics.net> http://fletchtronics.net

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. 

-----------------------------------------------------------------------------*/

#include "config.h"
#include <CDC_string.h>

char *cdc_string_copy(const char *original) {
  size_t origlen = strlen(original);
  char *rop = (char*)malloc(origlen+1);
  rop[origlen] = 0;
  strncpy(rop, original, origlen);
  return rop;
}

char *cdc_string_copy_n(const char *original, size_t length) {
  char *rop = (char*)malloc(length+1);
  strncpy(rop, original, length);
  rop[length] = 0;
  return rop;
}

int cdc_string_pos(const char *str, char c, size_t length) {
  int i;
  for (i=0; i<length; i++) {
    if (str[i] == c) return i;
  }
  return -1;
}
