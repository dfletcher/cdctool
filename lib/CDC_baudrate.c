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

#include "CDC_baudrate.h"

void cdc_set_baudrate(CDCFile *file, CDCBaudRate baudrate) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    switch(baudrate) {
      case CDCBR_110: serialparams.BaudRate = CBR_110; break;
      case CDCBR_300: serialparams.BaudRate = CBR_300; break;
      case CDCBR_600: serialparams.BaudRate = CBR_600; break;
      case CDCBR_1200: serialparams.BaudRate = CBR_1200; break;
      case CDCBR_2400: serialparams.BaudRate = CBR_2400; break;
      case CDCBR_4800: serialparams.BaudRate = CBR_4800; break;
      case CDCBR_9600: serialparams.BaudRate = CBR_9600; break;
      case CDCBR_19200: serialparams.BaudRate = CBR_19200; break;
      case CDCBR_38400: serialparams.BaudRate = CBR_38400; break;
      case CDCBR_57600: serialparams.BaudRate = CBR_57600; break;
      case CDCBR_115200: serialparams.BaudRate = CBR_115200; break;
      default: serialparams.BaudRate = CBR_9600;
    }
    SetCommState(file->file, &serialparams);
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    tio.c_cflag &= ~B0; // unset B0
    tio.c_cflag &= ~B50; // unset B50
    tio.c_cflag &= ~B75; // unset B75
    tio.c_cflag &= ~B110; // unset B110
    tio.c_cflag &= ~B134; // unset B134
    tio.c_cflag &= ~B150; // unset B150
    tio.c_cflag &= ~B200; // unset B200
    tio.c_cflag &= ~B300; // unset B300
    tio.c_cflag &= ~B600; // unset B600
    tio.c_cflag &= ~B1200; // unset B1200
    tio.c_cflag &= ~B1800; // unset B1800
    tio.c_cflag &= ~B2400; // unset B2400
    tio.c_cflag &= ~B4800; // unset B4800
    tio.c_cflag &= ~B9600; // unset B9600
    tio.c_cflag &= ~B19200; // unset B19200
    tio.c_cflag &= ~B38400; // unset B38400
    tio.c_cflag &= ~B57600; // unset B57600
    tio.c_cflag &= ~B115200; // unset B115200
    tio.c_cflag &= ~B230400; // unset B230400
    switch(baudrate) {
      case CDCBR_110: tio.c_cflag |= B110; break;
      case CDCBR_300: tio.c_cflag |= B300; break;
      case CDCBR_600: tio.c_cflag |= B600; break;
      case CDCBR_1200: tio.c_cflag |= B1200; break;
      case CDCBR_2400: tio.c_cflag |= B2400; break;
      case CDCBR_4800: tio.c_cflag |= B4800; break;
      case CDCBR_9600: tio.c_cflag |= B9600; break;
      case CDCBR_19200: tio.c_cflag |= B19200; break;
      case CDCBR_38400: tio.c_cflag |= B38400; break;
      case CDCBR_57600: tio.c_cflag |= B57600; break;
      case CDCBR_115200: tio.c_cflag |= B115200; break;
      default: tio.c_cflag |= B9600;
    }
    tcsetattr(file->file, TCSANOW, &tio);
  #endif
} 

CDCBaudRate cdc_get_baudrate(CDCFile *file) {
  #ifdef __WIN32__
    DCB serialparams = {0};
    serialparams.DCBlength=sizeof(serialparams);
    GetCommState(file->file, &serialparams);
    switch(serialparams.BaudRate) {
      case CBR_110: return CDCBR_110;
      case CBR_300: return CDCBR_300;
      case CBR_600: return CDCBR_600;
      case CBR_1200: return CDCBR_1200;
      case CBR_2400: return CDCBR_2400;
      case CBR_4800: return CDCBR_4800;
      case CBR_9600: return CDCBR_9600;
      case CBR_19200: return CDCBR_19200;
      case CBR_38400: return CDCBR_38400;
      case CBR_57600: return CDCBR_57600;
      case CBR_115200: return CDCBR_115200;
      default: return CDCBR_9600;
    }
  #else
    struct termios tio;
    tcgetattr(file->file, &tio);
    if (tio.c_cflag & B110) return CDCBR_110;
    else if (tio.c_cflag & B300) return CDCBR_300;
    else if (tio.c_cflag & B600) return CDCBR_600;
    else if (tio.c_cflag & B1200) return CDCBR_1200;
    else if (tio.c_cflag & B2400) return CDCBR_2400;
    else if (tio.c_cflag & B4800) return CDCBR_4800;
    else if (tio.c_cflag & B9600) return CDCBR_9600;
    else if (tio.c_cflag & B19200) return CDCBR_19200;
    else if (tio.c_cflag & B38400) return CDCBR_38400;
    else if (tio.c_cflag & B57600) return CDCBR_57600;
    else if (tio.c_cflag & B115200) return CDCBR_115200;
    return CDCBR_9600;
  #endif
}

