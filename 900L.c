#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "900L.h"
#include "memory.h"
#include "register.h"

// Pop an operant of the specified size (increments PC)
static BYTE pop_b() {
	BYTE x = cpu_getmem_b(CPU_STATE.PC);
	CPU_STATE.PC++;
	return x;
}

static WORD pop_w() {
	WORD x = cpu_getmem_w(CPU_STATE.PC);
	CPU_STATE.PC += 2;
	return x;
}

static WORD pop_dw() {
	WORD x = cpu_getmem_w(CPU_STATE.PC);
	CPU_STATE.PC += 2;
	return x;
}

/// Memory decoding, returns a memory address to be used with cpu_getmem_(size) and cpu_setmem_(size) from memory.h
/// -m--mmmm, all other bits are ignored
static DWORD getaddr(BYTE address_mode) {
	if (address_mode & 0x40) {
		// (R32)
		if (address_mode & 0x8) {
			return cpu_getR_dw(address_mode & 0x7);
		}
		// (R32+d8)
		return cpu_getR_dw(address_mode & 0x7) + pop_b();
	}
	
	switch (address_mode & 0x7) {
	case 0:
		// (#8)
		return pop_b();
	case 1:
		// (#16)
		return pop_w();
	case 2:
		// (#24)
		return pop_w() | (pop_b() << 16);
	case 3: {
			int b = pop_b();
			switch (b & 0x2) {
			case 0:
				// (r32)
				return cpu_getr_dw(b & 0x3C);
			case 1:
				// (r32+d16)
				return cpu_getr_dw(b & 0x3C) + pop_w();
			default:
				// (r32+r8)
				if (b & 0xFC) 
					return cpu_getr_dw(pop_b()) + cpu_getr_b(pop_b());
				// (r32+r16)
				return cpu_getr_dw(pop_b()) + cpu_getr_w(pop_b());
			}
		}

	case 4: {
			int b = pop_b();
			int reg = b & 0x3C;
			int disp = (b & 0x3);
			
			// (-r32)
			int address = cpu_getr_dw(reg) - (disp == 0 ? 1: disp == 1 ? 2 : 4);
			cpu_setr_dw(reg, address);
			return address;
		}

	default: {
			int b = pop_b();
			int reg = b & 0x3C;
			int disp = (b & 0x3);

			// (r32+)
			int address = cpu_getr_dw(reg);
			cpu_setr_dw(reg, address + (disp == 0 ? 1 : disp == 1 ? 2 : 4));
			return address;
		}
	}
}

// Instructions

static int const INTERRUPT_PERIOD = 10;

static bool exit_flag = false;

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

void cpu_exit() {
	exit_flag = true;
}

void cpu_run(void (*interrupt)(void)) {
	int interrupt_counter = INTERRUPT_PERIOD;

	while (true) {
		
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
