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
#endif
#include <cdc.h++>

namespace cdctool {

  class _CDCFile : public CDCFile {
    private:
      ::CDCFile *base;
      ::CDCLineBuffer *linebuf;
    public:
      _CDCFile(::CDCFile *base) {
        this->base = base;
        this->linebuf = cdc_filebuffer_new(this->base);
      }
      ~_CDCFile(void) {
        ::cdc_close(this->base);
        ::cdc_linebuffer_free(this->linebuf);
      }
      void setBaudRate(::CDCBaudRate rate) {
        ::cdc_set_baudrate(this->base, rate);
      }
      void setParity(::CDCParity parity) {
        ::cdc_set_parity(this->base, parity);
      }
      void setStopBits(::CDCStopBits stopbits) {
        ::cdc_set_stopbits(this->base, stopbits);
      }
      void setWordSize(::CDCWordSize wordsize) {
        ::cdc_set_wordsize(this->base, wordsize);
      }
      size_t read(const char *string, size_t maxlength) {
        return ::cdc_read(this->base, string, maxlength);
      }
      char *readline(void) {
        return cdc_linebuffer_readline(this->linebuf);
      }
      size_t write(const char *string, size_t length) {
        return ::cdc_write(this->base, string, length);
      }
  };

  CDCFile *CDCFile::open(const char *path) {
    ::CDCFile *f;
    f = ::cdc_open(path);
    if (f) {
      return new _CDCFile(f);
    }
    else {
      return NULL;
    }
  }

  static void freeline(char *line) {
    ::cdc_linebuffer_freeline(line);
  }

};
