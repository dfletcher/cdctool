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

#ifndef __WIN32__
  #include <unistd.h>
#endif
#include "CDC_read.h"

size_t cdc_read(CDCFile *file, const char *buffer, size_t maxlength) {

  // Read for Windows.
  #ifdef __WIN32__
    DWORD read = 0;
    ReadFile(file->file, (char*)buffer, maxlength, &read, NULL);
	return read;

  // Read for POSIX.
  #else
    return read(file->file, (void*)buffer, maxlength);
  #endif
}
