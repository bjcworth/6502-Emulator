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

#include "machine.h"
#include "mem.h"
#include "instruction.h"
#include "interrupt.h"
#include "misc.h"

static void inst_load(struct machine *m) __attribute__ ((unused));
static void inst_store(struct machine *m) __attribute__ ((unused));

static void
inst_load(struct machine *m)
{
  m->cpu.dbb = mem_get(m, m->cpu.abr);
}

static void
inst_store(struct machine *m)
{
  mem_put(m, m->cpu.abr, m->cpu.dbb);
}
static int
inst_am_inv(struct AddressingModeDesc *mode, struct machine *m)
{
  NYI; assert(0);
  return 1;
}

static int
inst_am_acc(struct AddressingModeDesc *mode, struct machine *m)
{
 m->cpu.reg.pc++;
 return 1;

}

static int
inst_am_abs(struct AddressingModeDesc *mode, struct machine *m)
{

  ADDR_SET_LOW(m->cpu.abr, mem_get(m, m->cpu.reg.pc+ 1));
  
  ADDR_SET_HIGH(m->cpu.abr, mem_get(m, m->cpu.reg.pc+ 2));

  m->cpu.reg.pc += 3;
  return 1;
  
}

static int
inst_am_absx(struct AddressingModeDesc *mode, struct machine *m)
{

  
  ADDR_SET_LOW(m->cpu.abr, mem_get(m, m->cpu.reg.pc+ 1));
  
  ADDR_SET_HIGH(m->cpu.abr, mem_get(m, m->cpu.reg.pc+ 2));

 
  m->cpu.abr += m->cpu.reg.x;
  m->cpu.reg.pc += 3;
  return 1;

}

static int
inst_am_absy(struct AddressingModeDesc *mode, struct machine *m)
{
 
  ADDR_SET_LOW(m->cpu.abr, mem_get(m, m->cpu.reg.pc+ 1));
  
  ADDR_SET_HIGH(m->cpu.abr, mem_get(m, m->cpu.reg.pc+ 2));

 
  m->cpu.abr += m->cpu.reg.y;
  m->cpu.reg.pc += 3;
  return 1;

}

static int
inst_am_imm(struct AddressingModeDesc *mode, struct machine *m)
{

  m->cpu.abr = m->cpu.reg.pc + 1;

  m->cpu.reg.pc += 2;

  return 1;
}

static int
inst_am_impl(struct AddressingModeDesc *mode, struct machine *m)
{

  m->cpu.reg.pc++;
  return 1;
}

static int
inst_am_ind(struct AddressingModeDesc *mode, struct machine *m)
{
  NYI; assert(0);
  return 1;
}

static int
inst_am_xind(struct AddressingModeDesc *mode, struct machine *m)
{
  NYI; assert(0);
  return 1;
}

static int
inst_am_indy(struct AddressingModeDesc *mode, struct machine *m)
{
  NYI; assert(0);
  return 1;
}

static int
inst_am_rel(struct AddressingModeDesc *mode, struct machine *m)
{
  
  m->cpu.abr = m->cpu.reg.pc + 1;

  m->cpu.reg.pc += 2;

  return 1;


}

static int
inst_am_zp(struct AddressingModeDesc *mode, struct machine *m)
{



  ADDR_SET_LOW(m->cpu.abr, mem_get(m,(m->cpu.reg.pc + 1)));
  ADDR_SET_HIGH(m->cpu.abr, mem_get(m,0x00));
  m->cpu.reg.pc += 2;
  return 1; 
}

static int
inst_am_zpx(struct AddressingModeDesc *mode, struct machine *m)
{

  ADDR_SET_LOW(m->cpu.abr, mem_get(m,(m->cpu.reg.pc + 1)));
  ADDR_SET_HIGH(m->cpu.abr, mem_get(m,0x00));
  m->cpu.abr += m->cpu.reg.x;
  m->cpu.reg.pc += 2;
  return 1;
  

}

static int
inst_am_zpy(struct AddressingModeDesc *mode, struct machine *m)
{
  NYI; assert(0);
  return 1;
}

static int
inst_INV(struct OPCodeDesc *op, struct machine *m)
{ NYI; dump_op(op); dump_reg(m); assert(0); return 0;}

static int
inst_ADC(struct OPCodeDesc *op, struct machine *m)
{

 
  inst_load(m);
  byte val = m->cpu.dbb;
  byte temp = m->cpu.reg.ac + (val + SR_C(m->cpu.reg.sr)); 


 if (SR_D(m->cpu.reg.sr)) {
		NYI;

		}

  SR_Z_CLR(m->cpu.reg.sr);
  SR_V_CLR(m->cpu.reg.sr);
  SR_C_CLR(m->cpu.reg.sr);
  SR_N_CLR(m->cpu.reg.sr);


  if( temp > 0xFF )
	SR_C_SET(m->cpu.reg.sr);




  if (!((m->cpu.reg.ac ^ val) & BYTE_SIGN_MASK) && ((m->cpu.reg.ac ^ temp) & BYTE_SIGN_MASK))
	SR_V_SET(m->cpu.reg.sr);
	
m->cpu.reg.ac = temp & 0xFF;

  if( m->cpu.reg.ac & BYTE_SIGN_MASK )
	SR_N_SET(m->cpu.reg.sr);

	
  if(m->cpu.reg.ac == 0)
	SR_Z_SET(m->cpu.reg.sr);

  return 1;
	}
	

static int
inst_AND(struct OPCodeDesc *op, struct machine *m)
{

  inst_load(m);
  m->cpu.reg.ac &= m->cpu.dbb;

  if (m->cpu.reg.ac == 0)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  if (m->cpu.reg.ac & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);


  return 1;


  
}

