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
#ifndef CDCDISCOVER_H
#define CDCDISCOVER_H

#include <stdlib.h>
#include <regex.h>

#ifndef CDCDISCOVER_NUM_CHECKS
/** This is the number of COM ports to check in Windows. Use
 *  -DCDCDISCOVER_NUM_CHECKS [n] to increase or decrease this
 *  at build time.
 */
#define CDCDISCOVER_NUM_CHECKS 64
#endif

/**
 *  Contains the results of a cdc_discover() query.
 */
typedef struct {
  char **match;
  size_t matches;
} CDCDiscoveryResults;

/**
 *  Send "testcommand" to each available virtual serial port,
 *  "regex" match the resulting string.
 *  @param testcommand Command to write to the serial device.
 *  @param regex Regular expression to match against the results.
 *  @param numlines Number of lines to read from the device. Each line is
 *         matched in turn, expressions that span multiple lines are not
 *         currently supported.
 *  @param numaddl Number of additional ports to try in the "addl" arg.
 *  @param addl Additional ports to attempt discovery on.
 *  @return The query results. Use cdc_discover_free_results() to dispose of
 *          this structure when done.
 */
CDCDiscoveryResults *cdc_discover(
  const char *testcommand, const char *regex, size_t numlines,
  size_t numaddl, const char **addl
);

/**
 *  Free the CDCDiscoveryResults when you are done.
 *  @param discover CDCDiscoveryResults object to dispose.
 */
void cdc_discover_free_results(CDCDiscoveryResults *discover);

#endif /* [CDCDISCOVER_H] */
