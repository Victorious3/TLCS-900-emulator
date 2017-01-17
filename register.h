#include "900L1.h"

// Macro to autocast a register to the required size
#define REG(reg, size) (\
	(size) == S_WORD ? (WORD*)(reg) : \
	(size) == S_WORD ? (DWORD*)(reg) : \
	(reg)\
)

// Macro to overload getr
#define CPU_GETr(size, reg) (\
	(size) == S_WORD ? cpu_getr_w((reg)) : \
	(size) == S_DWORD ? cpu_getr_dw((reg)) : \
	cpu_getr_b((reg)) \
)

// Macro to overload getR
#define CPU_GETR(size, reg) (\
	(size) == S_WORD ? cpu_getR_w((reg)) : \
	(size) == S_DWORD ? cpu_getR_dw((reg)) : \
	cpu_getR_b((reg)) \
)

// Register access

/// General purpose & bank registers, byte
BYTE* cpu_getr_b(BYTE reg);
/// General purpose & bank registers, word
WORD* cpu_getr_w(BYTE reg);
/// General purpose & bank registers, dword
DWORD* cpu_getr_dw(BYTE reg);

BYTE* cpu_getR_b(BYTE reg);
WORD* cpu_getR_w(BYTE reg);
DWORD* cpu_getR_dw(BYTE reg);

// Macro to overload stack_pop
#define CPU_STACK_POP(size) (\
	(size) == S_WORD ? cpu_stack_pop_w() : \
	(size) == S_DWORD ? cpu_stack_pop_dw() : \
	cpu_stack_pop_b() \
)

BYTE cpu_stack_pop_b(void);
WORD cpu_stack_pop_w(void);
DWORD cpu_stack_pop_dw(void);

// Macro to overload stack_push
#define CPU_STACK_PUSH(size, value) (\
	(size) == S_WORD ? cpu_stack_push_w((value)) : \
	(size) == S_DWORD ? cpu_stack_push_dw((value)) : \
	cpu_stack_push_b((value)) \
)

void cpu_stack_push_b(BYTE value);
void cpu_stack_push_w(WORD value);
void cpu_stack_push_dw(DWORD value);