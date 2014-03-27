# Copyright (C) 2011 by Jonathan Appavoo, Boston University
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
O=0
CFLAGS=-O${O} -Wall -g -D IMP

targets = 6502
hdrs= $(wildcard *.h)
src  = $(wildcard *.c)
objs = $(patsubst %.c,%.o,$(src))

all: $(targets)
.PHONY: all

tests: compile memcpy concpy unknown1 unknown2

6502: $(objs)
	gcc $(CFLAGS) -o $@ $(objs)

compile: 
	@bash -c 'make clean; make 6502 2> compile.out; num=$$(wc -l compile.out);\
	 if [[ -a 6502 && $$num=0 ]]; then echo COMPILE: PASS; else echo COMPILE: FAIL; fi' 

memcpy: memcpy.res

memcpy.res: 6502 apps/memcpy.img
	bash -c './6502 apps/memcpy.img memcpy.img.out < /dev/null  > /dev/null  2> memcpy.trc'
	echo "INPUT:" > memcpy.res
	./img dump apps/memcpy.img >> memcpy.res
	echo "OUTPUT:" >> memcpy.res
	./img dump memcpy.img.out >> memcpy.res
	cat memcpy.res
	@bash -c 'num=$$(grep Hello memcpy.res | wc -l);\
	 if (( num == 3 )); then echo MEMCOPY: PASS; else echo MEMCOPY: FAIL; fi' 

concpy: concpy.res

concpy.res: 6502 apps/concpy.img
	bash -c 'echo "Hello World" | ./6502 apps/concpy.img concpy.img.out  > concpy.con  2> concpy.trc'
	cmp apps/concpy.img concpy.img.out
	cat concpy.con | grep -v '^6502Lab' > concpy.res
	cat concpy.res
	@bash -c 'res=$$(cat concpy.res);\
	 if [[ $$res = "Hello World" ]]; then echo CONCPY: PASS; else echo CONCPY: FAIL; fi' 

unknown1: unknown1.res

unknown1.res: 6502 apps/unknown.img
	bash -c 'echo -e "q" | ./6502 apps/unknown.img unknown1.img.out > unknown1.con 2> unknown1.trc'
	@bash -c 'num=$$(grep 0 unknown1.con | wc -l);\
	 if (( num == 12 )); then echo UNKNOWN1: PASS; else echo UNKNOWN1: FAIL; fi' 

unknown2: unknown2.res

unknown2.res: 6502 apps/unknown.img
	bash -c 'echo -e "cp6343\np7655\np7245\np7152\np4334\np4536\np5233\np0002\n3625\np7362\np6444\np7531\np5543\np4324\np3325\np2513\np7072\np2412\np6271\np7170\np7473\np7323\np3122\np2322\npq" | ./6502 apps/unknown.img unknown2.img.out > unknown2.con 2> unknown2.trc'
	@bash -c 'num=$$(grep FF.FF.FF unknown2.con | wc -l);\
	 if (( num == 1 )); then echo UNKNOWN2: PASS; else echo UNKNOWN2: FAIL; fi' 

# use kill -s SIGUSR1 <pid> to send nmi
# use kill -s SIGUSR2 <pid> to send irq
interrupt.res: 6502 apps/interrupt.img
	bash -c './6502 apps/interrupt.img interrupt.img.out 100000000000 > interrupt.con 2> interrupt.trc & jobs -p; wait'
	./img dump apps/interrupt.img | grep -v '^000001.. ' > interrupt.img.dump
	./img dump interrupt.img.out  | grep -v '^000001.. ' > interrupt.img.out.dump
	diff interrupt.img.dump interrupt.img.out.dump
	cat interrupt.trc
	cat interrupt.con 
	echo "DONE"

apps/memcpy.img:
	make -C apps memcpy.img

apps/concpy.img:
	make -C apps concpy.img

apps/interrupt.img:
	make -C apps interrupt.img

apps/unknown.img:
	make -C apps unknown.img

handout: clean handoutdir handoutcopy handoutinst handoutloop handoutconsole

handoutdir:
	mkdir handout

handoutcopy: apps/unknown.img
	cp img Makefile *.c *.h .gdbinit handout
	cp -r apps handout
	cp -r doc handout
	rm handout/apps/unknown.s

handoutinst:
	cat instruction.c | sed '/.*CODE: BEGIN/,$$d' > handout/instruction.c
	cat instruction.c | sed '1,/.*CODE: BEGIN/d' | gcc -E -U IMP - | grep -v '^# ' >> handout/instruction.c

handoutloop:
	cat loop.c | sed '/.*CODE: BEGIN/,$$d' > handout/loop.c
	cat loop.c | sed '1,/.*CODE: BEGIN/d' | gcc -E -U IMP - | grep -v '^# ' >> handout/loop.c

handoutconsole:
	cat console.c | sed '/.*CODE: BEGIN/,$$d' > handout/console.c
	cat console.c | sed '1,/.*CODE: BEGIN/d' | gcc -E -U IMP - | grep -v '^# ' >> handout/console.c

$(objs) : $(src) $(hdrs)

clean:
	make -C apps clean
	-rm -rf $(wildcard $(objs) $(targets) handout *.res *.con *.out *.trc *.img *.dumpmake)
