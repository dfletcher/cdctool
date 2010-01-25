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
#ifndef CDCLINE_H
#define CDCLINE_H

#include "CDC_file.h"

/**
 *  Buffered CDCFile reader. The reading function returns one line at a time,
 *  separated by the \n newline character in input.
 */
typedef struct {
  CDCFile *file;
  char *buffer;
  size_t buffersize;
} CDCLineBuffer;

/**
 *  Initialize a CDCLineBuffer structure.
 *  @param buffer CDCLineBuffer to initialize.
 *  @param file CDCFile to buffer.
 */
CDCLineBuffer *cdc_linebuffer_new(CDCFile *file);

/**
 *  Read a single line. Blocks until a line is available.
 *  @param buffer CDCLineBuffer to read.
 */
char *cdc_linebuffer_readline(CDCLineBuffer *buffer);

/**
 *  Use this to dispose of lines read with cdc_linebuffer_readline().
 *  @param line Line string to dispose.
 */
void cdc_linebuffer_freeline(char *line);

/**
 *  Free the resources taken by a linebuffer. Does NOT close the associated
 *  CDCFile.
 *  @param buffer CDCLineBuffer to free.
 */
void cdc_linebuffer_free(CDCLineBuffer *buffer);

#endif /* [CDCLINE_H] */
