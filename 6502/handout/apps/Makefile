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
all: memcpy.img concpy.img interrupt.img unknown.img
.PHONY: all

memcpy.img: memcpy.s
	./p65 -b memcpy.s memcpy.img | sed 's/-/ /g' > memcpy.txt
	../img resetvec memcpy.img 0x0200

concpy.img: concpy.s
	./p65 -b concpy.s concpy.img | sed 's/-/ /g' > concpy.txt
	../img resetvec concpy.img 0x0200


interrupt.img: interrupt.s
	./p65 -b interrupt.s interrupt.img | sed 's/-/ /g' > interrupt.txt
	../img resetvec interrupt.img 0x0200
	../img nmivec interrupt.img 0x0500
	../img irqvec interrupt.img 0x0600	

unknown.img: unknown.s
	./p65 -b unknown.s unknown.img | sed 's/-/ /g' > unknown.txt
	../img resetvec unknown.img 0x1000

clean:
	-rm -rf $(wildcard memcpy.img concpy.img interrupt.img memcpy.txt concpy.txt interrupt.txt *.dumpmake)
