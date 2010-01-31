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

#ifndef CDCSTRING_H
#define CDCSTRING_H

#include <stdlib.h>
#include <string.h>

/**
 *  Allocate a new string and copy the contents of the original.
 *  @param original String to copy. Must be NULL terminated.
 *  @return String copy.
 */
char *cdc_string_copy(char *original);

/**
 *  Allocate a new string and copy 'length' characters from the original.
 *  @param original String to copy.
 *  @param length Number of characters to copy.
 *  @return String copy.
 */
char *cdc_string_copy_n(char *original, size_t length);

/**
 *  Utility function. Find the position of a character in the string, searching
 *  from str[0] to str[length-1].
 *  @param str String to search.
 *  @param c Character to search for.
 *  @param length Maximum search length.
 *  @return Position of the character in the string, or -1 if the character was
 *  not found.
 */
int cdc_string_pos(char *str, char c, size_t length);

#endif /* [CDCSTRING_H] */
