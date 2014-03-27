/******************************************************************************
* Copyright (C) 2011 by Jonathan Appavoo, Boston University
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*****************************************************************************/
#include <stdio.h>

#include "machine.h"
#include "mem.h"
#include "instruction.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "interrupt.h"
#include "misc.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "SiddBran",
    /* First member's full name */
    "Brandon Charlesworth",
    /* First member's email address */
    "bjcworth",
    /* Second member's full name (leave blank if none) */
    "I worked on this portion alone",
    /* Second member's email address (leave blank if none) */
    "Please award partial credit if at all possible. I spent a very long time on this and am about 98% done. If I had more time I am sure I would finish."
};

int
loop(int count, struct machine *m)
{
  int rc = 1;
  unsigned int i = 0;

  if (count<0) return rc;

  while (1) {
    TRACE_LOOP(dump_cpu(m));
    rc = interrupts(m);

    rc = fetch(m);
    if (rc < 0 ) break;
    rc = decode(m);
    if (rc < 0 ) break;
    rc = execute(m);
    if (rc < 0 ) break;
    i++;
    if (rc < 0 || (count && i == count)) break;
  }
  VPRINT("EXITING: count=%d i=%d\n",count, i);
  return rc;
}
