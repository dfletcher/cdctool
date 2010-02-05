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
#ifndef CDCLOSE_H
#define CDCLOSE_H

#include <CDC_file.h>

/**
 *  Close a CDCFile after using it. Also frees memory used by the pointer.
 *  @param file CDCFile to close. cdc_close() restores the serial paramters
 *  to the state they were in when cdc_open() was called.
 *  @see cdc_open
 */
void cdc_close(CDCFile *file);

#endif /* [CDCLOSE_H] */
