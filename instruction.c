#include "instruction.h"
#include "register.h"

// NOP - Do nothing.
void NOP(BYTE f) {}

// PUSH_SR, push the status register to the stack.
// (Decrements xsp by 2, then places the 2 byte status register at the "top" of the stack).
// (-XSP) <- SR
void PUSH_SR(BYTE f) {
	cpu_stack_push_w(CPU_STATE.SR);
}

