#pragma once

#include "900L1.h"

// Register access

/// General purpose & bank registers
BYTE* cpu_getr(BYTE reg);

/// General purpose
BYTE* cpu_getR(BYTE reg);

BYTE cpu_stack_pop_b(void);
WORD cpu_stack_pop_w(void);
DWORD cpu_stack_pop_dw(void);

void cpu_stack_push_b(BYTE value);
void cpu_stack_push_w(WORD value);
void cpu_stack_push_dw(DWORD value);

// TODO: big endian compatibility (also breaks strict aliasing)
static inline BYTE getr_b(BYTE* r) {
	return *r;
}

static inline void setr_b(BYTE* r, BYTE b) {
	*r = b;
}

static inline WORD getr_w(BYTE* r) {
	return *(WORD*)r;
}

static inline void setr_w(BYTE* r, WORD w) {
	*((WORD*)r) = w;
}

static inline DWORD getr_dw(BYTE* r) {
	return *(DWORD*)r;
}

static inline void setr_dw(BYTE* r, DWORD d) {
	*((DWORD*)r) = d;
}