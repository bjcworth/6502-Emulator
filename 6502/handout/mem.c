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
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#include "machine.h"
#include "mem.h"
#include "instruction.h"
#include "misc.h"

// at the expense of memory we avoid
// searching data structures on load store
// path by maintaining a complete array of possible
// mappings
struct mem_device {
  void *devptr;
  memdev_load lfunc;
  memdev_store sfunc;
} *devmappings[MEMSIZE];

int 
mem_mappings_init(void)
{
  bzero(devmappings, sizeof(devmappings));
  return 1;
}

int
mem_map_device(address addr, void *devptr, memdev_load lfunc, memdev_store sfunc)
{
  struct mem_device *d;
 
  d = devmappings[addr];
  if (d != NULL) return 0;
  
  d = (struct mem_device *)malloc(sizeof(struct mem_device));
  if (d == NULL) return 0;
  d->devptr = devptr;
  d->lfunc = lfunc;
  d->sfunc = sfunc;
  
  devmappings[addr] = d;

  return 1;
}

int
mem_umap_device(address addr, void **devptr)
{
  struct mem_device *d;
 
  d = devmappings[addr];
  if (d == NULL) return 0;

  devmappings[addr] = NULL;
  *devptr = d->devptr;
  free(d);
  return 1;
}

int 
mem_map_load(struct machine *m, address addr, byte *v)
{
  struct mem_device *d;

  d = devmappings[addr];
  if (d) { 
    d->lfunc(d->devptr, m, v);
    return 1;
  }
  return 0;
}

int 
mem_map_store(struct machine *m, address addr, byte v)
{
  struct mem_device *d;

  d = devmappings[addr];
  if (d) {
    d->sfunc(d->devptr, m, v);
    return 1;
  }
  return 0;
}

int 
mem_img_load(int fd, struct machine *m)
{
  int rc, n=0;

  // preconditions
  assert(sizeof(m->memory) == MEMSIZE);

  while (n < MEMSIZE) {
    rc = read(fd, &(m->memory[n]), MEMSIZE - n);
    if (rc == 0 || (rc < 0 && errno != EINTR)) break;
    n += rc;
  }
  VPRINT("n=%d fd=%d\n", n, fd);
  
  // postconditions
  assert(n == MEMSIZE);
  return 1;
}

int 
mem_img_save(int fd, struct machine *m)
{
  int rc, n = 0;

  // preconditions
  assert(sizeof(m->memory) == MEMSIZE);

  while (n < MEMSIZE) {
    rc = write(fd, &(m->memory[n]), sizeof(m->memory) - n);
    if (rc == 0 || (rc < 0 && errno != EINTR)) break;
    n += rc;
  }
  VPRINT("n=%d fd=%d\n", n, fd);
  
  // postconditions
  assert(n == MEMSIZE);
  return 1;
}

