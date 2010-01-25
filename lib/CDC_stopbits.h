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
#ifndef CDCSTOPBITS_H
#define CDCSTOPBITS_H

#include "CDC_file.h"

/**
 *  Set of common stop-bits settings.
 */
typedef enum {
  CDCSTOPBITS_ONE,
  CDCSTOPBITS_TWO,
} CDCStopBits;

/**
 *  Set the stop bits of a CDCFile.
 *  @param file CDCFile to set stop bits parameter.
 *  @param stopbits Desired stop bits.
 */
void cdc_set_stopbits(CDCFile *file, CDCStopBits stopbits);

/**
 *  Get the current stop bits of a CDCFile.
 *  @param file CDCFile to get stop bits parameter.
 *  @return Current stop bits.
 */
CDCStopBits cdc_get_stopbits(CDCFile *file);

#endif /* [CDCSTOPBITS_H] */
