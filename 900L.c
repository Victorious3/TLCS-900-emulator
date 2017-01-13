#include "900L.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

BYTE cpu_getr_b(BYTE reg) {
	return 0;
}

WORD cpu_getr_w(BYTE reg) {
	return 0;
}

DWORD cpu_getr_dw(BYTE reg) {
	return 0;
}

void cpu_setr_b(BYTE reg, BYTE value) {

}

void cpu_setr_w(BYTE reg, WORD value) {

}

void cpu_setr_dw(BYTE reg, DWORD value) {

}

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

void cpu_emulate(void (*interrupt)(void)) {
	int interrupt_counter = INTERRUPT_PERIOD;

	while (true) {
		
		if (interrupt_counter <= 0) {			
			interrupt_counter = INTERRUPT_PERIOD;

			// Call interrupt
			(*interrupt)();

			// Exit in case any interrupt aborts
			if (exit_flag) break;
		}
		interrupt_counter--;
	}
}