static int
inst_ASL(struct OPCodeDesc *op, struct machine *m)
{
if (op->am->mode == ACC) {
    
	if(m->cpu.reg.ac & BYTE_SIGN_MASK)
		SR_C_SET(m->cpu.reg.sr);
	else
		SR_C_CLR(m->cpu.reg.sr);
		
    m->cpu.reg.ac <<= 1;
    m->cpu.reg.ac &= 0xFF;

    if(m->cpu.reg.ac & BYTE_SIGN_MASK) {
  
       SR_N_SET(m->cpu.reg.sr);
     } else 
	 
       SR_N_CLR(m->cpu.reg.sr);
	   
     

     if (m->cpu.reg.ac == 0)
       SR_Z_SET(m->cpu.reg.sr);
     
     else
       SR_Z_CLR(m->cpu.reg.sr);
     

     return 1;

  }
  else {

     inst_load(m);
	 if(m->cpu.dbb & BYTE_SIGN_MASK)
		SR_C_SET(m->cpu.reg.sr);
	 else
		SR_C_CLR(m->cpu.reg.sr);
	 
     m->cpu.dbb <<= 1;
     m->cpu.dbb &= 0xFF;

     if(m->cpu.dbb & BYTE_SIGN_MASK) {
  
       SR_N_SET(m->cpu.reg.sr); }
     else
    
       SR_N_CLR(m->cpu.reg.sr);
     
     if (m->cpu.dbb == 0)
       SR_Z_SET(m->cpu.reg.sr);
     
     else
       SR_Z_CLR(m->cpu.reg.sr);
     
   inst_store(m);
   return 1;
  } 

 
}

static int
inst_BCC(struct OPCodeDesc *op, struct machine *m)
{
  if (!SR_C(m->cpu.reg.sr)){
	inst_load(m);
    m->cpu.reg.pc += (signed char) m->cpu.dbb;
	}
  return 1; 

}

static int
inst_BCS(struct OPCodeDesc *op, struct machine *m)
{
  if (SR_C(m->cpu.reg.sr)){
	inst_load(m);
        m->cpu.reg.pc += (signed char) m->cpu.dbb;
	}
  return 1; 

}

static int
inst_BEQ(struct OPCodeDesc *op, struct machine *m)
{
  if (SR_Z(m->cpu.reg.sr)){
	inst_load(m);
        m->cpu.reg.pc += (signed char) m->cpu.dbb;
	}
  return 1;

}

static int
inst_BIT(struct OPCodeDesc *op, struct machine *m)
{

  inst_load(m);
  byte amp = m->cpu.dbb & m->cpu.reg.ac;

  if (m->cpu.dbb  & BYTE_SIGN_MASK) 
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);

  if (m->cpu.dbb  & (BYTE_SIGN_MASK-1)) 
    SR_V_SET(m->cpu.reg.sr);
  else
    SR_V_CLR(m->cpu.reg.sr);

  if (amp == 0x00) 
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  return 1;



}

static int
inst_BMI(struct OPCodeDesc *op, struct machine *m)
{

	if(SR_N(m->cpu.reg.sr)){
		inst_load(m);
		m->cpu.reg.pc += (signed char) m->cpu.dbb;
	}
	return 1;



}

static int
inst_BNE(struct OPCodeDesc *op, struct machine *m)
{
if (!SR_Z(m->cpu.reg.sr)){
		inst_load(m);
		m->cpu.reg.pc += (signed char) m->cpu.dbb;
}
return 1;

}

static int
inst_BPL(struct OPCodeDesc *op, struct machine *m)
{
	if(!SR_N(m->cpu.reg.sr)){
		inst_load(m);
		m->cpu.reg.pc += (signed char) m->cpu.dbb;
	}
	return 1;

}

static int
inst_BRK(struct OPCodeDesc *op, struct machine *m)
{
  return -1;
}

static int
inst_BVC(struct OPCodeDesc *op, struct machine *m)
{
  if(!SR_V(m->cpu.reg.sr)){
		inst_load(m);
		m->cpu.reg.pc += (signed char) m->cpu.dbb;
	}
	return 1;


}

static int
inst_BVS(struct OPCodeDesc *op, struct machine *m)
{
  if(SR_V(m->cpu.reg.sr)){
		inst_load(m);
		m->cpu.reg.pc += (signed char) m->cpu.dbb;
	}
	return 1;


}

static int
inst_CLC(struct OPCodeDesc *op, struct machine *m)
{

  SR_C_CLR(m->cpu.reg.sr);
  return 1;

}

static int
inst_CLD(struct OPCodeDesc *op, struct machine *m)
{
  SR_D_CLR(m->cpu.reg.sr);
  return 1;
}

static int
inst_CLI(struct OPCodeDesc *op, struct machine *m)
{
  SR_I_CLR(m->cpu.reg.sr);
  return 1;
}

static int
inst_CLV(struct OPCodeDesc *op, struct machine *m)
{
  SR_V_CLR(m->cpu.reg.sr);
  return 1;

}

static int
inst_CMP(struct OPCodeDesc *op, struct machine *m)
{
     inst_load(m);
	
	if (m->cpu.dbb <= m->cpu.reg.ac)
		SR_C_SET(m->cpu.reg.sr);
	else
		SR_C_CLR(m->cpu.reg.sr);

	if ((m->cpu.reg.ac - m->cpu.dbb) == 0)
		SR_Z_SET(m->cpu.reg.sr);
	else
		SR_Z_CLR(m->cpu.reg.sr);

	if ((m->cpu.reg.ac - m->cpu.dbb) & BYTE_SIGN_MASK)
		SR_N_SET(m->cpu.reg.sr);
	else
		SR_N_SET(m->cpu.reg.sr);
	return 1;

}

static int
inst_CPX(struct OPCodeDesc *op, struct machine *m)
{
  inst_load(m);

  if (m->cpu.dbb <=  m->cpu.reg.x)
    SR_C_SET(m->cpu.reg.sr);
  else
    SR_C_CLR(m->cpu.reg.sr);

  if ((m->cpu.reg.x - m->cpu.dbb) & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);

  if (m->cpu.dbb == m->cpu.reg.x)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  return 1;

}

static int
inst_CPY(struct OPCodeDesc *op, struct machine *m)
{
  inst_load(m);
  if (m->cpu.dbb <= m->cpu.reg.y)
    SR_C_SET(m->cpu.reg.sr);
  else
    SR_C_CLR(m->cpu.reg.sr);

  if ((m->cpu.reg.y - m->cpu.dbb) & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);

  if (m->cpu.dbb == m->cpu.reg.y)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  return 1;

}

static int
inst_DEC(struct OPCodeDesc *op, struct machine *m)
{
  m->cpu.dbb--;

  if (m->cpu.dbb & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);

  if (m->cpu.dbb == 0)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  inst_store(m);
  
  return 1;
}

static int
inst_DEX(struct OPCodeDesc *op, struct machine *m)
{


  m->cpu.reg.x--;
  if (m->cpu.reg.x & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);

  if (m->cpu.reg.x == 0)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  return 1;

}

static int
inst_DEY(struct OPCodeDesc *op, struct machine *m)
{

 m->cpu.reg.y--;
  if (m->cpu.reg.y & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);

  if (m->cpu.reg.y == 0)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  return 1;

}

