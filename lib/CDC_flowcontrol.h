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
#ifndef CDCFLOWCONTROL_H
#define CDCFLOWCONTROL_H

#include "CDC_file.h"

/**
 *  Set of common flow control settings.
 */
typedef enum {
  CDCFLOWCONTROL_NONE = 0,
  CDCFLOWCONTROL_CRTSCTS = 1,
} CDCFlowControl;

/**
 *  Set the flow control of a CDCFile.
 *  @param file CDCFile to set flow control parameter.
 *  @param flowctrl Desired flow control.
 */
void cdc_set_flowcontrol(CDCFile *file, CDCFlowControl flowctrl);

/**
 *  Get the current flow control of a CDCFile.
 *  @param file CDCFile to get flow control parameter.
 *  @return Current flow control.
 */
CDCFlowControl cdc_get_flowcontrol(CDCFile *file);

#endif /* [CDCFLOWCONTROL_H] */
