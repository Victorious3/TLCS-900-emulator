#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

int main() {

	printf("Resetting CPU\n");
	cpu_reset();

	printf("Starting emulation\n");
	cpu_emulate();
	return 0;
}