#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int const INTERRUPT_PERIOD = 10;

static bool exit_flag = false;

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
