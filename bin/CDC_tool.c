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

#include <popt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "cdc.h"

#define BUFFERSIZE 1024

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
  char *line;
  int doread = 1;
  int dowrite = 1;
  int version = 0;
  int istopbits = 1;
  int iwordsize = 8;
  int ispeed = 57600;
  int linestoread = 1;
  size_t bytessent;
  size_t bytesread;
  char *inpath = NULL;
  char *outpath = NULL;
  char *errpath = NULL;
  char buffer[BUFFERSIZE];
  char *sflow = "none";
  char *sparity = "none";
  const char *sportname = NULL;
  poptContext poptcxt;

  // CDCFile and parameters.
  CDCFile *file;
  CDCStopBits stop = CDCSTOPBITS_ONE;
  CDCParity parity = CDCPARITY_NOPARITY;
  CDCBaudRate speed = CDCBR_57600;
  CDCFlowControl flow = CDCFLOWCONTROL_NONE;
  CDCWordSize wordsize = CDCWORDSIZE_EIGHT;

  // I/O defaults.
  in = stdin;
  out = stdout;
  err = stderr;

  // Popt options.
  struct poptOption options[] = {

    // --baudrate,-b
    {
      "baudrate", 'b', POPT_ARG_INT, &ispeed, 0,  
      "port speed as baud rate (DEFAULT=57600)", "baud"
    },

    // --flow,-f
    {
      "flow", 'f', POPT_ARG_STRING, &sflow, 0,
      "flow control (DEFAULT=none)", "none|hw"
    },

    // --parity,-p
    {
      "parity", 'p', POPT_ARG_STRING, &sparity, 0,
      "parity (DEFAULT=none)", "none|odd|even"
    },

    // --stopbits,-s
    {
      "stopbits", 's', POPT_ARG_INT, &istopbits, 0,
      "stop bits <1|2> (DEFAULT=1)", "1|2"
    },

    // --wordsize,-w
    {
      "wordsize", 'w', POPT_ARG_INT, &iwordsize, 0,
      "word size (DEFAULT=8)", "5|6|7|8"
    },

    // --input,-i
    {
      "input", 'i', POPT_ARG_STRING, &inpath, 0,
      "input file (DEFAULT=stdin)", "path|-"
    },

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

    // --nowrite,-w
    {
      "nowrite", 't', 0, 0, 't',
      "Don't write anything, just wait for input. (DEFAULT=write)"
    },

    // --noread,-r
    {
      "noread", 'r', 0, 0, 'r',
      "Don't read anything, just write and exit. (DEFAULT=read)"
    },

    // --version,-v
    {
      "version", 'v', 0, 0, 'v',
      "Print the version number and exit. All other options are ignored."
    },

    // --lines,-l
    {
      "lines", 'l', POPT_ARG_INT, &linestoread, 0,
      "Number of lines to read from the serial port before exiting. Zero is the same as -r. (DEFAULT=1)"
    },

    // --version,-v
    {
      "version", 'v', 0, 0, 'v',
      "Print the version number and exit. All other options are ignored."
    },

    // --help,-?
    POPT_AUTOHELP

    // end
    { NULL, 0, 0, NULL, 0 }

  };

  // Process popt options.
  poptcxt = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(poptcxt, "[OPTIONS]* <port>");
  if (argc < 2) {
    usage(poptcxt, 0, 0, 0);
  }
  while ((c = poptGetNextOpt(poptcxt)) >= 0) {
    if (c == 't') {
      dowrite = 0;
    }
    else if (c == 'r') {
      doread = 0;
    }
    else if (c == 'v') {
      version = 1;
    }
  }
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

  // Check input file.
  if (inpath && strcmp(inpath, "-") != 0) {
    in = fopen(inpath, "r");
    if (!in) {
      usage(poptcxt, 4, "Cannot open input file path", inpath);
    }
  }

  // Integer speed -> CDCBaudRate
  switch(ispeed) {
    case 110: speed = CDCBR_110; break;
    case 300: speed = CDCBR_300; break;
    case 600: speed = CDCBR_600; break;
    case 1200: speed = CDCBR_1200; break;
    case 2400: speed = CDCBR_2400; break;
    case 4800: speed = CDCBR_4800; break;
    case 9600: speed = CDCBR_9600; break;
    case 19200: speed = CDCBR_19200; break;
    case 38400: speed = CDCBR_38400; break;
    case 57600: speed = CDCBR_57600; break;
    case 115200: speed = CDCBR_115200; break;
    default:
      sprintf(buffer, "%d", ispeed);
      usage(
        poptcxt, 5,
        "Unknown baud rate, use one of 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, or 115200.",
        buffer
      );
  }

  // String flow control -> CDCFlowControl
  if (strcasecmp(sflow, "none")==0) {
    flow = CDCFLOWCONTROL_NONE;
  }
  else if (strcasecmp(sflow, "hw")==0) {
    flow = CDCFLOWCONTROL_CRTSCTS;
  }
  else {
    usage(
      poptcxt, 6, "Unknown flow control setting, use 'hw' or 'none'", sflow
    );
  }

  // String parity -> CDCParity
  if (strcasecmp(sparity, "none")==0) {
    parity = CDCPARITY_NOPARITY;
  }
  else if (strcasecmp(sparity, "odd")==0) {
    parity = CDCPARITY_ODDPARITY;
  }
  else if (strcasecmp(sparity, "even")==0) {
    parity = CDCPARITY_EVENPARITY;
  }
  else {
    usage(
      poptcxt, 7,
      "Unknown parity setting, use 'none', 'odd', or 'even'", sparity
    );
  }

  // Integer stop bits -> CDCStopBits
  if (istopbits == 1) {
    stop = CDCSTOPBITS_ONE;
  }
  else if (istopbits == 2) {
    stop = CDCSTOPBITS_TWO;
  }
  else {
    sprintf(buffer, "%d", istopbits);
    usage(
      poptcxt, 8, "Unknown stop bits setting, use 1 or 2", buffer
    );
  }

  // Integer word size -> CDCWordSize
  if (iwordsize == 5) {
    wordsize = CDCWORDSIZE_FIVE;
  }
  else if (iwordsize == 6) {
    wordsize = CDCWORDSIZE_SIX;
  }
  else if (iwordsize == 7) {
    wordsize = CDCWORDSIZE_SEVEN;
  }
  else if (iwordsize == 8) {
    wordsize = CDCWORDSIZE_EIGHT;
  }
  else {
    sprintf(buffer, "%d", iwordsize);
    usage(
      poptcxt, 9, "Unknown word size setting, use 5, 6, 7, or 8", buffer
    );
  }

  // Version mode, print and exit.
  if (version) {
    fprintf(out, "%s\n", PACKAGE_STRING);
    exit(0);
  }
  
  // Get the port (or device file) name.
  sportname = poptGetArg(poptcxt);
  if ((sportname == NULL) || !(poptPeekArg(poptcxt) == NULL)) {
    usage(
      poptcxt, 1, "Specify a single port", ".e.g., /dev/ttyACM0, or COM5."
    );
  }

  // Open the port.
  file = cdc_open(sportname);
  if (!file) {
    usage(poptcxt, 10, "Cannot open port", (char*)sportname);
  }

  // Set all parameters.
  cdc_set_wordsize(file, wordsize);
  cdc_set_flowcontrol(file, flow);
  cdc_set_baudrate(file, speed);
  cdc_set_stopbits(file, stop);
  cdc_set_parity(file, parity);

  // Write.
  if (dowrite) {
    while (!feof(in)) {
      bytessent = 0;
      bytesread = fread(buffer, 1, BUFFERSIZE, in);
      while (bytessent != bytesread) {
        bytessent += cdc_write(file, buffer+bytessent, bytesread-bytessent);
      }
    }
  }

  // Read.
  if (doread) {
    CDCLineBuffer *b = cdc_linebuffer_new(file);
    for (i=0; i<linestoread; i++) {
      line = cdc_linebuffer_readline(b);
      fwrite(line, 1, strlen(line), out);
      cdc_linebuffer_freeline(line);
    }
    cdc_linebuffer_free(b);
  }

  // Close.
  cdc_close(file);
  poptFreeContext(poptcxt);

  exit(0);
}
