#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "900L1.h"
#include "memory.h"
#include "register.h"
#include "instruction.h"

struct CPU_STATE CPU_STATE;

DWORD cpu_getaddr(BYTE address_mode) {
	if (address_mode & 0x40) {
		// (R32)
		if (address_mode & 0x8) {
			return *cpu_getR_dw(address_mode & 0x7);
		}
		// (R32+d8)
		return *cpu_getR_dw(address_mode & 0x7) + cpu_pull_op_b();
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
			int b = cpu_pull_op_b();
			switch (b & 0x2) {
			case 0:
				// (r32)
				return *cpu_getr_dw(b & 0x3C);
			case 1:
				// (r32+d16)
				return *cpu_getr_dw(b & 0x3C) + cpu_pull_op_w();
			default:
				// (r32+r8)
				if (b & 0xFC) 
					return *cpu_getr_dw(cpu_pull_op_b()) + *cpu_getr_b(cpu_pull_op_b());
				// (r32+r16)
				return *cpu_getr_dw(cpu_pull_op_b()) + *cpu_getr_w(cpu_pull_op_b());
			}
		}

	case 4: {
			int b = cpu_pull_op_b();
			int disp = (b & 0x3);
			
			// (-r32)
			DWORD* reg = cpu_getr_dw(b & 0x3C);
			int address = *reg - (disp == 0 ? 1: disp == 1 ? 2 : 4);
			*reg = address;
			return address;
		}

	default: {
			int b = cpu_pull_op_b();
			int disp = (b & 0x3);

			// (r32+)
			DWORD* reg = cpu_getr_dw(b & 0x3C);
			int address = *reg;
			*reg = address + (disp == 0 ? 1 : disp == 1 ? 2 : 4);
			return address;
		}
	}
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
			BYTE op = cpu_pull_op_b();
			cpu_optable[op](op);
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
