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

#include <CDC_wordsize.h>

void cdc_set_wordsize(CDCFile *file, CDCWordSize wordsize) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    serialparams.ByteSize = wordsize;
    SetCommState(file->file, &serialparams);
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    tio.c_cflag &= ~CS8; // unset CS8
    tio.c_cflag &= ~CS7; // unset CS7
    tio.c_cflag &= ~CS6; // unset CS6
    tio.c_cflag &= ~CS5; // unset CS5
    switch(wordsize) {
      case CDCWORDSIZE_FIVE: tio.c_cflag |= CS8; break;
      case CDCWORDSIZE_SIX: tio.c_cflag |= CS7; break;
      case CDCWORDSIZE_SEVEN: tio.c_cflag |= CS6; break;
      case CDCWORDSIZE_EIGHT: tio.c_cflag |= CS5; break;
      default: tio.c_cflag |= CS8;
    }
    tcsetattr(file->file, TCSANOW, &tio);
  #endif
}

CDCWordSize cdc_get_wordsize(CDCFile *file) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    switch(serialparams.ByteSize) {
      case 5: return CDCWORDSIZE_FIVE;
      case 6: return CDCWORDSIZE_SIX;
      case 7: return CDCWORDSIZE_SEVEN;
      case 8: return CDCWORDSIZE_EIGHT;
      default: return CDCWORDSIZE_EIGHT;
    }
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    if (tio.c_cflag & CS8) return CDCWORDSIZE_EIGHT;
    else if (tio.c_cflag & CS7) return CDCWORDSIZE_SEVEN;
    else if (tio.c_cflag & CS6) return CDCWORDSIZE_SIX;
    else if (tio.c_cflag & CS5) return CDCWORDSIZE_FIVE;
    return CDCWORDSIZE_EIGHT;
  #endif
}