static int
inst_EOR(struct OPCodeDesc *op, struct machine *m)
{

  inst_load(m);
  m->cpu.reg.ac ^= m->cpu.dbb;

  if (m->cpu.reg.ac == 0)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  if (m->cpu.reg.ac & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);


  return 1;

}

static int
inst_INC(struct OPCodeDesc *op, struct machine *m)
{

  inst_load(m);
  m->cpu.dbb++;
 
  if (m->cpu.dbb == 0)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  if (m->cpu.dbb & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);

  inst_store(m);
  return 1;
  // NYI; dump_op(op); dump_reg(m); assert(0); return 0;

}

static int
inst_INX(struct OPCodeDesc *op, struct machine *m)
{
  
  m->cpu.reg.x++;
 
  if (m->cpu.reg.x == 0)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  if (m->cpu.reg.x & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);

  return 1;


}

static int
inst_INY(struct OPCodeDesc *op, struct machine *m)
{

    m->cpu.reg.y++;
    if (m->cpu.reg.y == 0)
		SR_Z_SET(m->cpu.reg.sr);
    else
		SR_Z_CLR(m->cpu.reg.sr);

    if (m->cpu.reg.y & BYTE_SIGN_MASK)
		SR_N_SET(m->cpu.reg.sr);
    else
		SR_N_CLR(m->cpu.reg.sr);
    return 1;

}

static int
inst_JMP(struct OPCodeDesc *op, struct machine *m)
{
  m->cpu.reg.pc = m->cpu.abr;
  return 1;
}

static int
inst_JSR(struct OPCodeDesc *op, struct machine *m)
{
 

  inst_push(m, ADDR_HIGH(m->cpu.reg.pc - 1));
  inst_push(m, ADDR_LOW(m->cpu.reg.pc - 1));
  
  m->cpu.reg.pc = m->cpu.abr;
  return 1; 

}

static int
inst_LDA(struct OPCodeDesc *op, struct machine *m)
{

    	inst_load(m);
	m->cpu.reg.ac = m->cpu.dbb;
	if (m->cpu.reg.ac == 0)
		SR_Z_SET(m->cpu.reg.sr);
	else
		SR_Z_CLR(m->cpu.reg.sr);
		
	if (m->cpu.reg.ac & BYTE_SIGN_MASK)
		SR_N_SET(m->cpu.reg.sr);
	else
		SR_N_CLR(m->cpu.reg.sr);
	return 1;


}

static int
inst_LDX(struct OPCodeDesc *op, struct machine *m)
{

    	inst_load(m);
	m->cpu.reg.x = m->cpu.dbb;
	if (m->cpu.reg.x == 0)
		SR_Z_SET(m->cpu.reg.sr);
	else
		SR_Z_CLR(m->cpu.reg.sr);
		
	if (m->cpu.reg.x & BYTE_SIGN_MASK)
		SR_N_SET(m->cpu.reg.sr);
	else
		SR_N_CLR(m->cpu.reg.sr);
	return 1;

}

static int
inst_LDY(struct OPCodeDesc *op, struct machine *m)
{

    inst_load(m);
	m->cpu.reg.y = m->cpu.dbb;
	if (m->cpu.reg.y == 0)
		SR_Z_SET(m->cpu.reg.sr);
	else
		SR_Z_CLR(m->cpu.reg.sr);
		
	if (m->cpu.reg.y & BYTE_SIGN_MASK)
		SR_N_SET(m->cpu.reg.sr);
	else
		SR_N_CLR(m->cpu.reg.sr);
	return 1;



}

static int
inst_LSR(struct OPCodeDesc *op, struct machine *m)
{  
if (op->am->mode == ACC) {
     
     
     if(m->cpu.reg.ac & SR_C_MASK)
       SR_C_SET(m->cpu.reg.sr);
     
     else
       SR_C_CLR(m->cpu.reg.sr);
	 
     m->cpu.reg.ac >>= 1;
     m->cpu.reg.ac &= 0x7F;
    
     SR_N_CLR(m->cpu.reg.sr);

     if (m->cpu.reg.ac == 0)
       SR_Z_SET(m->cpu.reg.sr);
     
     else
       SR_Z_CLR(m->cpu.reg.sr);
     
     return 1;

  }
  else {

     inst_load(m);

     if(m->cpu.dbb & SR_C_MASK)
       SR_C_SET(m->cpu.reg.sr);
     
     else
       SR_C_CLR(m->cpu.reg.sr);
     
     m->cpu.dbb >>=  1;
     m->cpu.dbb &= 0x7F;
   
    
     SR_N_CLR(m->cpu.reg.sr);

     if (m->cpu.dbb == 0)
       SR_Z_SET(m->cpu.reg.sr);
     
     else
       SR_Z_CLR(m->cpu.reg.sr);
     
     

   inst_store(m);
   return 1;
  }
  
}

static int
inst_NOP(struct OPCodeDesc *op, struct machine *m)
{ NYI; dump_op(op); dump_reg(m); assert(0); return 0;}

static int
inst_ORA(struct OPCodeDesc *op, struct machine *m)
{


  inst_load(m);
  m->cpu.reg.ac |= m->cpu.dbb;

  if (m->cpu.reg.ac == 0)
    SR_Z_SET(m->cpu.reg.sr);
  else
    SR_Z_CLR(m->cpu.reg.sr);

  if (m->cpu.reg.ac & BYTE_SIGN_MASK)
    SR_N_SET(m->cpu.reg.sr);
  else
    SR_N_CLR(m->cpu.reg.sr);


  return 1;





}

static int
inst_PHA(struct OPCodeDesc *op, struct machine *m)
{
    inst_push(m, m->cpu.reg.ac);
    return 1;

}

static int
inst_PHP(struct OPCodeDesc *op, struct machine *m)
{


	inst_push(m, (m->cpu.reg.sr | 0xEF));
	return 1;


}

static int
inst_PLA(struct OPCodeDesc *op, struct machine *m)
{

 
	m->cpu.reg.ac = inst_pop(m);
	if (m->cpu.reg.ac == 0)
	  SR_Z_SET(m->cpu.reg.sr);
	else
	  SR_Z_CLR(m->cpu.reg.sr);

	if (m->cpu.reg.ac & BYTE_SIGN_MASK)
	  SR_N_SET(m->cpu.reg.sr);
	else
	  SR_N_CLR(m->cpu.reg.sr);
	return 1;



}

