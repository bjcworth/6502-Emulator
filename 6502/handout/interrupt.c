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
#include <assert.h>
#include <signal.h>

#include "machine.h"
#include "mem.h"
#include "interrupt.h"
#include "instruction.h"
#include "misc.h"

struct MachineDesc {
  struct machine *m;
  struct MachineDesc *next;
} *machines = NULL;

void
sighandler(int sig, siginfo_t *siginfo, void *context)
{
  struct MachineDesc *md = machines;
  
  while (md) {
    if (sig == SIGUSR1 )
      md->m->cpu.pendingNMI = 1;
    else if (sig == SIGUSR2 ) 
      md->m->cpu.pendingIRQ = 1;
    md=md->next;
  }
}

int 
int_init(struct machine *m)
{
  struct MachineDesc *md;
  struct sigaction setmask;
  
  // add this machine to the list of machines to post
  // interrupts too
  md = (struct MachineDesc *)malloc(sizeof(struct MachineDesc));

  if (md==NULL) return 0;
  
  md->m = m;
  md->next = machines;
  machines = md;

  if (md->next == NULL) {
    // if this is the first machine added to the list then
    // install signal handler
    sigemptyset( &setmask.sa_mask );
    
    setmask.sa_sigaction = sighandler;
    setmask.sa_flags     = 0;
    
    sigaction( SIGUSR1, &setmask, NULL );
    sigaction( SIGUSR2, &setmask, NULL );	 
  }
  return 1;
}

int 
interrupts(struct machine *m)
{
  if (m->cpu.pendingNMI && int_nmib(m)) m->cpu.pendingNMI=0;
  if (m->cpu.pendingIRQ && int_irqb(m)) m->cpu.pendingIRQ=0;
  return 1;
}

int
int_resb(struct machine *m)
{
  bzero((void *)&(m->cpu), sizeof(struct cpu));
  
  // values explicit set by hardware on reset
  SR_B_SET(m->cpu.reg.sr);   
  SR_D_CLR(m->cpu.reg.sr);
  SR_I_SET(m->cpu.reg.sr);

  ADDR_SET_LOW(m->cpu.reg.pc, mem_get(m,VEC_RESB_LOW));
  ADDR_SET_HIGH(m->cpu.reg.pc, mem_get(m,VEC_RESB_HIGH));

  TRACE_INT(fprintf(stderr, "RESET:\n")); 
  TRACE_INT(dump_machine(m, MEM_SPECIAL));
  return 1;
}

void
pushstate(struct machine *m)
{
  inst_push(m, ADDR_HIGH(m->cpu.reg.pc));
  inst_push(m, ADDR_LOW(m->cpu.reg.pc));
  inst_push(m, m->cpu.reg.sr);
}

void
int_RTI(struct machine *m)
{
  if (m->cpu.runningNMI==1) m->cpu.runningNMI=0;
}

// these functions get executed at top of loop
// as such pc is current set to next instruction to be executed
int
int_nmib(struct machine *m)
{
  // we explicit ensure that only one NMI is executing at once
  if (m->cpu.runningNMI==1) return 0;
  m->cpu.runningNMI=1;
  pushstate(m);
  ADDR_SET_LOW(m->cpu.reg.pc, mem_get(m,VEC_NMIB_LOW));
  ADDR_SET_HIGH(m->cpu.reg.pc, mem_get(m,VEC_NMIB_HIGH));
  SR_I_SET(m->cpu.reg.sr);
  TRACE_INT(fprintf(stderr, "NMI: "));
  TRACE_INT(dump_machine(m, MEM_SPECIAL));
  return 1;
}

int
int_irqb(struct machine *m)
{
  pushstate(m);
  ADDR_SET_LOW(m->cpu.reg.pc, mem_get(m, VEC_IRQB_LOW));
  ADDR_SET_HIGH(m->cpu.reg.pc, mem_get(m,VEC_IRQB_HIGH));
  SR_I_SET(m->cpu.reg.sr);
  TRACE_INT(fprintf(stderr, "IRQ: "));
  TRACE_INT(dump_machine(m, MEM_SPECIAL));
  return 1;
}

int
int_brkb(struct machine *m)
{
  NYI;
  VPRINT("BRK: %d\n", dump_reg(m));
  return 1;
}

