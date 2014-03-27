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
#include "misc.h"

int
fetch_oprands(struct machine *m)
{
  struct OPCodeDesc *op;
  struct AddressingModeDesc *am;

  op = &(OPCodeTable[m->cpu.ir]);
  am = op->am;

  return am->func(am, m);
}

int
fetch(struct machine *m)
{
  
  m->cpu.ir = mem_get(m, m->cpu.reg.pc);
  fetch_oprands(m);
#if 0
  VERBOSE("%d %d\n", dump_page(m, ADDR_PAGE(m->cpu.reg.pc)), dump_machine(m, MEM_NONE));
  dump_op(&OPCodeTable[m->cpu.ir]);
#endif
  return 1;
}
