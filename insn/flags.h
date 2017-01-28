#pragma once

#include "../900L1.h"

#define SF CPU_STATE.S
#define ZF CPU_STATE.Z
#define HF CPU_STATE.H
#define VF CPU_STATE.V
#define NF CPU_STATE.N
#define CF CPU_STATE.C

static inline void c_SF(enum OP_SIZE size, DWORD res) {
	DWORD mask = (size << 3) - 1;
	SF = (res & mask) != 0;
}

static inline void c_VF(enum OP_SIZE size, DWORD a, DWORD b, DWORD res) {
	DWORD mask = (size << 3) - 1;
	VF = ((a ^ b) & mask) ? ((a ^ res) & mask) != 0 : 0;
}

static inline void c_HF(enum OP_SIZE size, DWORD a, DWORD b) {
	DWORD mask = 0xF << ((size - 1) << 3);
	HF = ((b & mask + a & mask) & 0x10) != 0;
}

static inline void c_PF(enum OP_SIZE size, DWORD res) {
	if (size == S_BYTE) {
		VF = ((((BYTE)res * 0x0101010101010101ULL) & 0x8040201008040201ULL) % 0x1FF) & 1;
	}
	else {
		WORD v = (WORD)res;
		v ^= v >> 8;
		v ^= v >> 4;
		v &= 0x0F;
		VF = ((0x6996 >> v) & 1) != 0; // TODO Parity of the entire word?
	}
}