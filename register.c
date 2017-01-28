#include "register.h"
#include "memory.h"

BYTE* cpu_getr(BYTE reg) {
	if (reg > 0x3C) {
		if (reg > 0xF0) return CPU_STATE.REG + (reg & 0xF);
		if (reg > 0xE0) return CPU_STATE.BANK + (((CPU_STATE.RFP + 7) % 8) + (reg & 0xF));
		return CPU_STATE.BANK + (CPU_STATE.RFP + (reg & 0xF));
	}
	return CPU_STATE.BANK + reg;
}

static int const reg_R[8] = {0x1, 0x0, 0x5, 0x4, 0x9, 0x8, 0xD, 0xC};

BYTE* cpu_getR(BYTE reg) {
	return CPU_STATE.BANK + (CPU_STATE.RFP + reg_R[reg]);
}

BYTE cpu_stack_pop_b() {
	return cpu_getmem_b((*(DWORD*)(CPU_STATE.REG + 0xC))++);
}

WORD cpu_stack_pop_w() {
	DWORD* xsp = (DWORD*)(CPU_STATE.REG + 0xC);
	WORD value = cpu_getmem_w(*xsp);
	*xsp += S_WORD;
	return value;
}

DWORD cpu_stack_pop_dw() {
	DWORD* xsp = (DWORD*)(CPU_STATE.REG + 0xC);
	DWORD value = cpu_getmem_dw(*xsp);
	*xsp += S_DWORD;
	return value;
}


void cpu_stack_push_b(BYTE value) {
	cpu_setmem_b(--(*(DWORD*)(CPU_STATE.REG + 0xC)), value);
}

void cpu_stack_push_w(WORD value) {
	DWORD* xsp = (DWORD*)(CPU_STATE.REG + 0xC);
	*xsp -= S_WORD;
	cpu_setmem_w(*xsp, value);
}

void cpu_stack_push_dw(DWORD value) {
	DWORD* xsp = (DWORD*)(CPU_STATE.REG + 0xC);
	*xsp -= S_DWORD;
	cpu_setmem_dw(*xsp, value);
}