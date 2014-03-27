define sbreak
  set $pcaddr = &(m->cpu.reg.pc)
  watch (*((address *)$pcaddr) == $arg0 )
end
document sbreak
set a simulated break point on pc == $arg0
end

define sxbyte
  if $argc == 1
   set $sxbaddr = $arg0
   set $sxbcnt = 1
  end

  if $argc == 2
   set $sxbaddr = $arg0
   set $sxbcnt = $arg1
  end

  set $c = $sxbcnt
  set $d = 0

  while $c > 0
    set $lb = m->memory[$sxbaddr]
    set $c = $c - 1
    
    if $d == 0
      printf "0x%04x: 0x%02x (%c)", $sxbaddr, $lb, $lb
      set $d = $d + 1
    else
      printf " 0x%02x (%c)", $lb, $lb
      if $d == 3 
         printf "\n"
         set $d = 0
      else
         set $d = $d + 1
      end
    end
    set $sxbaddr = $sxbaddr + 1
  end
  printf "\n"
end
document sxbyte
print value of simulated memory at address arg0
end

define sxshort

  if $argc == 1
   set $sxsaddr = $arg0
   set $sxscnt = 1
  end

  if $argc == 2
   set $sxsaddr = $arg0
   set $sxscnt = $arg1
  end

  set $c = $sxscnt
  set $d = 0

  while $c > 0
    set $lb = m->memory[$sxsaddr]
    set $hb = m->memory[$sxsaddr+1]
    set $c = $c - 1
    
    if $d == 0
       printf "0x%04x: 0x%02x%02x", $sxsaddr, $hb, $lb
       set $d = $d + 1
    else
      printf " 0x%02x%02x", $hb, $lb
      if $d == 3 
         printf "\n"
         set $d = 0
      else
         set $d = $d + 1
      end
    end
    set $sxsaddr = $sxsaddr + 2
  end
  printf "\n"
end
document sxshort
  print address located in memory at address $arg0
end
 
define sopdecode
   p OPCodeTable[$arg0]
   set $op = $
   p *$op.am
   p *$op.ins
end
document sopdecode
   print the details of opcode = $arg0	
end

define sdisassemble

  if $argc == 1
   set $sdaddr = $arg0
   set $sdcnt = 1
  end

  if $argc == 2
   set $sdaddr = $arg0
   set $sdcnt = $arg1
  end

  set $d = $sdcnt
  while $d > 0

    set $op = m->memory[$sdaddr]
    set $bytes = OPCodeTable[$op].am.bytes
    set $fmt   = OPCodeTable[$op].am.prfmt
    set $ins   = OPCodeTable[$op].ins

    set $d = $d - 1

    if $bytes == 1
          printf "0x%04x:           : %s\n", $sdaddr, $ins->memonic 
    else 
      if $bytes == 2
          printf "0x%04x: 0x%02x      : %s 0x%02x   (%s)\n", $sdaddr, m->memory[$sdaddr+1], $ins->memonic, m->memory[$sdaddr+1], $fmt
      else     
        if $bytes == 3
          printf "0x%04x: 0x%02x 0x%02x : %s 0x%02x%02x (%s)\n", $sdaddr, m->memory[$sdaddr+1], m->memory[$sdaddr+2], $ins->memonic, m->memory[$sdaddr+2], m->memory[$sdaddr+1], $fmt 
        else 
	  printf "ERROR: invalid opcode? 0x%04x opcode=0x%02x\n", $sdaddr, $op
        end
      end
    end
    set $sdaddr = $sdaddr + $bytes

  end

end

document sdisassemble
  sdisassemble <addr> [count], sdissaemble invoked with no arguments will disassemble from the last address for the last count specified
end
  

define srst
  sxs 0xFFFC
end
document srst
  print the address of the installed reset interrupt service routine
end

define snmi
  sxs 0xFFFA
end
document snmi
  print the address of the installed non-maskable interrupt service routine
end

define sirq
  sxs 0xFFFE
end
document sirq
  print the address of the installed irq interrupt service routine
end

define sreg
  p /x m->cpu.reg
end
document sreg
  print 6502 registers
end

define ssp
  sxs 0x0000
end
document ssp
  print cc65 argument stackpointer (initialized to 0xFEFF in crt0.s)
end

define ssbyte
  set $lb = m->memory[0x0000]
  set $hb = m->memory[0x0001]
  set $ad = ($hb << 8) | $lb
  set $num = (0xfeff - $ad)
  sxb $ad $num
end
document ssb
  print cc65 argument stack as bytes
end

define ssshort
  set $lb = m->memory[0x0000]
  set $hb = m->memory[0x0001]
  set $ad = ($hb << 8) | $lb
  set $num = (0xfeff - $ad) / 2
  sxs $ad $num
end
document sss
  print cc65 argument stack as shorts/addresses
end

define sbt
  set $sbtaddr = 0x01fe
  set $sbttaddr = ((0x01 << 8) | m->cpu.reg.sp)
 
  while $sbtaddr > $sbttaddr 
    set $lb = m->memory[$sbtaddr]
    set $hb = m->memory[$sbtaddr+1]
    set $sbtca = (($hb << 8) | $lb) - 2
    printf "(sp=0x%04x) ", $sbtaddr
    sd $sbtca 1
    set $sbtaddr = $sbtaddr - 2
  end

end
document sbt
  print attempt to produce a back trace from 6502 stack page this may not alway work as there maybe other values pushed on the stack other than JSR addresses
end
define sinfo 
  printf "pc=0x%04x ac=0x%02x x=0x%02x y=0x%02x sr=0x%02x sp=0x01%02x asp=0x%02x%02x srbits:\n",  m->cpu.reg.pc, m->cpu.reg.ac, m->cpu.reg.x, m->cpu.reg.y, m->cpu.reg.sr, m->cpu.reg.sp, m->memory[1], m->memory[0]
  p/t m->cpu.reg.sr
end
document sinfo
  pretty print register state
end

define strace
  b fetch
end
document strace
  set breakpoint for single stepping simulated instructions
end

define sstep
  c
  sinfo
  sd m->cpu.reg.pc
end
document sstep
  single step simulated instruction
end
   


  