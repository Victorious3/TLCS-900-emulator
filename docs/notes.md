Some Rambling Notes
===================

the TLCS stuff has similarities to the Z80 but they are not compatible.
Apparently the TLCS-90 is binary compatible with the Z80.
The TLCS-900 **is not** binary compatible with the Z80, though the assembly language is similar.
This should make it "easy" to port software over, at least.

## Where to start execution from.
If this is Z80 based I'd assume it's started from address 0.
However, Vic noted that the docs mentioned reset vectors which are basically an addess that is read and jumped to...
