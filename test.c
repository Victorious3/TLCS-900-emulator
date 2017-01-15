#include <stdio.h>
#include <stdlib.h>

#include "900L1.h"
#include "TMP91C016.h"
#include "register.h"
#include "memory.h"
#include "module.h"

static void interrupt(void) {
	
}

int main(void) {

	printf("Initializing Microcontroller\n");
	mc_init();

	// 16mb of SRAM - Start address 0 end address 0xFFFFFF
	mc_setexmem(2, mod_ram(0x1000000, 0));

	cpu_setmem_b(0x12345, 23);
	printf("%u\n", cpu_getmem_b(0x12345));

	printf("Starting emulation\n");
	mc_run(interrupt);

	return 0;
}