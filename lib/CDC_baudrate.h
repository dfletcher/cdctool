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
#ifndef CDCBAUDRATE_H
#define CDCBAUDRATE_H

#include "CDC_file.h"

/**
 *  Set of common baud rates.
 */
typedef enum {
  CDCBR_110    = 110,
  CDCBR_300    = 300,
  CDCBR_600    = 600,
  CDCBR_1200   = 1200,
  CDCBR_2400   = 2400,
  CDCBR_4800   = 4800,
  CDCBR_9600   = 9600,
  CDCBR_19200  = 19200,
  CDCBR_38400  = 38400,
  CDCBR_57600  = 57600,
  CDCBR_115200 = 115200,
} CDCBaudRate;

/**
 *  Set the baud rate of a CDCFile.
 *  @param file CDCFile to set baud rate parameter.
 *  @param baudrate Desired baud rate.
 */
void cdc_set_baudrate(CDCFile *file, CDCBaudRate baudrate);

/**
 *  Get the current baud rate of a CDCFile.
 *  @param file CDCFile to get baud rate parameter.
 *  @return Current baud rate.
 */
CDCBaudRate cdc_get_baudrate(CDCFile *file);

#endif /* [CDCBAUDRATE_H] */
