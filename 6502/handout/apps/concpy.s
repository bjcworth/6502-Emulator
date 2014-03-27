;; Copyright (C) 2011 by Jonathan Appavoo, Boston University
;;
;; Permission is hereby granted, free of charge, to any person obtaining a copy
;; of this software and associated documentation files (the "Software"), to deal
;; in the Software without restriction, including without limitation the rights
;; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;; copies of the Software, and to permit persons to whom the Software is
;; furnished to do so, subject to the following conditions:
;;
;; The above copyright notice and this permission notice shall be included in
;; all copies or substantial portions of the Software.
;; 
;; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
;; THE SOFTWARE.

.alias MEMSTART $0000
.alias ZPG      $0000
.alias STACK    $0100
.alias CODE     $0200 
.alias CONMEM   $FF00
.alias MEMEND   $FFFF

.org MEMSTART

.advance CODE
START:	LDX   #$00
LOOP:	LDA   CONMEM    ; load Acc sets Z flag
	BEQ   DONE      ; branch based on value loaded in Acc
        STA   CONMEM	; str does not affect any flags
	JMP   LOOP
DONE:	BRK


.advance MEMEND
.byte $00
