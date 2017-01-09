#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

static void interrupt(void) {
	
}

int main(void) {

	printf("Resetting CPU\n");
	cpu_reset();

	printf("Starting emulation\n");

	
	cpu_emulate(interrupt);
	return 0;
}