#ifndef __MISC_H__
#define __MISC_H__
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

//#define ENABLE_VERBOSE
//#define ENABLE_TRACE_LOOP
//#define ENABLE_TRACE_INIT
//#define ENABLE_TRACE_INT
//#define ENABLE_TRACE_INST
//#define ENABLE_TRACE_MEM
//#define ENABLE_TRACE_CON

#ifdef ENABLE_VERBOSE
#define VPRINT(fmt, ...) fprintf(stderr, "%s: " fmt, __func__, __VA_ARGS__)
//#define VERBOSE(stmt) { stmt; }
#else
#define VPRINT(...)
//#define VERBOSE(stmt)
#endif

#ifdef ENABLE_TRACE_LOOP
#define TRACE_LOOP(stmt) { stmt; }
#else
#define TRACE_LOOP(stmt)
#endif

#ifdef ENABLE_TRACE_INIT
#define TRACE_INIT(stmt) { stmt; }
#else
#define TRACE_INIT(stmt)
#endif

#ifdef ENABLE_TRACE_INT
#define TRACE_INT(stmt) { stmt; }
#else
#define TRACE_INT(stmt)
#endif

#ifdef ENABLE_TRACE_INST
#define TRACE_INST(stmt) { stmt; }
#else
#define TRACE_INST(stmt)
#endif

#ifdef ENABLE_TRACE_MEM
#define TRACE_MEM(stmt) { stmt; }
#else
#define TRACE_MEM(stmt)
#endif

#ifdef ENABLE_TRACE_CON
#define TRACE_CON(stmt) { stmt; }
#else
#define TRACE_CON(stmt)
#endif

#define NYI fprintf(stderr, "%s: NYI\n", __func__)


enum DumpMemDirective { MEM_NONE, MEM_ALL, MEM_SPECIAL };
int dump_machine(struct machine *m, enum DumpMemDirective md);

int dump_cpu(struct machine *m);
int dump_reg(struct machine *m);
int dump_mem(struct machine *m);
int dump_page(struct machine *m, byte page);
int dump_op(struct OPCodeDesc *op);

typedef struct {
    char *teamname; /* ID1+ID2 or ID1 */
    char *name1;    /* full name of first member */
    char *id1;      /* login ID of first member */
    char *name2;    /* full name of second member (if any) */
    char *id2;      /* login ID of second member */
} team_t;

#endif