static int
inst_PLP(struct OPCodeDesc *op, struct machine *m)
{
    
	m->cpu.reg.sr = inst_pop(m);
	m->cpu.reg.sr |= 0x20;
//	m->cpu.reg.sr |= 0xEF;
	SR_D_CLR(m->cpu.reg.sr);
	SR_B_CLR(m->cpu.reg.sr);
	

	return 1;

}

static int
inst_ROL(struct OPCodeDesc *op, struct machine *m)
{
if (op->am->mode == ACC) {

   	 byte temp_c = m->cpu.reg.ac & BYTE_SIGN_MASK;

	m->cpu.reg.ac <<= 1;
	if(SR_C(m->cpu.reg.sr)) 
		m->cpu.reg.ac |= SR_C_MASK;
	else
		m->cpu.reg.ac &= 0xFE;
	if(temp_c)
		SR_C_SET(m->cpu.reg.sr);
	else
		SR_C_CLR(m->cpu.reg.sr);
	

     if (m->cpu.reg.ac & BYTE_SIGN_MASK)
	SR_N_SET(m->cpu.reg.sr);
    else
	SR_N_CLR(m->cpu.reg.sr);
     if (m->cpu.reg.ac == 0)
       SR_Z_SET(m->cpu.reg.sr);
     
     else
       SR_Z_CLR(m->cpu.reg.sr);
     

     return 1;

  }
  else {

   inst_load(m);
 byte temp_c = m->cpu.dbb & BYTE_SIGN_MASK;

	m->cpu.dbb <<= 1;
	if(SR_C(m->cpu.reg.sr)) 
		m->cpu.dbb |= SR_C_MASK;
	else
		m->cpu.dbb &= 0xFE;
	if(temp_c)
		SR_C_SET(m->cpu.reg.sr);
	else
		SR_C_CLR(m->cpu.reg.sr);

 	if (m->cpu.dbb & BYTE_SIGN_MASK)
		SR_N_SET(m->cpu.reg.sr);
	else
		SR_N_SET(m->cpu.reg.sr);
     if (m->cpu.dbb == 0)
       SR_Z_SET(m->cpu.reg.sr);
     
     else
       SR_Z_CLR(m->cpu.reg.sr);
     

   inst_store(m);
   return 1;
  } 



}

static int
inst_ROR(struct OPCodeDesc *op, struct machine *m)
{

  if (op->am->mode == ACC) {
 byte temp_c = m->cpu.reg.ac & SR_C_MASK;

	m->cpu.reg.ac >>= 1;
	if(SR_C(m->cpu.reg.sr)) 
		m->cpu.reg.ac |= BYTE_SIGN_MASK;
	else 
		m->cpu.reg.ac &= 0x7F;

	
	if(temp_c)
		SR_C_SET(m->cpu.reg.sr);
	else
		SR_C_CLR(m->cpu.reg.sr);




	if (m->cpu.reg.ac & BYTE_SIGN_MASK)
		SR_N_SET(m->cpu.reg.sr);
	else
		SR_N_CLR(m->cpu.reg.sr);
     if (m->cpu.reg.ac == 0)
       SR_Z_SET(m->cpu.reg.sr);
     
     else
       SR_Z_CLR(m->cpu.reg.sr);
     

     return 1;

  }
  else {
  
 inst_load(m);
 byte temp_c = m->cpu.dbb & SR_C_MASK;

	m->cpu.dbb >>= 1;
	if(SR_C(m->cpu.reg.sr)) 
		m->cpu.dbb |= BYTE_SIGN_MASK;
	else 
		m->cpu.dbb &= 0x7F;

	
	if(temp_c)
		SR_C_SET(m->cpu.reg.sr);
	else
		SR_C_CLR(m->cpu.reg.sr);



	if (m->cpu.reg.ac & BYTE_SIGN_MASK)
		SR_N_SET(m->cpu.reg.sr);
	else
		SR_N_CLR(m->cpu.reg.sr);

     if (m->cpu.dbb == 0)
       SR_Z_SET(m->cpu.reg.sr);
     
     else
       SR_Z_CLR(m->cpu.reg.sr);
     

   inst_store(m);
   return 1;
  } 
  
 
} 


static int
inst_RTI(struct OPCodeDesc *op, struct machine *m)
{
  byte SR, PCL, PCH;

  SR = inst_pop(m);
  PCL = inst_pop(m);
  PCH = inst_pop(m);

  m->cpu.reg.sr = SR;
  ADDR_SET_LOW(m->cpu.reg.pc, PCL);
  ADDR_SET_HIGH(m->cpu.reg.pc, PCH);
  int_RTI(m);
  return 1;
}

static int
inst_RTS(struct OPCodeDesc *op, struct machine *m)
{

  byte PCL, PCH;
  PCL = inst_pop(m);
  PCH = inst_pop(m);

  ADDR_SET_LOW(m->cpu.reg.pc, PCL);
  ADDR_SET_HIGH(m->cpu.reg.pc, PCH);

  m->cpu.reg.pc++;
  return 1;
}

static int
inst_SBC(struct OPCodeDesc *op, struct machine *m)
{
  if (SR_D(m->cpu.reg.sr))
  	NYI; 
 
  inst_load(m);
  byte val = m->cpu.dbb;
  byte temp = m->cpu.reg.ac - val;
  if (!SR_C(m->cpu.reg.sr))
	temp -= 1;
 
  SR_Z_CLR(m->cpu.reg.sr);
  SR_V_CLR(m->cpu.reg.sr);
  SR_C_CLR(m->cpu.reg.sr);
  SR_N_CLR(m->cpu.reg.sr);


  if( temp >= 0x00) 
	SR_C_SET(m->cpu.reg.sr);
	

	
  if (((m->cpu.reg.ac ^ val)& BYTE_SIGN_MASK) && ((m->cpu.reg.ac ^ temp)& BYTE_SIGN_MASK))
	SR_V_SET(m->cpu.reg.sr);
	
  m->cpu.reg.ac = temp & 0xFF;
  
  if( m->cpu.reg.ac & BYTE_SIGN_MASK )
	SR_N_SET(m->cpu.reg.sr);
	
  if(m->cpu.reg.ac == 0)
	SR_Z_SET(m->cpu.reg.sr);

  return 1;

}

static int
inst_SEC(struct OPCodeDesc *op, struct machine *m)
{

  SR_C_SET(m->cpu.reg.sr);
  return 1;

}

static int
inst_SED(struct OPCodeDesc *op, struct machine *m)
{ 
	SR_D_SET(m->cpu.reg.sr);
	return 1;

}

