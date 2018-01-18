#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "900L1.h"
#include "memory.h"
#include "register.h"
#include "opcodes.h"

struct CPU_STATE CPU_STATE;

DWORD cpu_getaddr(BYTE address_mode) {
	if (address_mode & 0x40) {
		// (R32)
		if (address_mode & 0x8) {
			return getr_dw(cpu_getR(address_mode & 0x7));
		}
		// (R32+d8)
		return getr_dw(cpu_getR(address_mode & 0x7)) + cpu_pull_op_b();
	}
	
	switch (address_mode & 0x7) {
	case 0:
		// (#8)
		return cpu_pull_op_b();
	case 1:
		// (#16)
		return cpu_pull_op_w();
	case 2:
		// (#24)
		return cpu_pull_op_w() | (cpu_pull_op_b() << 16);
	case 3: {
			BYTE b = cpu_pull_op_b();
			switch (b & 0x2) {
			case 0:
				// (r32)
				return getr_dw(cpu_getR(b & 0x3C));
			case 1:
				// (r32+d16)
				return getr_dw(cpu_getR(b & 0x3C)) + cpu_pull_op_w();
			default:
				// (r32+r8)
				b &= 0xFC;
				if (b == 1)
					return getr_dw(cpu_getR(cpu_pull_op_b())) + getr_dw(cpu_getR(cpu_pull_op_b()));
				// (r32+r16)
				else if (b == 0)
					return getr_dw(cpu_getR(cpu_pull_op_b())) + getr_dw(cpu_getR(cpu_pull_op_b()));
				// LDAR ($ + 4 + d16)
				else if (b == 2)
					return cpu_pull_op_w() + CPU_STATE.PC + 1; // +1 for following second opcode
			}
		}

	case 4: {
			int b = cpu_pull_op_b();
			int disp = (b & 0x3);
			
			// (-r32)
			BYTE* reg = cpu_getr(b & 0x3C);
			int addr = getr_dw(reg) - (disp == 0 ? 1: disp == 1 ? 2 : 4);
			setr_dw(reg, addr);
			return addr;
		}

	default: {
			int b = cpu_pull_op_b();
			int disp = (b & 0x3);

			// (r32+)
			BYTE* reg = cpu_getr(b & 0x3C);
			int addr = getr_dw(reg);
			setr_dw(reg, addr + (disp == 0 ? 1 : disp == 1 ? 2 : 4));
			return addr;
		}
	}
	return 0;
}

void cpu_init() {

	// Check if the sizes align
	assert(sizeof(BYTE) == 1);
	assert(sizeof(WORD) == 2);
	assert(sizeof(DWORD) == 4);

	// Enforce little endian
	int i = 1;
	assert(*(char*)&i == 1);

	cpu_reset();
}

void cpu_reset() {
	
}

static int const INTERRUPT_PERIOD = 10;
static bool exit_flag = false;

void cpu_exit() {
	exit_flag = true;
}

void cpu_run(void (*interrupt)(void)) {
	int interrupt_counter = INTERRUPT_PERIOD;

	while (true) {
		if (!CPU_STATE.halt) {
			// Exectute next instruction
			cpu_exec_insn();
		}

		if (interrupt_counter <= 0) {			
			interrupt_counter = INTERRUPT_PERIOD;

			// Call interrupt
			interrupt();

			// Exit in case any interrupt aborts
			if (exit_flag) break;
		}
		interrupt_counter--;
	}
}
