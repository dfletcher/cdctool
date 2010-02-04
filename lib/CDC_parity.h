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
#ifndef CDCPARITY_H
#define CDCPARITY_H

#include <CDC_file.h>

/**
 *  Set of common parity settings.
 */
typedef enum {
  CDCPARITY_NOPARITY,
  CDCPARITY_ODDPARITY,
  CDCPARITY_EVENPARITY,
} CDCParity;

/**
 *  Set the parity of a CDCFile.
 *  @param file CDCFile to set parity parameter.
 *  @param parity Desired parity.
 */
void cdc_set_parity(CDCFile *file, CDCParity parity);

/**
 *  Get the current parity of a CDCFile.
 *  @param file CDCFile to get parity parameter.
 *  @return Current parity.
 */
CDCParity cdc_get_parity(CDCFile *file);

#endif /* [CDCPARITY_H] */