static int
inst_SEI(struct OPCodeDesc *op, struct machine *m)
{ 
	SR_I_SET(m->cpu.reg.sr);
	return 1;
}

static int
inst_STA(struct OPCodeDesc *op, struct machine *m)
{

  m->cpu.dbb = m->cpu.reg.ac;
  inst_store(m);
  return 1;

}

static int
inst_STX(struct OPCodeDesc *op, struct machine *m)
{

  m->cpu.dbb = m->cpu.reg.x;
  inst_store(m);
  return 1;


 
}

static int
inst_STY(struct OPCodeDesc *op, struct machine *m)
{


	m->cpu.dbb = m->cpu.reg.y;
	inst_store(m);
	return 1;


}

static int
inst_TAX(struct OPCodeDesc *op, struct machine *m)
{

        m->cpu.reg.x = m->cpu.reg.ac;
	if (m->cpu.reg.x == 0)
	  SR_Z_SET(m->cpu.reg.sr);
	else
	  SR_Z_CLR(m->cpu.reg.sr);

	if (m->cpu.reg.x & BYTE_SIGN_MASK)
	  SR_N_SET(m->cpu.reg.sr);
	else
	  SR_N_CLR(m->cpu.reg.sr);
	return 1;



 

}

static int
inst_TAY(struct OPCodeDesc *op, struct machine *m)
{

    m->cpu.reg.y = m->cpu.reg.ac;
	if (m->cpu.reg.y == 0)
	  SR_Z_SET(m->cpu.reg.sr);
	else
	  SR_Z_CLR(m->cpu.reg.sr);

	if (m->cpu.reg.y & BYTE_SIGN_MASK)
	  SR_N_SET(m->cpu.reg.sr);
	else
	  SR_N_CLR(m->cpu.reg.sr);
	return 1;



  

}

static int
inst_TSX(struct OPCodeDesc *op, struct machine *m)
{


    m->cpu.reg.x = m->cpu.reg.sp;
    if (m->cpu.reg.x == 0)
	SR_Z_SET(m->cpu.reg.sr);
    else
	SR_Z_CLR(m->cpu.reg.sr);
		
    if (m->cpu.reg.x & BYTE_SIGN_MASK)
	SR_N_SET(m->cpu.reg.sr);
    else
	SR_N_CLR(m->cpu.reg.sr);

    return 1;

}

static int
inst_TXA(struct OPCodeDesc *op, struct machine *m)
{


	m->cpu.reg.ac = m->cpu.reg.x;
	if (m->cpu.reg.ac == 0)
		SR_Z_SET(m->cpu.reg.sr);
	else
		SR_Z_CLR(m->cpu.reg.sr);
		
	if (m->cpu.reg.ac & BYTE_SIGN_MASK)
		SR_N_SET(m->cpu.reg.sr);
	else
		SR_N_CLR(m->cpu.reg.sr);
	return 1;

}

static int
inst_TXS(struct OPCodeDesc *op, struct machine *m)
{


  m->cpu.reg.sp = m->cpu.reg.x;
  return 1;

}

static int
inst_TYA(struct OPCodeDesc *op, struct machine *m)
{

	m->cpu.reg.ac = m->cpu.reg.y;
	if (m->cpu.reg.ac == 0)
	  SR_Z_SET(m->cpu.reg.sr);
	else
	  SR_Z_CLR(m->cpu.reg.sr);

	if (m->cpu.reg.ac & BYTE_SIGN_MASK)
	  SR_N_SET(m->cpu.reg.sr);
	else
	  SR_N_CLR(m->cpu.reg.sr);
	return 1;


}

struct AddressingModeDesc AMTable[] = {
  { INVALIDAM, inst_am_inv, 0, "INVALID ADDRESSING MODE", "" },
  { ACC, inst_am_acc, 1, "A: Accumulator A", "" },
  { ABS, inst_am_abs, 3, "abs: Absolute a", "$HHLL" },
  { ABSX, inst_am_absx, 3, "abs,X: Absolute Indexed with X a,x", "$HHLL,X" },
  { ABSY, inst_am_absy, 3, "abs,Y: Absolute Indexed with Y a,y", "$HHLL,Y" },
  { IMM, inst_am_imm, 2, "#: Immediate #", "#$BB" },
  { IMPL, inst_am_impl, 1, "impl: Implied i", "" },
  { IND, inst_am_ind, 3, "ind: Absolute Indirect (a)", "$($HHLL)" },
  { XIND, inst_am_xind, 2, "X,ind: Zero Page Indexed Indirect (zp,x)", "($BB,X)" },
  { INDY, inst_am_indy, 2, "ind,Y: Zero Page Indirect Indexed with Y (zp),y", "($LL),Y" },
  { REL, inst_am_rel, 2, "rel: Program Counter Relative r", "$BB" },
  { ZP, inst_am_zp, 2, "zpg: Zero Page zp", "$LL" },
  { ZPX, inst_am_zpx, 2, "zpg,X: Zero Page Index with X", "$LL,X" },
  { ZPY, inst_am_zpy, 2, "zpg,Y: Zero Page Index with Y", "$LL,Y" }
};

