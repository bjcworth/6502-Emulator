#ifndef __MEM_H__
#define __MEM_H__
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

// memory image loading and saving
int mem_img_load(int fd, struct machine *m);
int mem_img_save(int fd, struct machine *m);

// memory mapping interfaces
typedef void (*memdev_load)(void *, struct machine *m, byte *v);
typedef void (*memdev_store)(void *, struct machine *m, byte v);

int mem_mappings_init(void);
int mem_map_device(address addr, void *devptr, memdev_load lfunc, memdev_store sfunc);
int mem_unmap_device(address addr, void **devptr);
int mem_map_load(struct machine *m, address addr, byte *v);
int mem_map_store(struct machine *m, address addr, byte v);

// memory operators
// operators on memory

static inline byte mem_get(struct machine *m, address a) 
{
  mem_map_load(m, a, &(m->memory[a])); // allow any mapped device to 
                                    // update memory array     
  return m->memory[a];        // return value from memory array
}

static inline void mem_put(struct machine *m, address a, byte v) 
{
  mem_map_store(m, a, v); // allow any memory mapped device to
                          // to store the value
  m->memory[a] = v;          // store data  in memory array
}
    
#endif
