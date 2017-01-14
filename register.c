#include "register.h"

BYTE cpu_getr_b(BYTE reg) {
	if (reg > 0x3C) {
		if (reg > 0xF0) return CPU_STATE.REG.byte[reg & 0xF];
		if (reg > 0xE0) return CPU_STATE.BANK.byte[((CPU_STATE.RFP + 7) % 8) + (reg & 0xF)];
		return CPU_STATE.BANK.byte[CPU_STATE.RFP + (reg & 0xF)];
	}
	return CPU_STATE.BANK.byte[reg];
}

WORD cpu_getr_w(BYTE reg) {
	if (reg > 0x3C) {
		if (reg > 0xF0) return CPU_STATE.REG.word[(reg & 0xF) >> 2];
		if (reg > 0xE0) return CPU_STATE.BANK.word[(((CPU_STATE.RFP + 7) % 8) + (reg & 0xF)) >> 2];
		return CPU_STATE.BANK.word[CPU_STATE.RFP + (reg & 0xF)];
	}
	return CPU_STATE.BANK.word[reg];
}

DWORD cpu_getr_dw(BYTE reg) {
	if (reg > 0x3C) {
		if (reg > 0xF0) return CPU_STATE.REG.dword[(reg & 0xF) >> 4];
		if (reg > 0xE0) return CPU_STATE.BANK.dword[(((CPU_STATE.RFP + 7) % 8) + (reg & 0xF)) >> 4];
		return CPU_STATE.BANK.dword[CPU_STATE.RFP + (reg & 0xF)];
	}
	return CPU_STATE.BANK.dword[reg];
}

void cpu_setr_b(BYTE reg, BYTE value) {
	if (reg > 0x3C) {
		if (reg > 0xF0) CPU_STATE.REG.byte[reg & 0xF] = value;
		else if (reg > 0xE0) CPU_STATE.BANK.byte[((CPU_STATE.RFP + 7) % 8) + (reg & 0xF)] = value;
		else CPU_STATE.BANK.byte[CPU_STATE.RFP + (reg & 0xF)] = value;
	}
	else CPU_STATE.BANK.byte[reg] = value;
}

void cpu_setr_w(BYTE reg, WORD value) {
	if (reg > 0x3C) {
		if (reg > 0xF0) CPU_STATE.REG.word[(reg & 0xF) >> 2] = value;
		else if (reg > 0xE0) CPU_STATE.BANK.word[(((CPU_STATE.RFP + 7) % 8) + (reg & 0xF)) >> 2] = value;
		else CPU_STATE.BANK.word[CPU_STATE.RFP + (reg & 0xF)] = value;
	}
	else CPU_STATE.BANK.word[reg] = value;
}

void cpu_setr_dw(BYTE reg, DWORD value) {
	if (reg > 0x3C) {
		if (reg > 0xF0) CPU_STATE.REG.dword[(reg & 0xF) >> 4] = value;
		else if (reg > 0xE0) CPU_STATE.BANK.dword[(((CPU_STATE.RFP + 7) % 8) + (reg & 0xF)) >> 4] = value;
		else CPU_STATE.BANK.dword[CPU_STATE.RFP + (reg & 0xF)] = value;
	}
	else CPU_STATE.BANK.dword[reg] = value;
}

static int const reg_R[8] = {0x1, 0x0, 0x5, 0x4, 0x9, 0x8, 0xD, 0xC};

BYTE cpu_getR_b(BYTE reg) {
	return CPU_STATE.BANK.byte[CPU_STATE.RFP + reg_R[reg]];
}

WORD cpu_getR_w(BYTE reg) {
	return CPU_STATE.BANK.word[(CPU_STATE.RFP + reg_R[reg]) >> 2];
}

DWORD cpu_getR_dw(BYTE reg) {
	return CPU_STATE.BANK.dword[(CPU_STATE.RFP + reg_R[reg]) >> 4];
}

void cpu_setR_b(BYTE reg, BYTE value) {
	CPU_STATE.BANK.byte[CPU_STATE.RFP + reg_R[reg]] = value;
}

void cpu_setR_w(BYTE reg, WORD value) {
	CPU_STATE.BANK.word[(CPU_STATE.RFP + reg_R[reg]) >> 2] = value;
}

void cpu_setR_dw(BYTE reg, DWORD value) {
	CPU_STATE.BANK.dword[(CPU_STATE.RFP + reg_R[reg]) >> 4] = value;
}