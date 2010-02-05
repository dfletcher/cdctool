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

#include <CDC_flowcontrol.h>

void cdc_set_flowcontrol(CDCFile *file, CDCFlowControl flowctrl) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    switch(flowctrl) {
      case CDCFLOWCONTROL_NONE: serialparams.fOutxCtsFlow = 0; break;
      case CDCFLOWCONTROL_CRTSCTS: serialparams.fOutxCtsFlow = 1; break;
      default: serialparams.fOutxCtsFlow = 0;
    }
    SetCommState(file->file, &serialparams);
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    tio.c_cflag &= ~CRTSCTS; // unset CRTSCTS
    switch(flowctrl) {
      case CDCFLOWCONTROL_NONE: break;
      case CDCFLOWCONTROL_CRTSCTS: tio.c_cflag |= CRTSCTS; break;
      default: break;
    }
    tcsetattr(file->file, TCSANOW, &tio);
  #endif
}

CDCFlowControl cdc_get_flowcontrol(CDCFile *file) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    if (serialparams.fOutxCtsFlow) return CDCFLOWCONTROL_CRTSCTS;
    return CDCFLOWCONTROL_NONE;
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    if (tio.c_cflag & CRTSCTS) return CDCFLOWCONTROL_CRTSCTS;
    return CDCFLOWCONTROL_NONE;
  #endif
}