struct InstructionDesc InstTable[] = {
  { INV, "INV", "INVALID INST" },
  { ADC, "ADC", "add with carry" },
  { AND, "AND", "and (with accumulator)" },
  { ASL, "ASL", "arithmetic shift left" },
  { BCC, "BCC", "branch on carry clear" },
  { BCS, "BCS", "branch on carry set" },
  { BEQ, "BEQ", "branch on equal (zero set)" },
  { BIT, "BIT", "bit test" },
  { BMI, "BMI", "branch on minus (negative set)" },
  { BNE, "BNE", "branch on not equal (zero clear)" },
  { BPL, "BPL", "branch on plus (negative clear)" },
  { BRK, "BRK", "interrupt" },
  { BVC, "BVC", "branch on overflow clear" },
  { BVS, "BVS", "branch on overflow set" },
  { CLC, "CLC", "clear carry" },
  { CLD, "CLD", "clear decimal" },
  { CLI, "CLI", "clear interrupt disable" },
  { CLV, "CLV", "clear overflow" },
  { CMP, "CMP", "compare (with accumulator)" },
  { CPX, "CPX", "compare with X" },
  { CPY, "CPY", "compare with Y" },
  { DEC, "DEC", "decrement" },
  { DEX, "DEX", "decrement X" },
  { DEY, "DEY", "decrement Y" },
  { EOR, "EOR", "exclusive or (with accumulator)" },
  { INC, "INC", "increment" },
  { INX, "INX", "increment X" },
  { INY, "INY", "increment Y" },
  { JMP, "JMP", "jump" },
  { JSR, "JSR", "jump subroutine" },
  { LDA, "LDA", "load accumulator" },
  { LDX, "LDX", "load X" },
  { LDY, "LDY", "load Y" },
  { LSR, "LSR", "logical shift right" },
  { NOP, "NOP", "no operation" },
  { ORA, "ORA", "or with accumulator" },
  { PHA, "PHA", "push accumulator" },
  { PHP, "PHP", "push processor status (SR)" },
  { PLA, "PLA", "pull accumulator" },
  { PLP, "PLP", "pull processor status (SR)" },
  { ROL, "ROL", "rotate left" },
  { ROR, "ROR", "rotate right" },
  { RTI, "RTI", "return from interrupt" },
  { RTS, "RTS", "return from subroutine" },
  { SBC, "SBC", "subtract with carry" },
  { SEC, "SEC", "set carry" },
  { SED, "SED", "set decimal" },
  { SEI, "SEI", "set interrupt disable" },
  { STA, "STA", "store accumulator" },
  { STX, "STX", "store X" },
  { STY, "STY", "store Y" },
  { TAX, "TAX", "transfer accumulator to X" },
  { TAY, "TAY", "transfer accumulator to Y" },
  { TSX, "TSX", "transfer stack pointer to X" },
  { TXA, "TXA", "transfer X to accumulator" },
  { TXS, "TXS", "transfer X to stack pointer" },
  { TYA, "TYA", "transfer Y to accumulator" }
};

