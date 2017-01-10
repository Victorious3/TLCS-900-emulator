Some Rambling Notes
===================

the TLCS stuff has similarities to the Z80 but they are not compatible.
Apparently the TLCS-90 is binary compatible with the Z80.
The TLCS-900 **is not** binary compatible with the Z80, though the assembly language is similar.
This should make it "easy" to port software over, at least.

## Where to start execution from.
Reset vector is at FFFF00h (0xFFFF00). The three bytes stored there is the PC used to start the execution.
(what size is the value?)
The stack pointer is at 100h (0x100)?
