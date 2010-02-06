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

#ifndef __WIN32__
  #include <glob.h>
#endif

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
  const char *CDCDISCOVER_BASE[] = {
    "COM",
    0 // Term.
  };
#else
  const char *CDCDISCOVER_BASE[] = {

    // TODO: what's missing?
    // TODO: put the platforms in #ifdefs, save a bit of time

    // Linux
    "/dev/ttyACM*",
    "/dev/ttyUSB*",
    "/dev/usb/tty/*",
    "/dev/usb/acm/*",

    // MacOSX
    "/dev/cu.USA*P*.1",
    "/dev/tty.USA*P*.1",

    // NetBSD
    "/dev/[tdc]tyU",

    0 // Term.
  };
#endif

void _cdc_discover(
  CDCDiscoveryResults *rop, CDCFile *file, size_t numlines,
  const char *path, const char *cmd, regex_t *cregex
) {
  int i;
  CDCLineBuffer *buf = cdc_filebuffer_new(file);
  cdc_write(file, cmd, strlen(cmd));
  cdc_write(file, "\r\n", 2);
  for (i=0; i<numlines; i++) {
    char *line = cdc_linebuffer_readline(buf);
    if (regexec(cregex, line, 0, 0, 0) == 0) {
      size_t newlen = rop->matches++;
      rop->match = realloc(rop->match, sizeof(char*) * rop->matches);
      rop->match[newlen] = cdc_string_copy(path);
    }
    cdc_linebuffer_freeline(line);
  }
  cdc_linebuffer_free(buf);
}

CDCDiscoveryResults *cdc_discover(
  const char *cmd, const char *regex, size_t numlines,
  size_t numaddl, const char **addl
) {
  int i, j;
  regex_t cregex;
  CDCFile *cdcfile;
  CDCDiscoveryResults *rop;
  #ifdef __WIN32__
    char namebuf[16];
  #else
    glob_t globbuf;
  #endif

  rop = (CDCDiscoveryResults*)malloc(sizeof(CDCDiscoveryResults));
  rop->matches = 0;
  rop->match = 0;

  if (regcomp(&cregex, regex, REG_NOSUB) != 0) {
    return rop;
  }

  for (i=0; CDCDISCOVER_BASE[i]; i++) {
    #ifdef __WIN32__
      for (j=0; j<CDCDISCOVER_NUM_CHECKS; j++) {
        sprintf(namebuf, "%s%d", CDCDISCOVER_BASE[i], j);
        cdcfile = cdc_open(namebuf);
        if (cdcfile) {
          _cdc_discover(rop, cdcfile, numlines, namebuf, cmd, &cregex);
          cdc_close(cdcfile);
        }
      }
    #else
      glob(CDCDISCOVER_BASE[i], GLOB_NOSORT|GLOB_TILDE, NULL, &globbuf);
      for (j=0; j<globbuf.gl_pathc; j++) {
        cdcfile = cdc_open(globbuf.gl_pathv[j]);
        _cdc_discover(
          rop, cdcfile, numlines, globbuf.gl_pathv[j], cmd, &cregex
        );
        cdc_close(cdcfile);
      }
      globfree(&globbuf);
    #endif
  }

  for (i=0; i<numaddl; i++) {
    cdcfile = cdc_open(addl[i]);
    if (cdcfile) {
      _cdc_discover(rop, cdcfile, numlines, addl[i], cmd, &cregex);
      cdc_close(cdcfile);
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
