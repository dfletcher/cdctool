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
#ifndef CDCREAD_H
#define CDCREAD_H

#include <CDC_file.h>

/**
 *  Read data from a CDCFile. Set any needed serial comm parameters before
 *  calling this. This function waits for input, reads and returns a single
 *  line.
 *  @param file CDCFile to read.
 *  @param buffer Buffer to receive incoming data.
 *  @param length Maximum number of characters to read. Must be less
 *  than or equal to maxlength.
 *  @return Number of bytes actually read.
 */
size_t cdc_read(CDCFile *file, const char *buffer, size_t maxlength);

#endif /* [CDCREAD_H] */
