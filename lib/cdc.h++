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

#ifndef CDCCPLUSPLUS_H
#define CDCCPLUSPLUS_H

#include <cdc.h>

namespace cdctool {

  /**
   *  Encapsulates all functions of CDCTool in a C++ class. Some implementation
   *  details are hidden by the C++ file. The static functions actually return
   *  a subclass of CDCFile. Because of this factory-like construction, the
   *  CDCFile class is not designed to be extended via C++ inheritance, instead
   *  you should keep one or more CDCFile members in your own classes and call
   *  CDCFile::open(...) to initialize.
   */
  class CDCFile {

    public:

      /**
       *  Open and return a CDCFile. Upon failure, NULL is returned.
       *  @see CDC_open.h
       */
      static CDCFile *open(const char *path);

      /**
       *  Set baud rate.
       *  @see CDC_baudrate.h
       */
      virtual void setBaudRate(::CDCBaudRate rate)=0;

      /**
       *  Set parity.
       *  @see CDC_parity.h
       */
      virtual void setParity(::CDCParity parity);

      /**
       *  Set stop bits.
       *  @see CDC_stopbits.h
       */
      virtual void setStopBits(::CDCStopBits stopbits);

      /**
       *  Set word size.
       *  @see CDC_wordsize.h
       */
      virtual void setWordSize(::CDCWordSize wordsize);

      /**
       *  Read from the file.
       *  @see CDC_read.h
       */
      virtual size_t read(const char *buffer, size_t maxlength);

      /**
       *  Read a line from the file up to the next newline ([\r]?\n). Removes
       *  the ending newline character and returns a copy of the string.
       *  Free it with freeline (not all platforms support object deletion
       *  across DLL/SO, so this class supplies a static function for doing
       *  it).
       *  @see CDC_read.h, CDC_linebuffer.h
       */
      virtual char *readline(void);

      /**
       *  Free a line when you're done with it.
       *  @see readline
       */
      static void freeline(char *line);

      /** Write to the file.
       *  @see CDC_write.h
       */
      virtual size_t write(const char *string, size_t length);
  };

};

#endif /* [CDCCPLUSPLUS_H] */
