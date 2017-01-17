#pragma once

#include "900L1.h"

// Macro to overload getmem
#define CPU_GETMEM(size, address) (\
	(size) == S_WORD ? cpu_getmem_w((address)) : \
	(size) == S_DWORD ? cpu_getmem_dw((address)) : \
	cpu_getmem_b((address)) \
)

// Macro to overload setmem
#define CPU_SETMEM(size, address, value) (\
	(size) == S_WORD ? cpu_setmem_w((address), (value)) : \
	(size) == S_DWORD ? cpu_setmem_dw((address), (value)) : \
	cpu_setmem_b((address), (value)) \
)

enum { CPU_MEMBUS_SIZE = 4 };

/// Four memory channels, to be set by extension methods
/// Each channel is polled in sequence until one bus accepts
/// by returning true
extern bool (*CPU_MEMBUS[CPU_MEMBUS_SIZE])(DWORD address, BYTE* mem, bool read);

BYTE cpu_getmem_b(DWORD address);
WORD cpu_getmem_w(DWORD address);
DWORD cpu_getmem_dw(DWORD address);

void cpu_setmem_b(DWORD address, BYTE value);
void cpu_setmem_w(DWORD address, WORD value);
void cpu_setmem_dw(DWORD address, DWORD value);

// Macro to overload pull_op
#define CPU_PULL_OP(size) (\
	(size) == S_WORD ? cpu_pull_op_w() : \
	(size) == S_DWORD ? cpu_pull_op_dw() : \
	cpu_pull_op_b() \
)

BYTE cpu_pull_op_b(void);
WORD cpu_pull_op_w(void);
DWORD cpu_pull_op_dw(void);

// Macro to overload pull_op
#define CPU_PEEK_OP(size, offset) (\
	(size) == S_WORD ? cpu_pull_op_w() : \
	(size) == S_DWORD ? cpu_pull_op_dw() : \
	cpu_pull_op_b() \
)

BYTE cpu_peek_op_b(BYTE offset);
WORD cpu_peek_op_w(BYTE offset);
DWORD cpu_peek_op_dw(BYTE offset);