struct OPCodeDesc OPCodeTable[NUMOPCODES] = {
  { 0x00, &(AMTable[IMPL]), 0, inst_BRK, &(InstTable[BRK]) },
  { 0x01, &(AMTable[XIND]), 0, inst_ORA, &(InstTable[ORA]) },
  { 0x02, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x03, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x04, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x05, &(AMTable[ZP]), 0, inst_ORA, &(InstTable[ORA]) },
  { 0x06, &(AMTable[ZP]), 0, inst_ASL, &(InstTable[ASL]) },
  { 0x07, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x08, &(AMTable[IMPL]), 0, inst_PHP, &(InstTable[PHP]) },
  { 0x09, &(AMTable[IMM]), 0, inst_ORA, &(InstTable[ORA]) },
  { 0x0a, &(AMTable[ACC]), 0, inst_ASL, &(InstTable[ASL]) },
  { 0x0b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x0c, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x0d, &(AMTable[ABS]), 0, inst_ORA, &(InstTable[ORA]) },
  { 0x0e, &(AMTable[ABS]), 0, inst_ASL, &(InstTable[ASL]) },
  { 0x0f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x10, &(AMTable[REL]), 0, inst_BPL, &(InstTable[BPL]) },
  { 0x11, &(AMTable[INDY]), 0, inst_ORA, &(InstTable[ORA]) },
  { 0x12, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x13, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x14, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x15, &(AMTable[ZPX]), 0, inst_ORA, &(InstTable[ORA]) },
  { 0x16, &(AMTable[ZPX]), 0, inst_ASL, &(InstTable[ASL]) },
  { 0x17, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x18, &(AMTable[IMPL]), 0, inst_CLC, &(InstTable[CLC]) },
  { 0x19, &(AMTable[ABSY]), 0, inst_ORA, &(InstTable[ORA]) },
  { 0x1a, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x1b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x1c, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x1d, &(AMTable[ABSX]), 0, inst_ORA, &(InstTable[ORA]) },
  { 0x1e, &(AMTable[ABSX]), 0, inst_ASL, &(InstTable[ASL]) },
  { 0x1f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x20, &(AMTable[ABS]), 0, inst_JSR, &(InstTable[JSR]) },
  { 0x21, &(AMTable[XIND]), 0, inst_AND, &(InstTable[AND]) },
  { 0x22, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x23, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x24, &(AMTable[ZP]), 0, inst_BIT, &(InstTable[BIT]) },
  { 0x25, &(AMTable[ZP]), 0, inst_AND, &(InstTable[AND]) },
  { 0x26, &(AMTable[ZP]), 0, inst_ROL, &(InstTable[ROL]) },
  { 0x27, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x28, &(AMTable[IMPL]), 0, inst_PLP, &(InstTable[PLP]) },
  { 0x29, &(AMTable[IMM]), 0, inst_AND, &(InstTable[AND]) },
  { 0x2a, &(AMTable[ACC]), 0, inst_ROL, &(InstTable[ROL]) },
  { 0x2b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x2c, &(AMTable[ABS]), 0, inst_BIT, &(InstTable[BIT]) },
  { 0x2d, &(AMTable[ABS]), 0, inst_AND, &(InstTable[AND]) },
  { 0x2e, &(AMTable[ABS]), 0, inst_ROL, &(InstTable[ROL]) },
  { 0x2f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x30, &(AMTable[REL]), 0, inst_BMI, &(InstTable[BMI]) },
  { 0x31, &(AMTable[INDY]), 0, inst_AND, &(InstTable[AND]) },
  { 0x32, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x33, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x34, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x35, &(AMTable[ZPX]), 0, inst_AND, &(InstTable[AND]) },
  { 0x36, &(AMTable[ZPX]), 0, inst_ROL, &(InstTable[ROL]) },
  { 0x37, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x38, &(AMTable[IMPL]), 0, inst_SEC, &(InstTable[SEC]) },
  { 0x39, &(AMTable[ABSY]), 0, inst_AND, &(InstTable[AND]) },
  { 0x3a, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x3b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x3c, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x3d, &(AMTable[ABSX]), 0, inst_AND, &(InstTable[AND]) },
  { 0x3e, &(AMTable[ABSX]), 0, inst_ROL, &(InstTable[ROL]) },
  { 0x3f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x40, &(AMTable[IMPL]), 0, inst_RTI, &(InstTable[RTI]) },
  { 0x41, &(AMTable[XIND]), 0, inst_EOR, &(InstTable[EOR]) },
  { 0x42, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x43, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x44, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x45, &(AMTable[ZP]), 0, inst_EOR, &(InstTable[EOR]) },
  { 0x46, &(AMTable[ZP]), 0, inst_LSR, &(InstTable[LSR]) },
  { 0x47, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x48, &(AMTable[IMPL]), 0, inst_PHA, &(InstTable[PHA]) },
  { 0x49, &(AMTable[IMM]), 0, inst_EOR, &(InstTable[EOR]) },
  { 0x4a, &(AMTable[ACC]), 0, inst_LSR, &(InstTable[LSR]) },
  { 0x4b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x4c, &(AMTable[ABS]), 0, inst_JMP, &(InstTable[JMP]) },
  { 0x4d, &(AMTable[ABS]), 0, inst_EOR, &(InstTable[EOR]) },
  { 0x4e, &(AMTable[ABS]), 0, inst_LSR, &(InstTable[LSR]) },
  { 0x4f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x50, &(AMTable[REL]), 0, inst_BVC, &(InstTable[BVC]) },
  { 0x51, &(AMTable[INDY]), 0, inst_EOR, &(InstTable[EOR]) },
  { 0x52, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x53, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x54, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x55, &(AMTable[ZPX]), 0, inst_EOR, &(InstTable[EOR]) },
  { 0x56, &(AMTable[ZPX]), 0, inst_LSR, &(InstTable[LSR]) },
  { 0x57, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x58, &(AMTable[IMPL]), 0, inst_CLI, &(InstTable[CLI]) },
  { 0x59, &(AMTable[ABSY]), 0, inst_EOR, &(InstTable[EOR]) },
  { 0x5a, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x5b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x5c, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x5d, &(AMTable[ABSX]), 0, inst_EOR, &(InstTable[EOR]) },
  { 0x5e, &(AMTable[ABSX]), 0, inst_LSR, &(InstTable[LSR]) },
  { 0x5f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x60, &(AMTable[IMPL]), 0, inst_RTS, &(InstTable[RTS]) },
  { 0x61, &(AMTable[XIND]), 0, inst_ADC, &(InstTable[ADC]) },
  { 0x62, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x63, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x64, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x65, &(AMTable[ZP]), 0, inst_ADC, &(InstTable[ADC]) },
  { 0x66, &(AMTable[ZP]), 0, inst_ROR, &(InstTable[ROR]) },
  { 0x67, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x68, &(AMTable[IMPL]), 0, inst_PLA, &(InstTable[PLA]) },
  { 0x69, &(AMTable[IMM]), 0, inst_ADC, &(InstTable[ADC]) },
  { 0x6a, &(AMTable[ACC]), 0, inst_ROR, &(InstTable[ROR]) },
  { 0x6b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x6c, &(AMTable[IND]), 0, inst_JMP, &(InstTable[JMP]) },
  { 0x6d, &(AMTable[ABS]), 0, inst_ADC, &(InstTable[ADC]) },
  { 0x6e, &(AMTable[ABS]), 0, inst_ROR, &(InstTable[ROR]) },
  { 0x6f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x70, &(AMTable[REL]), 0, inst_BVS, &(InstTable[BVS]) },
  { 0x71, &(AMTable[INDY]), 0, inst_ADC, &(InstTable[ADC]) },
  { 0x72, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x73, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x74, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x75, &(AMTable[ZPX]), 0, inst_ADC, &(InstTable[ADC]) },
  { 0x76, &(AMTable[ZPX]), 0, inst_ROR, &(InstTable[ROR]) },
  { 0x77, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x78, &(AMTable[IMPL]), 0, inst_SEI, &(InstTable[SEI]) },
  { 0x79, &(AMTable[ABSY]), 0, inst_ADC, &(InstTable[ADC]) },
  { 0x7a, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x7b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x7c, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x7d, &(AMTable[ABSX]), 0, inst_ADC, &(InstTable[ADC]) },
  { 0x7e, &(AMTable[ABSX]), 0, inst_ROR, &(InstTable[ROR]) },
  { 0x7f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x80, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x81, &(AMTable[XIND]), 0, inst_STA, &(InstTable[STA]) },
  { 0x82, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x83, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x84, &(AMTable[ZP]), 0, inst_STY, &(InstTable[STY]) },
  { 0x85, &(AMTable[ZP]), 0, inst_STA, &(InstTable[STA]) },
  { 0x86, &(AMTable[ZP]), 0, inst_STX, &(InstTable[STX]) },
  { 0x87, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x88, &(AMTable[IMPL]), 0, inst_DEY, &(InstTable[DEY]) },
  { 0x89, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x8a, &(AMTable[IMPL]), 0, inst_TXA, &(InstTable[TXA]) },
  { 0x8b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x8c, &(AMTable[ABS]), 0, inst_STY, &(InstTable[STY]) },
  { 0x8d, &(AMTable[ABS]), 0, inst_STA, &(InstTable[STA]) },
  { 0x8e, &(AMTable[ABS]), 0, inst_STX, &(InstTable[STX]) },
  { 0x8f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x90, &(AMTable[REL]), 0, inst_BCC, &(InstTable[BCC]) },
  { 0x91, &(AMTable[INDY]), 0, inst_STA, &(InstTable[STA]) },
  { 0x92, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x93, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x94, &(AMTable[ZPX]), 0, inst_STY, &(InstTable[STY]) },
  { 0x95, &(AMTable[ZPX]), 0, inst_STA, &(InstTable[STA]) },
  { 0x96, &(AMTable[ZPY]), 0, inst_STX, &(InstTable[STX]) },
  { 0x97, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x98, &(AMTable[IMPL]), 0, inst_TYA, &(InstTable[TYA]) },
  { 0x99, &(AMTable[ABSY]), 0, inst_STA, &(InstTable[STA]) },
  { 0x9a, &(AMTable[IMPL]), 0, inst_TXS, &(InstTable[TXS]) },
  { 0x9b, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x9c, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x9d, &(AMTable[ABSX]), 0, inst_STA, &(InstTable[STA]) },
  { 0x9e, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0x9f, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xa0, &(AMTable[IMM]), 0, inst_LDY, &(InstTable[LDY]) },
  { 0xa1, &(AMTable[XIND]), 0, inst_LDA, &(InstTable[LDA]) },
  { 0xa2, &(AMTable[IMM]), 0, inst_LDX, &(InstTable[LDX]) },
  { 0xa3, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xa4, &(AMTable[ZP]), 0, inst_LDY, &(InstTable[LDY]) },
  { 0xa5, &(AMTable[ZP]), 0, inst_LDA, &(InstTable[LDA]) },
  { 0xa6, &(AMTable[ZP]), 0, inst_LDX, &(InstTable[LDX]) },
  { 0xa7, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xa8, &(AMTable[IMPL]), 0, inst_TAY, &(InstTable[TAY]) },
  { 0xa9, &(AMTable[IMM]), 0, inst_LDA, &(InstTable[LDA]) },
  { 0xaa, &(AMTable[IMPL]), 0, inst_TAX, &(InstTable[TAX]) },
  { 0xab, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xac, &(AMTable[ABS]), 0, inst_LDY, &(InstTable[LDY]) },
  { 0xad, &(AMTable[ABS]), 0, inst_LDA, &(InstTable[LDA]) },
  { 0xae, &(AMTable[ABS]), 0, inst_LDX, &(InstTable[LDX]) },
  { 0xaf, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xb0, &(AMTable[REL]), 0, inst_BCS, &(InstTable[BCS]) },
  { 0xb1, &(AMTable[INDY]), 0, inst_LDA, &(InstTable[LDA]) },
  { 0xb2, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xb3, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xb4, &(AMTable[ZPX]), 0, inst_LDY, &(InstTable[LDY]) },
  { 0xb5, &(AMTable[ZPX]), 0, inst_LDA, &(InstTable[LDA]) },
  { 0xb6, &(AMTable[ZPY]), 0, inst_LDX, &(InstTable[LDX]) },
  { 0xb7, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xb8, &(AMTable[IMPL]), 0, inst_CLV, &(InstTable[CLV]) },
  { 0xb9, &(AMTable[ABSY]), 0, inst_LDA, &(InstTable[LDA]) },
  { 0xba, &(AMTable[IMPL]), 0, inst_TSX, &(InstTable[TSX]) },
  { 0xbb, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xbc, &(AMTable[ABSX]), 0, inst_LDY, &(InstTable[LDY]) },
  { 0xbd, &(AMTable[ABSX]), 0, inst_LDA, &(InstTable[LDA]) },
  { 0xbe, &(AMTable[ABSY]), 0, inst_LDX, &(InstTable[LDX]) },
  { 0xbf, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xc0, &(AMTable[IMM]), 0, inst_CPY, &(InstTable[CPY]) },
  { 0xc1, &(AMTable[XIND]), 0, inst_CMP, &(InstTable[CMP]) },
  { 0xc2, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xc3, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xc4, &(AMTable[ZP]), 0, inst_CPY, &(InstTable[CPY]) },
  { 0xc5, &(AMTable[ZP]), 0, inst_CMP, &(InstTable[CMP]) },
  { 0xc6, &(AMTable[ZP]), 0, inst_DEC, &(InstTable[DEC]) },
  { 0xc7, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xc8, &(AMTable[IMPL]), 0, inst_INY, &(InstTable[INY]) },
  { 0xc9, &(AMTable[IMM]), 0, inst_CMP, &(InstTable[CMP]) },
  { 0xca, &(AMTable[IMPL]), 0, inst_DEX, &(InstTable[DEX]) },
  { 0xcb, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xcc, &(AMTable[ABS]), 0, inst_CPY, &(InstTable[CPY]) },
  { 0xcd, &(AMTable[ABS]), 0, inst_CMP, &(InstTable[CMP]) },
  { 0xce, &(AMTable[ABS]), 0, inst_DEC, &(InstTable[DEC]) },
  { 0xcf, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xd0, &(AMTable[REL]), 0, inst_BNE, &(InstTable[BNE]) },
  { 0xd1, &(AMTable[INDY]), 0, inst_CMP, &(InstTable[CMP]) },
  { 0xd2, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xd3, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xd4, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xd5, &(AMTable[ZPX]), 0, inst_CMP, &(InstTable[CMP]) },
  { 0xd6, &(AMTable[ZPX]), 0, inst_DEC, &(InstTable[DEC]) },
  { 0xd7, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xd8, &(AMTable[IMPL]), 0, inst_CLD, &(InstTable[CLD]) },
  { 0xd9, &(AMTable[ABSY]), 0, inst_CMP, &(InstTable[CMP]) },
  { 0xda, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xdb, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xdc, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xdd, &(AMTable[ABSX]), 0, inst_CMP, &(InstTable[CMP]) },
  { 0xde, &(AMTable[ABSX]), 0, inst_DEC, &(InstTable[DEC]) },
  { 0xdf, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xe0, &(AMTable[IMM]), 0, inst_CPX, &(InstTable[CPX]) },
  { 0xe1, &(AMTable[XIND]), 0, inst_SBC, &(InstTable[SBC]) },
  { 0xe2, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xe3, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xe4, &(AMTable[ZP]), 0, inst_CPX, &(InstTable[CPX]) },
  { 0xe5, &(AMTable[ZP]), 0, inst_SBC, &(InstTable[SBC]) },
  { 0xe6, &(AMTable[ZP]), 0, inst_INC, &(InstTable[INC]) },
  { 0xe7, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xe8, &(AMTable[IMPL]), 0, inst_INX, &(InstTable[INX]) },
  { 0xe9, &(AMTable[IMM]), 0, inst_SBC, &(InstTable[SBC]) },
  { 0xea, &(AMTable[IMPL]), 0, inst_NOP, &(InstTable[NOP]) },
  { 0xeb, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xec, &(AMTable[ABS]), 0, inst_CPX, &(InstTable[CPX]) },
  { 0xed, &(AMTable[ABS]), 0, inst_SBC, &(InstTable[SBC]) },
  { 0xee, &(AMTable[ABS]), 0, inst_INC, &(InstTable[INC]) },
  { 0xef, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xf0, &(AMTable[REL]), 0, inst_BEQ, &(InstTable[BEQ]) },
  { 0xf1, &(AMTable[INDY]), 0, inst_SBC, &(InstTable[SBC]) },
  { 0xf2, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xf3, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xf4, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xf5, &(AMTable[ZPX]), 0, inst_SBC, &(InstTable[SBC]) },
  { 0xf6, &(AMTable[ZPX]), 0, inst_INC, &(InstTable[INC]) },
  { 0xf7, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xf8, &(AMTable[IMPL]), 0, inst_SED, &(InstTable[SED]) },
  { 0xf9, &(AMTable[ABSY]), 0, inst_SBC, &(InstTable[SBC]) },
  { 0xfa, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xfb, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xfc, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) },
  { 0xfd, &(AMTable[ABSX]), 0, inst_SBC, &(InstTable[SBC]) },
  { 0xfe, &(AMTable[ABSX]), 0, inst_INC, &(InstTable[INC]) },
  { 0xff, &(AMTable[INVALIDAM]), 0, inst_INV, &(InstTable[INV]) }
};
