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

#include <CDC_stopbits.h>

void cdc_set_stopbits(CDCFile *file, CDCStopBits stopbits) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    switch(stopbits) {
      case CDCSTOPBITS_ONE: serialparams.StopBits = ONESTOPBIT; break;
      case CDCSTOPBITS_TWO: serialparams.StopBits = TWOSTOPBITS; break;
      default: serialparams.StopBits = ONESTOPBIT;
    }
    SetCommState(file->file, &serialparams);
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    tio.c_cflag &= ~CSTOPB; // unset CSTOPB
    switch(stopbits) {
      case CDCSTOPBITS_ONE: break;
      case CDCSTOPBITS_TWO: tio.c_cflag |= CSTOPB; break;
      default: break;
    }
    tcsetattr(file->file, TCSANOW, &tio);
  #endif
}

CDCStopBits cdc_get_stopbits(CDCFile *file) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    switch(serialparams.StopBits) {
      case ONESTOPBIT: return CDCSTOPBITS_ONE;
      case TWOSTOPBITS: return CDCSTOPBITS_TWO;
      default: return CDCSTOPBITS_ONE;
    }
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    if (tio.c_cflag & CSTOPB) return CDCSTOPBITS_TWO;
    else return CDCSTOPBITS_ONE;
  #endif
}
