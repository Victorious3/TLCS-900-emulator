#include "TMP91C016.h"
#include "900L.h"
#include "memory.h"

// Internal registers
struct MC {
	// Theoretically this goes up to FE0h 
	// but all the higher registers are left undefined
	BYTE REG[0x45F];
} MC;

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

/// CS/Wait controller
static bool per_cs_wait(DWORD address, BYTE* mem, bool read) {

}

void mc_init() {
	CPU_MEMBUS[0] = per_internal_io;
	CPU_MEMBUS[1] = per_cs_wait;

	// TODO: Implement MMU
	// CPU_MEMBUS[2] = per_mmu;

	cpu_init();
}