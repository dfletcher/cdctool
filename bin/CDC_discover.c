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
#include <stdio.h>
#include <popt.h>
#include <cdc.h>
#include "config.h"

FILE *in;
FILE *out;
FILE *err;

void usage(poptContext poptcxt, int exitcode, char *error, char *addl) {
  FILE *where = err;
  fprintf(where, "%s\n", PACKAGE_STRING);
  if (exitcode == 0) {
    // Special case... usage asked for ... output stream not error.
    where = out;
  }
  else if (exitcode == 2) {
    // Special case... error setting error stream, so use stderr.
    where = stderr;
  }
  poptPrintUsage(poptcxt, where, 0);
  fprintf(where, "Please send bug reports to <%s>.\n", PACKAGE_BUGREPORT);
  if (error) fprintf(where, "%s: %s\n", error, addl);
  exit(exitcode);
}

int main(int argc, const char **argv) {

  int i;
  char c;
  size_t numaddl = 0;
  int linestoread = 1;
  char *outpath = NULL;
  char *errpath = NULL;
  poptContext poptcxt;
  const char **addl = NULL;
  const char *regexp = NULL;
  const char *command = NULL;
  CDCDiscoveryResults *discovery;

  // I/O defaults.
  in = stdin;
  out = stdout;
  err = stderr;

  struct poptOption options[] = {

    // --output,-o
    {
      "output", 'o', POPT_ARG_STRING, &outpath, 0,
      "output file (DEFAULT=stdout)", "path|-"
    },

    // --error,-e
    {
      "error", 'e', POPT_ARG_STRING, &errpath, 0,
      "error file (DEFAULT=stderr)", "path|-"
    },

    // --lines,-l
    {
      "lines", 'l', POPT_ARG_INT, &linestoread, 0,
      "Number of lines to read from the serial port before exiting. Zero is the same as -r. (DEFAULT=1)"
    },

    // --help,-?
    POPT_AUTOHELP

    // end
    { NULL, 0, 0, NULL, 0 }

  };

  // Process popt options.
  poptcxt = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(
    poptcxt, "[OPTIONS]* <command> <regexp> (additional port)+"
  );
  if (argc < 2) {
    usage(poptcxt, 0, 0, 0);
  }
  while ((c = poptGetNextOpt(poptcxt)) >= 0) { ; }
  if (c < -1) {
    // An error occurred during option processing.
    fprintf(
      stderr, "%s: %s\n",
      poptBadOption(poptcxt, POPT_BADOPTION_NOALIAS), poptStrerror(c)
    );
    return 1;
  }

  // Check error file.
  if (errpath && strcmp(errpath, "-") != 0) {
    err = fopen(errpath, "a");
    if (!err) {
      usage(poptcxt, 2, "Cannot open error file path", errpath);
    }
  }

  // Check output file.
  if (outpath && strcmp(outpath, "-") != 0) {
    out = fopen(outpath, "a");
    if (!out) {
      usage(poptcxt, 3, "Cannot open output file path", outpath);
    }
  }

  // Get the command and regular expression query.
  command = poptGetArg(poptcxt);
  regexp = poptGetArg(poptcxt);
  if ((command == NULL) || (regexp == NULL)) {
    usage(
      poptcxt, 1, "Please specify", "<command> <regex>"
    );
  }

  // Additional ports to check.
  while (poptPeekArg(poptcxt)) {
    addl = (const char**)realloc(addl, (++numaddl) * sizeof(char*));
    addl[numaddl] = poptGetArg(poptcxt);
  }

  // Discovery.
  discovery = cdc_discover(command, regexp, linestoread, numaddl, addl);
  for (i=0; i<discovery->matches; i++) {
    fprintf(out, "%s\n", discovery->match[i]);
  }
  cdc_discover_free_results(discovery);

  // Cleanup.
  if (numaddl) {
    free(addl);
  }
  poptFreeContext(poptcxt);

  return 0;
}
