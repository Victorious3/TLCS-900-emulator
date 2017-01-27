#pragma once

#include "../900L1.h"

// Flags change for arithmetic operations

static inline void flags_ar_b(BYTE dst, BYTE src, BYTE res) {
	CPU_STATE.S = (res & 0x80) != 0;
	CPU_STATE.Z = res == 0;
	CPU_STATE.H = ((dst & 0xF + src & 0xF) & 0x10) != 0;
	CPU_STATE.V = ((src ^ dst) & 0x80) ? ((src ^ res) & 0x80) != 0 : 0;
	CPU_STATE.N = 0;
	CPU_STATE.C = res < dst;
}

static inline void flags_ar_w(WORD dst, WORD src, WORD res) {
	CPU_STATE.S = (res & 0x8000) != 0;
	CPU_STATE.Z = res == 0;
	CPU_STATE.H = ((dst & 0xF00 + src & 0xF00) & 0x1000) != 0; // High byte
	CPU_STATE.V = ((src ^ dst) & 0x8000) ? ((src ^ res) & 0x8000) != 0 : 0;
	CPU_STATE.N = 0;
	CPU_STATE.C = res < dst;
}

static inline void flags_ar_dw(DWORD dst, DWORD src, DWORD res) {
	CPU_STATE.S = (res & 0x8000000) != 0;
	CPU_STATE.Z = res == 0;
	// CPU_STATE.H undefined
	CPU_STATE.V = ((src ^ dst) & 0x80000000) ? ((src ^ res) & 0x80000000) != 0 : 0;
	CPU_STATE.N = 0;
	CPU_STATE.C = res < dst;
}

// Flags change for logical operations

static inline void flags_log_b(BYTE res) {
	CPU_STATE.S = (res & 0x80) != 0;
	CPU_STATE.Z = res == 0;
	CPU_STATE.H = 1;
	CPU_STATE.V = (((res * 0x0101010101010101ULL) & 0x8040201008040201ULL) % 0x1FF) & 1;
	CPU_STATE.N = CPU_STATE.C = 0;
}

static inline void flags_log_w(WORD res) {
	CPU_STATE.S = (res & 0x8000) != 0;
	CPU_STATE.Z = res == 0;
	CPU_STATE.H = 1;
	{
		WORD v = res;
		v ^= v >> 8;
		v ^= v >> 4;
		v &= 0x0F;
		CPU_STATE.V = ((0x6996 >> v) & 1) != 0; // TODO Parity of the entire word?
	}
	CPU_STATE.N = CPU_STATE.C = 0;
}

static inline void flags_log_dw(DWORD res) {
	CPU_STATE.S = (res & 0x80000000) != 0;
	CPU_STATE.Z = res == 0;
	CPU_STATE.H = 1;
	// CPU_STATE.V; Undefined
	CPU_STATE.N = CPU_STATE.C = 0;
}