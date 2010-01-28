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

#include <stdio.h>
#include "CDC_file.h"

/**
 *  Callback for CDCLineBuffer input.
 */
typedef size_t(*CDCLineBufferIn)(char *buf, size_t length, void *userdat);

/**
 *  Buffered line reader. The reading function returns one line at a time,
 *  separated by the \n newline character in input.
 */
typedef struct {
  CDCLineBufferIn input;
  void *userdata;
  char *buffer;
  size_t buffersize;
} CDCLineBuffer;

/**
 *  Initialize a CDCLineBuffer structure. This is a generic function with a
 *  source neutral input function. Use cdc_filebuffer_new() or
 *  cdc_stdfilebuffer_new() to create line buffers backed by one of those two
 *  common types of input stream.
 *  @param input Input callback function.
 *  @param userdata User object passed to the callback.
 *  @return New CDCLineBuffer. Use cdc_linebuffer_free() when you're done.
 */
CDCLineBuffer *cdc_linebuffer_new(CDCLineBufferIn input, void *userdata);

/**
 *  Initialize a CDCLineBuffer structure for use with a CDCFile. Provides the
 *  necessary callback internally.
 *  @param input CDCFile to use for input.
 *  @return New CDCLineBuffer. Use cdc_linebuffer_free() when you're done.
 */
 CDCLineBuffer *cdc_filebuffer_new(CDCFile *input);

/**
 *  Initialize a CDCLineBuffer structure for use with a stdio FILE pointer.
 *  Provides the necessary callback internally.
 *  @param input FILE* to use for input.
 *  @return New CDCLineBuffer. Use cdc_linebuffer_free() when you're done.
 */
CDCLineBuffer *cdc_stdfilebuffer_new(FILE *f);

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
 *  stream.
 *  @param buffer CDCLineBuffer to free.
 */
void cdc_linebuffer_free(CDCLineBuffer *buffer);

#endif /* [CDCLINE_H] */
