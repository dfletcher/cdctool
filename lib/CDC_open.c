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

#include "config.h"
#ifdef __WIN32__
  #include <windows.h>
#else
  #include <stdlib.h>
#endif
#include <CDC_open.h>
#include <CDC_baudrate.h>
#include <CDC_flowcontrol.h>
#include <CDC_parity.h>
#include <CDC_stopbits.h>
#include <CDC_wordsize.h>

CDCFile *cdc_open(const char *pathport) {

  // Allocate.
  CDCFile *rop;
  rop = (CDCFile*)malloc(sizeof(CDCFile));
  rop->file = 0;

  // Open for Windows.
  #ifdef __WIN32__

    DCB defaultparams;
    COMMTIMEOUTS timeouts = {0};

    // Open.
    rop->file = CreateFile(
      pathport, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
    );
    if(rop->file == INVALID_HANDLE_VALUE) {
      free(rop);
      return 0;
    }

    // Save initial state.
    rop->savedparams.DCBlength=sizeof(defaultparams);
    GetCommState(rop->file, &rop->savedparams);
    
    // Put the CDCFile in a known state.
    defaultparams.DCBlength=sizeof(defaultparams);
    GetCommState(rop->file, &defaultparams);
    defaultparams.BaudRate = CBR_57600;
    defaultparams.fBinary = 1;
    defaultparams.fParity = NOPARITY;
    defaultparams.fOutxCtsFlow = 0;
    defaultparams.fOutxDsrFlow = 0;
    defaultparams.fDtrControl = DTR_CONTROL_DISABLE;
    defaultparams.fDsrSensitivity = 0;
    defaultparams.fTXContinueOnXoff = 1;
    defaultparams.fOutX = 0;
    defaultparams.fInX = 0;
    defaultparams.fErrorChar = 0;
    defaultparams.fNull = 0;
    defaultparams.fRtsControl = RTS_CONTROL_DISABLE;
    defaultparams.fAbortOnError = 0;
    //defaultparams.XonLim = 1;
    //defaultparams.XoffLim = 1;
    defaultparams.Parity = NOPARITY;
    defaultparams.StopBits = ONESTOPBIT;
    defaultparams.fDtrControl = DTR_CONTROL_DISABLE;
    defaultparams.fDtrControl = DTR_CONTROL_DISABLE;
    defaultparams.fDtrControl = DTR_CONTROL_DISABLE;
    defaultparams.fDtrControl = DTR_CONTROL_DISABLE;
    defaultparams.fDtrControl = DTR_CONTROL_DISABLE;
    defaultparams.fDtrControl = DTR_CONTROL_DISABLE;
    //defaultparams.XonChar = 1;
    //defaultparams.XoffChar = 1;
    //defaultparams.ErrorChar = 1;
    //defaultparams.EofChar = 1;
    //defaultparams.EvtChar = 1;
    SetCommState(rop->file, &defaultparams);
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    SetCommTimeouts(rop->file, &timeouts);

  // Open for POSIX platforms.
  #else

    struct termios defaultparams;

    // Open.
    rop->file = open(pathport, O_RDWR | O_NOCTTY);
    if (rop->file < 0) {
      free(rop);
      return 0;
    }

    // Save initial state.
    tcgetattr(rop->file, &rop->savedparams);

    // Put the CDCFile in a known state.
    tcgetattr(rop->file, &defaultparams);
    defaultparams.c_cflag = B57600 | CS8 | CLOCAL | CREAD;
    defaultparams.c_iflag = IGNPAR | ICRNL;
    defaultparams.c_oflag = 0;
    defaultparams.c_lflag = ICANON;
    defaultparams.c_cc[VINTR]    = 0;
    defaultparams.c_cc[VQUIT]    = 0;
    defaultparams.c_cc[VERASE]   = 0;
    defaultparams.c_cc[VKILL]    = 0;
    defaultparams.c_cc[VEOF]     = 4;
    defaultparams.c_cc[VTIME]    = 0;
    defaultparams.c_cc[VMIN]     = 1;
    defaultparams.c_cc[VSWTC]    = 0;
    defaultparams.c_cc[VSTART]   = 0;
    defaultparams.c_cc[VSTOP]    = 0;
    defaultparams.c_cc[VSUSP]    = 0;
    defaultparams.c_cc[VEOL]     = 0;
    defaultparams.c_cc[VREPRINT] = 0;
    defaultparams.c_cc[VDISCARD] = 0;
    defaultparams.c_cc[VWERASE]  = 0;
    defaultparams.c_cc[VLNEXT]   = 0;
    defaultparams.c_cc[VEOL2]    = 0;
    tcflush(rop->file, TCIFLUSH);
    tcsetattr(rop->file, TCSANOW, &defaultparams);
  #endif

  // Default parameters.
  cdc_set_baudrate(rop, CDCBR_57600);
  cdc_set_wordsize(rop, CDCWORDSIZE_EIGHT);
  cdc_set_parity(rop, CDCPARITY_NOPARITY);
  cdc_set_stopbits(rop, CDCSTOPBITS_ONE);
  cdc_set_flowcontrol(rop, CDCFLOWCONTROL_NONE);

  return rop;
}
