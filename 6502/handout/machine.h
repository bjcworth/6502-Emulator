#ifndef __MACHINE_H__
#define __MACHINE_H__
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
// Derived based on information from following places
// http://archive.6502.org/books/mcs6500_family_programming_manual.pdf
// http://archive.6502.org/books/mcs6500_family_hardware_manual.pdf
// http://archive.6502.org/datasheets/mos_6500_mpu_nov_1985.pdf
// http://archive.6502.org/datasheets/wdc_w65c02s_oct_19_2010.pdf
// http://www.masswerk.at/6502/6502_instruction_set.html
// http://www.6502.org/tutorials/6502opcodes.html
// http://nesdev.parodius.com/6502.txt
// http://www.llx.com/~nparker/a2/index.html
// http://www.oxyron.de/html/opcodes02.html
//
// Nice summary of the 6502 can be found here:
// http://www.cpu-world.com/CPUs/650x/index.html
// http://www.cpu-world.com/Arch/650x.html
//
// NOTES:
// FROM: http://www.masswerk.at/6502/6502_instruction_set.html
// "Processor Stack:
//    LIFO, top down, 8 bit range, 0x0100 - 0x01FF 
//  Bytes, Words, Addressing:
//    8 bit bytes, 16 bit words in lobyte-hibyte representation (Little-Endian).
//   16 bit address range, operands follow instruction codes."


typedef unsigned short address;
typedef unsigned char  byte;
typedef byte instruction;

#define ADDR_LOW(addr)         ((byte)(0xFF & (addr)))
#define ADDR_HIGH(addr)        ((byte)((0xFF00 & (addr))>>8))
#define ADDR_PAGE(addr)         ADDR_HIGH(addr)
#define ADDR_SET_LOW(addr, b)  (addr = (((addr) & 0xFF00) | (address)(b)))
#define ADDR_SET_HIGH(addr, b) (addr = (((addr) & 0x00FF) | (((address)(b))<<8)))

#define BYTE_SIGN_BIT 7
#define BYTE_SIGN_MASK (1 << BYTE_SIGN_BIT)

#define STACK_PAGE 0x01

// programmer visible registers
struct registers {
  byte          ac;  // Accumulator A
  byte          y;   // Index Register Y
  byte          x;   // Index Register X
  address       pc;  // Program counter
  byte          sp;  // Stack pointer (0x01<SP>-0x01<SP>)
  byte          sr;  // (P)rocessor Status Register
                     // broken down into the following bit fields
                     // bit 7: N Negative/sign 1 = neg
                     // bit 6: V Overflow      1 = true
                     // bit 5: ignored/reserved (unspecifed value)
                     // bit 4: BRK Command,    1= BRK, 0 = IRQB
                     // bit 3: Decimal mode    1= true
                     // bit 2: IRQB disable    1 = disable
                     // bit 1: Zero            1= true
                     // bit 0: Carry           1 = true
};

struct cpu {
  struct registers reg;
  // internal registers used by cpu operation
  address      abr;     // address bus register
  byte         dbb;     // data bus buffer
  instruction  ir;      // instruction register
  struct DecodedInfo {
    void   *v1; // decoded value
    void   *v2;
  } decoded; 
  byte         pendingNMI;
  byte         runningNMI;
  byte         pendingIRQ;
  byte         pendingBRK;
};

#define SR_N_BIT 7
#define SR_V_BIT 6
#define SR_B_BIT 4
#define SR_D_BIT 3
#define SR_I_BIT 2
#define SR_Z_BIT 1
#define SR_C_BIT 0

#define SR_MASK(srbit) ( 1 << SR_ ## srbit ## _BIT )

#define SR_N_MASK SR_MASK(N)
#define SR_V_MASK SR_MASK(V)
#define SR_B_MASK SR_MASK(B)
#define SR_D_MASK SR_MASK(D)
#define SR_I_MASK SR_MASK(I)
#define SR_Z_MASK SR_MASK(Z)
#define SR_C_MASK SR_MASK(C)

#define SR_N(sr)     ((sr &  SR_N_MASK) != 0)
#define SR_N_SET(sr) (sr |=  SR_N_MASK)
#define SR_N_CLR(sr) (sr &= ~SR_N_MASK)

#define SR_V(sr)     ((sr &   SR_V_MASK) != 0)
#define SR_V_SET(sr) (sr |=  SR_V_MASK)
#define SR_V_CLR(sr) (sr &= ~SR_V_MASK)

#define SR_B(sr)     ((sr &   SR_B_MASK) != 0)
#define SR_B_SET(sr) (sr |=  SR_B_MASK)
#define SR_B_CLR(sr) (sr &= ~SR_B_MASK)

#define SR_D(sr)     ((sr &   SR_D_MASK) != 0)
#define SR_D_SET(sr) (sr |=  SR_D_MASK)
#define SR_D_CLR(sr) (sr &= ~SR_D_MASK)

#define SR_I(sr)     ((sr &   SR_I_MASK) != 0)
#define SR_I_SET(sr) (sr |=  SR_I_MASK)
#define SR_I_CLR(sr) (sr &= ~SR_I_MASK)

#define SR_Z(sr)     ((sr &   SR_Z_MASK) != 0)
#define SR_Z_SET(sr) (sr |=  SR_Z_MASK)
#define SR_Z_CLR(sr) (sr &= ~SR_Z_MASK)

#define SR_C(sr)     ((sr &   SR_C_MASK) != 0)
#define SR_C_SET(sr) (sr |=  SR_C_MASK)
#define SR_C_CLR(sr) (sr &= ~SR_C_MASK)


#define MEMSIZE (1 << (sizeof(address) * 8))

struct machine {
  byte       memory[MEMSIZE];
  struct cpu cpu;
};

#define VEC_NMIB_LOW  0xFFFA
#define VEC_NMIB_HIGH 0xFFFB
#define VEC_RESB_LOW  0xFFFC
#define VEC_RESB_HIGH 0xFFFD
#define VEC_BRK_LOW   0xFFFE
#define VEC_BRK_HIGH  0xFFFF
#define VEC_IRQB_LOW  VEC_BRK_LOW
#define VEC_IRQB_HIGH VEC_BRK_HIGH

#endif
