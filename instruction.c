#include "instruction.h"
#include "register.h"

/// NOP - Do nothing.
void NOP(BYTE f) {}

/// PUSH SR, push the status register to the stack.
/// Decrements xsp by 2, then places the 2 byte status register at the top of the stack.
/// (-XSP) <- SR
void PUSH_SR(BYTE f) {
	cpu_stack_push_w(CPU_STATE.SR);
}

/// PUSH A, push register A to the stack,
/// (-XSP) <- A
void PUSH_A(BYTE f) {
	cpu_stack_push_b(cpu_getR_b(0x1));
}

/// PUSH #, push a byte to the stack.
/// (-XSP) <- #
void PUSH_n(BYTE f) {
	cpu_stack_push_b(cpu_pull_op_b());
}

/// PUSHW #, push a word to the stack.
/// (-XSP) <- #
void PUSHW_nn(BYTE f) {
	cpu_stack_push_w(cpu_pull_op_w());
}

/// POP SR, pop the status register from the stack.
/// SR <- (XSP+)
void POP_SR(BYTE f) {
	CPU_STATE.SR = cpu_stack_pop_w();
}
