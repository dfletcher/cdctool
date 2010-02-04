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

#include "CDC_parity.h"

void cdc_set_parity(CDCFile *file, CDCParity parity) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    switch(parity) {
      case CDCPARITY_NOPARITY: serialparams.Parity = NOPARITY; break;
      case CDCPARITY_ODDPARITY: serialparams.Parity = ODDPARITY; break;
      case CDCPARITY_EVENPARITY: serialparams.Parity = EVENPARITY; break;
      default: serialparams.Parity = NOPARITY;
    }
    SetCommState(file->file, &serialparams);
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    tio.c_cflag &= ~PARENB; // unset PARENB
    tio.c_cflag &= ~PARODD; // unset PARODD
    switch(parity) {
      case CDCPARITY_NOPARITY: break;
      case CDCPARITY_EVENPARITY: tio.c_cflag |= PARENB; break;
      case CDCPARITY_ODDPARITY:
        tio.c_cflag |= PARENB;
        tio.c_cflag |= PARODD;
      break;
      default: break;
    }
    tcsetattr(file->file, TCSANOW, &tio);
  #endif
}

CDCParity cdc_get_parity(CDCFile *file) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    switch(serialparams.Parity) {
      case NOPARITY: return CDCPARITY_NOPARITY;
      case ODDPARITY: return CDCPARITY_ODDPARITY;
      case EVENPARITY: return CDCPARITY_EVENPARITY;
      default: return CDCPARITY_NOPARITY;
    }
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    if (!(tio.c_cflag & PARENB)) return CDCPARITY_NOPARITY;
    if (tio.c_cflag & PARODD) return CDCPARITY_ODDPARITY;
    return CDCPARITY_EVENPARITY;
  #endif
}
