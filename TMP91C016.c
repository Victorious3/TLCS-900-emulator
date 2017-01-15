#include "TMP91C016.h"
#include "memory.h"

// Internal registers
struct MC {
	// Theoretically this goes up to FE0h 
	// but all the higher registers are left undefined
	BYTE REG[0x45F];

	// CS/Wait specified extensions
	struct EX_MEMORY *EX_MEMORY[EX_MEMORY_SIZE];
} MC;

void mc_setexmem(int slot, struct EX_MEMORY* memory) {
	struct EX_MEMORY* current = MC.EX_MEMORY[slot];
	if (current && current->destroy)
		current->destroy(current);
	MC.EX_MEMORY[slot] = memory;
}

BYTE mc_getr(enum MC_REG reg) {
	return MC.REG[reg];
}

void mc_setr(enum MC_REG reg, BYTE value) {
	MC.REG[reg] = value;
}

/// Internal IO
static bool per_internal_io(DWORD address, BYTE* mem, bool read) {
	// Internal registers
	if (address < 0xFE0) {
		if (read) MC.REG[address] = *mem;
		else *mem = MC.REG[address];
		return true;
	}
	return false;
}

#include <stdio.h>

/// CS/Wait controller
static bool per_cs_wait(DWORD address, BYTE* mem, bool read) {
	for (int i = 0; i < EX_MEMORY_SIZE; i++) {
		struct EX_MEMORY* ex_mem = MC.EX_MEMORY[i];
		if (!ex_mem) continue;

		BYTE bcs = mc_getr(B0CS + i);

		// Check if enabled
		if (!(bcs & 0x80)) continue;

		// CS2 has an override setting which makes it active on the entire address range
		if (!(i == 2 && (!(bcs & 0x40)))) {
			DWORD start = ((DWORD)mc_getr(MSAR0 + i)) << 16;
			BYTE mamr0 = mc_getr(MAMR0 + i);
			DWORD mask = 0;

			// CS0 and CS1 use different mask settings
			switch (i) {
			case 0:
				mask = ((DWORD)mamr0 & 0xFC) << 13;
				mask |= mamr0 & 0x2 ? 0x3F00 : 0;
				mask |= mamr0 & 0x1 ? 0x1FF : 0xFF;
				break;
			case 1:
				mask = ((DWORD)mamr0 & 0xFC) << 14;
				mask |= mamr0 & 0x2 ? 0x7F00 : 0;
				mask |= mamr0 & 0x1 ? 0x1FF : 0xFF;
				break;
			default:
				mask = (((DWORD)mamr0) << 15) | 0x7FFF;
				break;
			}

			// Only multiples of the region's size are allowed as start value
			DWORD end = start | mask;
			if (address < start || address > end) continue;
		}
		// printf("Address: %X, Value: %x\n", B0CS + 2, bcs);
		ex_mem->access(address, mem, read, ex_mem->userdata);
		return true;
	}
	return false;
}

static void reset() {
	// CS/WAIT
	mc_setr(MSAR0, 0xFF); mc_setr(MSAR1, 0xFF);
	mc_setr(MSAR2, 0xFF); mc_setr(MSAR3, 0xFF);

	mc_setr(MAMR0, 0xFF); mc_setr(MAMR1, 0xFF);
	mc_setr(MAMR2, 0xFF); mc_setr(MAMR3, 0xFF);
	// Chip 2 is enabled by default
	mc_setr(B2CS, 0x80); 
}

void mc_init() {
	CPU_MEMBUS[0] = per_internal_io;
	CPU_MEMBUS[1] = per_cs_wait;

	// TODO: Implement MMU
	// CPU_MEMBUS[2] = per_mmu;

	cpu_init();
	reset();
}

void mc_reset() {
	cpu_reset();
	memset(&MC.REG, 0, sizeof (MC.REG)); // Resets all registers to 0
	reset();
}

static void(*ex_interrupt)(void);

static void mc_interrupt() {
	ex_interrupt();
}

void mc_run(void(*interrupt)(void)) {
	ex_interrupt = interrupt;
	cpu_run(mc_interrupt);
}