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

static inline void mc_setr_range(enum MC_REG from, enum MC_REG to, BYTE value) {
	memset(MC.REG + from, value, to - from + 1);
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

		ex_mem->access(address, mem, read, ex_mem->userdata);
		return true;
	}
	return false;
}

static void reset() {
	// I/O ports
	mc_setr(P2, 0xFF);

	// I/O port control
	mc_setr(P2FC, 0xFF); // Port 2 is used as address bus
	mc_setr(P5UDE, 0x2C);
	mc_setr(P6UE, 0x33);
	mc_setr(P7UDE, 0x18);
	mc_setr(P9UE, 0xFF);
	mc_setr(PBUDE, 0x8);
	mc_setr(PCCR, 0xC0);
	mc_setr(PDUE, 0x1F);

	// CS/WAIT
	mc_setr_range(MSAR0, MAMR3, 0xFF);

	// Chip 2 is enabled by default
	mc_setr(B2CS, 0x80);

	// Clock gear
	mc_setr(SYSCR0, 0xE0);
	mc_setr(SYSCR1, 0x4);
	mc_setr(SYSCR2, 0x2C);
	mc_setr(EMCCR0, 0x23);
	// TODO: EMCCR1, EMCCR2 ?

	// DFM Control
	mc_setr(DFMCR1, 0x13);

	// 8-bit timer
	mc_setr(TA1FFCR, 0xC);
	mc_setr(TA3FFCR, 0xC);

	// DRAM control
	mc_setr(DMEMCR, 0x80);

	// Watchdog timer
	mc_setr(WDMOD, 0x80);
	// TODO: WDCR?

	// TODO: RTC (Real time clock) Set to current time?

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
	memset(MC.REG, 0, sizeof (MC.REG)); // Resets all registers to 0
	reset();
}

static void (*ex_interrupt)(void);

static void mc_interrupt(void) {
	ex_interrupt();
}

void mc_run(void (*interrupt)(void)) {
	ex_interrupt = interrupt;
	cpu_run(mc_interrupt);
}