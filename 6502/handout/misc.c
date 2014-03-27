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
#include "misc.h"

static int dump_cnt=0;

int 
dump_sr(byte sr)
{
  fprintf(stderr, "sr: 0x%02x: N=%01d V=%01d B=%01d D=%01d I=%01d Z=%01d C=%01d\n", sr,
	  SR_N(sr), SR_V(sr), SR_B(sr), SR_D(sr), 
	  SR_I(sr), SR_Z(sr), SR_C(sr));
  return 1;
}

int 
dump_reg(struct machine *m)
{
  int rc = dump_cnt;
  fprintf(stderr, "ac: 0x%02x y: 0x%02x x: 0x%02x pc: 0x%02x,0x%02x, sp: 0x%02x\n",
	  m->cpu.reg.ac, m->cpu.reg.y, m->cpu.reg.x, ADDR_HIGH(m->cpu.reg.pc),
	  ADDR_LOW(m->cpu.reg.pc), m->cpu.reg.sp);
  dump_sr(m->cpu.reg.sr);
  dump_cnt++;
  return rc;
}

int 
dump_cpu(struct machine *m)
{
  int rc = dump_cnt;

  dump_reg(m);
  fprintf(stderr, "abr=0x%04x dbb=0x%04x ir=0x%02x (%s)\n",
	  m->cpu.abr, m->cpu.dbb, m->cpu.ir, 
	  inst_memonic(m->cpu.ir)); 

  dump_cnt++;
  return rc;
}
 
int 
dump_page(struct machine *m, byte page)
{
  int rc = dump_cnt;
  byte offset;
  address addr=0;

  ADDR_SET_HIGH(addr, page);


  fprintf(stderr, "0x%04x:\n", addr);

  fprintf(stderr, "    00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n"); 
  fprintf(stderr, "    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --\n"); 
  for (offset = 0; offset != 0xFF; offset++) {
    ADDR_SET_LOW(addr, offset);
    if ((offset & 0x0F) == 0x00) fprintf(stderr, "%02x: ", offset);
    // we avoid memory mapped devices and read memory array directly
    fprintf(stderr, "%02x", m->memory[addr]); 
    if ((offset & 0x0F) == 0x0F) fprintf(stderr, "\n");
    else fprintf(stderr, " ");
  }
  ADDR_SET_LOW(addr, offset);
  // we avoid memory mapped devices and read memory array directly
  fprintf(stderr, "%02x\n", m->memory[addr]);
  dump_cnt++;
  return rc;
}

int
dump_mem(struct machine *m)
{
  int rc = dump_cnt;
  byte page;

  for (page = 0; page != 0xFF; page++) 
    dump_page(m, page);
  
  dump_page(m, page);

  dump_cnt++;
  return rc;
}

int
dump_machine(struct machine *m, enum DumpMemDirective md)
{
  int rc = dump_cnt;

  dump_cpu(m);

  if (md==MEM_ALL) dump_mem(m);
  else if (md==MEM_SPECIAL) {
    dump_page(m, 0x00);
    dump_page(m, 0x01);
    dump_page(m, 0xFF);
  }

  dump_cnt++;
  return rc;
}

int 
dump_am(struct AddressingModeDesc *am)
{
  int rc=dump_cnt;

  fprintf(stderr, "mode=%d, func=%p, %s", am->mode, am->func, am->desc);

  dump_cnt++;
  return rc;
}

int
dump_inst(struct InstructionDesc *i)
{
  int rc=dump_cnt;
  
  fprintf(stderr, "inst=%d, %s, %s", i->inst, i->memonic, i->description);

  dump_cnt++;
  return rc;
}

int
dump_op(struct OPCodeDesc *op)
{
  int rc=dump_cnt;

  if (!op) fprintf(stderr, "op=NULL\n");
  else {
    fprintf(stderr, "op=0x%02x am=%p \n(", op->op, op->am);
    dump_am(op->am);
    fprintf(stderr, ")\nsrbits=0x%02x, instfunc=%p, ins=%p\n(",
	    op->SRBits, op->func, op->ins);
    dump_inst(op->ins);
    fprintf(stderr, ")\n");
  }
  dump_cnt++;
  return rc;
}
