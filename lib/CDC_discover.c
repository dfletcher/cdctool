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
#include "CDC_discover.h"
#include "CDC_file.h"
#include "CDC_open.h"
#include "CDC_read.h"
#include "CDC_write.h"
#include "CDC_close.h"
#include "CDC_string.h"
#include "CDC_linebuffer.h"

#ifdef __WIN32__
  const char *CDCDISCOVER_BASES[] = {
    "COM",
    0
  };
#else
  const char *CDCDISCOVER_BASES[] = {
    // TODO: what's missing? MacOS?
    "/dev/ttyACM",
    "/dev/ttyUSB",
    "/dev/usb/acm",
    "/dev/usb/usb",
    0
  };
#endif

CDCDiscoveryResults *cdc_discover(
  const char *testcommand, const char *regex, size_t numlines
) {
  int i, j, k;
  regex_t cregex;
  CDCFile *cdcfile;
  CDCLineBuffer *buf;
  CDCDiscoveryResults *rop;
  char namebuffer[64];

  rop = (CDCDiscoveryResults*)malloc(sizeof(CDCDiscoveryResults));
  rop->matches = 0;
  rop->match = 0;

  if (regcomp(&cregex, regex, REG_NOSUB) != 0) {
    return rop;
  }

  for (i=0; CDCDISCOVER_BASES[i]; i++) {
    for (j=0; j<CDCDISCOVER_NUM_CHECKS; j++) {
      sprintf(namebuffer, "%s%d", CDCDISCOVER_BASES[i], j);
      cdcfile = cdc_open(namebuffer);
      if (cdcfile) {
        buf = cdc_filebuffer_new(cdcfile);
        cdc_write(cdcfile, testcommand, strlen(testcommand));
        cdc_write(cdcfile, "\r\n", 2);
        for (k=0; k<numlines; k++) {
          char *line = cdc_linebuffer_readline(buf);
          if (regexec(&cregex, line, 0, 0, 0) == 0) {
            size_t newlen = rop->matches++;
            rop->match = realloc(rop->match, sizeof(char*) * rop->matches);
            rop->match[newlen] = cdc_string_copy(namebuffer);
          }
          cdc_linebuffer_freeline(line);
        }
        cdc_linebuffer_free(buf);
        cdc_close(cdcfile);
      }
    }
  }

  regfree(&cregex);
  return rop;

}

void cdc_discover_free_results(CDCDiscoveryResults *discover) {
  int i;
  for (i=0; i<discover->matches; i++) {
    free(discover->match[i]);
  }
  free(discover->match);
  free(discover);
}
