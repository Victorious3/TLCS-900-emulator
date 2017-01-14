#pragma once

#include "900L.h"

enum { CPU_MEMBUS_SIZE = 8 };

/// Four memory channels, to be set by extension methods
/// Each channel is polled in sequence until one bus accepts
/// by returning true
bool(*CPU_MEMBUS[CPU_MEMBUS_SIZE])(DWORD address, BYTE* mem, bool read);

BYTE cpu_getmem_b(DWORD address);

WORD cpu_getmem_w(DWORD address);

DWORD cpu_getmem_dw(DWORD address);

void cpu_setmem_b(DWORD address, BYTE value);

void cpu_setmem_w(DWORD address, WORD value);

void cpu_setmem_dw(DWORD address, DWORD value);