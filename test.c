#include <stdio.h>
#include <stdlib.h>

#include "900L.h"
#include "TMP91C016.h"
#include "register.h"
#include "memory.h"

static void interrupt(void) {
	
}

int main(void) {

	printf("Initializing Microcontroller\n");
	mc_init();

	printf("Starting emulation\n");
	cpu_run(interrupt);

	return 0;
}