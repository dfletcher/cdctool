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
#ifndef CDCWORDSIZE_H
#define CDCWORDSIZE_H

#include "CDC_file.h"

/**
 *  Set of common word (character) sizes.
 */
typedef enum {
  CDCWORDSIZE_FIVE = 5,
  CDCWORDSIZE_SIX = 6,
  CDCWORDSIZE_SEVEN = 7,
  CDCWORDSIZE_EIGHT = 8,
} CDCWordSize;

/**
 *  Set the baud rate of a CDCFile.
 *  @param file CDCFile to set baud rate parameter.
 *  @param wordsize Desired word size.
 */
void cdc_set_wordsize(CDCFile *file, CDCWordSize wordsize);

/**
 *  Get the current word size of a CDCFile.
 *  @param file CDCFile to get word size parameter.
 *  @return Current word size.
 */
 CDCWordSize cdc_get_wordsize(CDCFile *file);

#endif /* [CDCWORDSIZE_H] */
