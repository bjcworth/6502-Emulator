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
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "machine.h"
#include "mem.h"
#include "instruction.h"
#include "interrupt.h"
#include "misc.h"
#include "tty.h"
#include "console.h"


struct console
{
  address addr;
  int last_store_val;
  int num_store;
  int last_load_val;
  int num_load;
  int load_fd;
  int store_fd;
};


static void
console_store(void *ptr, struct machine *m, byte v) __attribute__ ((unused));
static void
console_load(void *ptr, struct machine *m, byte *v) __attribute__ ((unused));


static void
console_store(void *ptr, struct machine *m, byte v)
{
  int rc;
  struct console *c;

  assert(ptr);
  c = ptr;

  rc = write(c->store_fd, &v, 1);
  if (rc == 1) {
    c->last_store_val = v;
    c->num_store++;
  }
}

static void
console_load(void *ptr, struct machine *m, byte *v)
{
  int rc;
  struct console *c;

  assert(ptr);
  c = ptr;
  rc = read(c->load_fd, &(c->last_load_val), 1);
  if (rc == 1) {
   c->num_load++;
   *v = c->last_load_val;
  } else {
    *v = 0;
  }
}

int
console_init(struct machine *m)
{
  struct console *c;

  c = (struct console *)malloc(sizeof(struct console));
  if (c==NULL) return 0;

  bzero(c, sizeof(struct console));
  c->addr = CONSOLE_ADDR;
  c->load_fd = STDIN_FILENO;
  c->store_fd = STDOUT_FILENO;
  tty_init(c->store_fd);
  mem_map_device(c->addr, c, console_load, console_store);



  return 1;

}
