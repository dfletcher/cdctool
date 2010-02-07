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
  #include <string.h>
#endif
#include <CDC_linebuffer.h>
#include <CDC_string.h>
#include <CDC_read.h>

// NOTE: Previous versions used a bigger buffer but the new feature of
// supporting different types of streams was causing trouble. Specifically,
// stdin from a terminal won't return when newlines are encountered. It doesn't
// return until the buffer is filled. There is probably a POSIX friendly way to
// change this so that stdin read() calls will return when linebreaks are
// encountered, but just changing the buffer/read size to 1 is a simple way
// that should work on all platforms.
#define CDC_LINEBUFFER_READBUFFER_SIZE 1

/**
 *  Utility function. Shift bytes down in a buffer and return the part that
 *  was squeezed off the front.
 *  @param buffer Address of the buffer.
 *  @param count Amount to shift down.
 *  @param length Length of the buffer.
 *  @return A copy of the string squeezed off the front of the buffer.
 */
char *_cdc_buffer_shift_down(char *buffer, size_t count, size_t length) {
  int i, j;
  char *rop = cdc_string_copy_n(buffer, count);
  if (count == 0) return cdc_string_copy("");
  for (i=count, j=0; i < length; i++, j++) {
    buffer[j] = buffer[i];
  }
  return rop;
}

/**
 *  Utility function. Append part of one array to another, reallocating as
 *  needed.
 *  @param destbuffer Destination buffer. Will be reallocated.
 *  @param srcbuffer Source buffer. 'count' characters are copied from here.
 *  @param count Number of characters to copy.
 *  @param length Original length of destbuffer.
 *  @return New buffer size.
 */
size_t _cdc_buffer_append(
  char **destbuffer,  char *srcbuffer, size_t count, size_t length
) {
  size_t newsize = length + count;
  size_t term = newsize + 1;
  (*destbuffer) = (char*)realloc((*destbuffer), term);
  strncpy((*destbuffer)+length, srcbuffer, count);
  (*destbuffer)[newsize] = 0;
  return newsize;
}

/**
 * Implements CDCLineBufferReadInput() for CDCFile type.
 */
size_t _cdc_filebuffer_input(
  char *buffer, size_t maxlength, void *userdata
) {
  return cdc_read((CDCFile*)userdata, buffer, maxlength);
}

/**
 * Implements CDCLineBufferReadInput() for FILE* type.
 */
size_t _cdc_stdfilebuffer_input(
  char *buffer, size_t maxlength, void *userdata
) {
  return fread(buffer, 1, maxlength, (FILE*)userdata);
}

CDCLineBuffer *cdc_linebuffer_new(CDCLineBufferIn input, void *userdata) {
  CDCLineBuffer *rop;
  rop = (CDCLineBuffer*)malloc(sizeof(CDCLineBuffer));
  rop->input = input;
  rop->userdata = userdata;
  rop->buffer = NULL;
  rop->buffersize = 0;
  return rop;
}

CDCLineBuffer *cdc_filebuffer_new(CDCFile *input) {
  return cdc_linebuffer_new(_cdc_filebuffer_input, input);
}

CDCLineBuffer *cdc_stdfilebuffer_new(FILE *input) {
  return cdc_linebuffer_new(_cdc_stdfilebuffer_input, input);
}

CDCLineBuffer *cdc_stdinfilebuffer_new(void) {
  return cdc_linebuffer_new(_cdc_stdfilebuffer_input, stdin);
}

char *cdc_linebuffer_readline(CDCLineBuffer *buffer) {

  char localbuf[CDC_LINEBUFFER_READBUFFER_SIZE+1];
  
  localbuf[1] = 0;

  while(1) {
    // Read from the input callback.
    size_t bytesread = buffer->input(
      localbuf, CDC_LINEBUFFER_READBUFFER_SIZE, buffer->userdata
    );

    // Newlines in the buffer?
    int buffernlpos = cdc_string_pos(buffer->buffer, '\n', buffer->buffersize);

    // Newlines in the local buffer?
    int localbufnlpos = cdc_string_pos(localbuf, 10, bytesread);

    // Case 1: buffer contains a newline. Shift the buffer down and return the
    // string up to that point.
    if (buffernlpos >= 0) {
      char *string = _cdc_buffer_shift_down(
        buffer->buffer, buffernlpos+1, buffer->buffersize
      );
      buffer->buffer = (char*)realloc(buffer->buffer, buffernlpos+1);
      buffer->buffersize = buffernlpos;
      buffer->buffer[buffernlpos] = 0;
      return string;
    }

    // Case 2: line read contains a newline. Append the remaining chars to the
    // buffer, shift it down and return the string up to that point.
    else if (localbufnlpos >= 0) {
      char *string;
      
      if (buffer->buffersize == 0) {
        // Filter out empty strings.
	continue;
      }

      buffer->buffersize = _cdc_buffer_append(
        &buffer->buffer, localbuf, bytesread, buffer->buffersize
      );
      string = _cdc_buffer_shift_down(
        buffer->buffer, buffer->buffersize, buffer->buffersize
      );
      free(buffer->buffer);
      buffer->buffer = 0;
      buffer->buffersize = 0;
      return string;
    }

    // Case 3: neither buffer nor line read contains a newline. Append the whole
    // line to the buffer and contine the loop.
    else {
      buffer->buffersize = _cdc_buffer_append(
        &buffer->buffer, localbuf, bytesread, buffer->buffersize
      );
    }

  }

}

void cdc_linebuffer_freeline(char *line) {
  // NOTE: this is a bit odd to need to do this, but the library doing the
  // allocating needs to do the disposing as well, or segfault (at least, this
  // is the case in Windows/Cygwin).
  if (line) {
    free(line);
  }
}

void cdc_linebuffer_free(CDCLineBuffer *buffer) {
  if (buffer->buffer) {
    free(buffer->buffer);
  }
  free(buffer);
}
