#include "memory.h"

bool(*CPU_MEMBUS[CPU_MEMBUS_SIZE])(DWORD address, BYTE* mem, bool read);

BYTE cpu_getmem_b(DWORD address) {
	for (int i = 0; i < CPU_MEMBUS_SIZE; i++) {
		bool (*bus)(DWORD, BYTE*, bool) = CPU_MEMBUS[i];
		if (!bus) continue;
		BYTE in;
		bool succ = bus(address, &in, true);
		if (!succ) continue;
		return in;
	}
	// No memory found for address, returning 0
	return 0;
}

WORD cpu_getmem_w(DWORD address) {
	return cpu_getmem_b(address) | cpu_getmem_b(address + 1) << 8;
}

DWORD cpu_getmem_dw(DWORD address) {
	return cpu_getmem_w(address) | cpu_getmem_b(address + 2) << 16;
}

void cpu_setmem_b(DWORD address, BYTE value) {
	for (int i = 0; i < CPU_MEMBUS_SIZE; i++) {
		bool (*bus)(DWORD, BYTE*, bool) = CPU_MEMBUS[i];
		if (!bus) continue;
		bool succ = bus(address, &value, false);
		if (!succ) continue;
	}
}

void cpu_setmem_w(DWORD address, WORD value) {
	cpu_setmem_b(address, (BYTE)value);
	cpu_setmem_b(address + 1, (BYTE)((value & 0xFF00) >> 8));
}

void cpu_setmem_dw(DWORD address, DWORD value) {
	cpu_setmem_w(address, (WORD)value);
	cpu_setmem_w(address + 1, (WORD)((value & 0xFFFF0000) >> 16));
}