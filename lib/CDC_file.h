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
#ifndef CDSTRUCT_H
#define CDSTRUCT_H

#ifdef __WIN32__
  #include <windows.h>
#else
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <termios.h>
  #include <stdio.h>
  #define _POSIX_SOURCE 1
#endif

/**
 *  Wraps platform specific serial communications file objects into a single
 *  cross platform structure. Never access the fields directly, use only the
 *  functions in this library to manipulate CDCFile*, and then your program
 *  should work on any platform. Sets default comm params to:
 *  57600, 8/n/1, no flow control.
 */
typedef struct {
  #ifdef __WIN32__
    /** Windows backed CDCFile. */
    HANDLE file;
    DCB savedparams;
  #else
    /** POSIX backed CDCFile. */
    int file;
    struct termios savedparams;
  #endif
} CDCFile;

#endif /* [CDSTRUCT_H] */
