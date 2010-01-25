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

#include <stdlib.h>
#include <string.h>
#include "CDC_linebuffer.h"
#include "CDC_read.h"

#define CDC_LINEBUFFER_READBUFFER_SIZE 1024

/**
 *  Allocate a new string and copy the contents of the original.
 *  @param original String to copy. Must be NULL terminated.
 *  @return String copy.
 */
char *_cdc_copy_string(char *original) {
  size_t origlen = strlen(original);
  char *rop = (char*)malloc(origlen+1);
  rop[origlen] = 0;
  strncpy(rop, original, origlen);
  return rop;
}

/**
 *  Allocate a new string and copy 'length' characters from the original.
 *  @param original String to copy.
 *  @param length Number of characters to copy.
 *  @return String copy.
 */
char *_cdc_copy_string_n(char *original, size_t length) {
  char *rop = (char*)malloc(length+1);
  strncpy(rop, original, length);
  rop[length] = 0;
  return rop;
}

/**
 *  Utility function. Find the position of a character in the string, searching
 *  from str[0] to str[length-1].
 *  @param str String to search.
 *  @param c Character to search for.
 *  @param length Maximum search length.
 *  @return Position of the character in the string, or -1 if the character was
 *  not found.
 */
int _cdc_linebuffer_strpos(char *str, char c, size_t length) {
  int i;
  for (i=0; i<length; i++) {
    if (str[i] == c) return i;
  }
  return -1;
}

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
  char *rop = _cdc_copy_string_n(buffer, count);
  if (count == 0) return _cdc_copy_string("");
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
  *destbuffer = (char*)realloc(*destbuffer, newsize+1);
  strncpy(*destbuffer+length, srcbuffer, count);
  (*destbuffer)[newsize] = '\0';
  return newsize;
}

CDCLineBuffer *cdc_linebuffer_new(CDCFile *file) {
  CDCLineBuffer *rop;
  rop = (CDCLineBuffer*)malloc(sizeof(CDCLineBuffer));
  rop->file = file;
  rop->buffer = NULL;
  rop->buffersize = 0;
  return rop;
}
#include <stdio.h>
char *cdc_linebuffer_readline(CDCLineBuffer *buffer) {

  char localbuf[CDC_LINEBUFFER_READBUFFER_SIZE];

  while(1) {

    // Read from the CDCFile.
    size_t bytesread = cdc_read(
      buffer->file, localbuf, CDC_LINEBUFFER_READBUFFER_SIZE
    );

    // Newlines in the buffer?
    int buffernlpos = _cdc_linebuffer_strpos(
      buffer->buffer, '\n', buffer->buffersize
    );

    // Newlines in the local buffer?
    int localbufnlpos = _cdc_linebuffer_strpos(localbuf, '\n', bytesread);

    // Case 1: buffer contains a newline. Shift the buffer down and return the
    // string up to that point.
    if (buffernlpos > 0) {
      char *string = _cdc_buffer_shift_down(
        buffer->buffer, buffernlpos+1, buffer->buffersize
      );
      buffer->buffer = (char*)realloc(buffer->buffer, buffernlpos+1);
      buffer->buffer[buffernlpos+1] = 0;
      return string;
    }

    // Case 2: line read contains a newline. Append the remaining chars to the
    // buffer and return the string up to that point.
    else if (localbufnlpos > 0) {
      char *string = _cdc_buffer_shift_down(
        localbuf, localbufnlpos+1, CDC_LINEBUFFER_READBUFFER_SIZE
      );
      buffer->buffersize = _cdc_buffer_append(
        &buffer->buffer, localbuf, bytesread-strlen(string), buffer->buffersize
      );
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
