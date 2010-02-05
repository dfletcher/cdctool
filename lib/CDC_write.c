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
#include <CDC_write.h>

size_t cdc_write(CDCFile *file, const char *string, size_t length) {

  // Write for Windows.
  #ifdef __WIN32__
    DWORD written = 0;
    WriteFile(file->file, string, length, &written, NULL);
    return written;

  // Write for POSIX.
  #else
    return write(file->file, string, length);
  #endif
}
