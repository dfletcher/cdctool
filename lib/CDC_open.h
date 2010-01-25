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
#ifndef CDOPEN_H
#define CDOPEN_H

#include "CDC_file.h"

/**
 *  Open a CDC file. All CDCFiles are opened for both reading and writing so
 *  there is no explicit 'mode' argument like fopen(). Returns 0 if the CDCFile
 *  could not be opened. After opening, the serial comm params are set by
 *  default to 57600, 8/n/1.
 *  @param pathport In Windows, the name of a COM[n] port to open. In other
 *  platforms, path to a device file.
 *  @return Newly created CDCFile structure or 0 if the CDCFile could not be
 *  opened. Close with cdc_close() when you are done with this pointer.
 *  @see cdc_close
 */
CDCFile *cdc_open(const char *pathport);

#endif /* [CDOPEN_H] */
