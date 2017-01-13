#include <stdio.h>
#include <stdlib.h>

#include "900L.h"

static void interrupt(void) {
	
}

int main(void) {

	printf("Initializing CPU\n");
	cpu_init();

	printf("Starting emulation\n");
	
	cpu_emulate(interrupt);
	return 0;
}