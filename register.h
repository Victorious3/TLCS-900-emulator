#include "900L1.h"

// Register access

/// General purpose & bank registers, byte
BYTE cpu_getr_b(BYTE reg);

/// General purpose & bank registers, word
WORD cpu_getr_w(BYTE reg);

/// General purpose & bank registers, dword
DWORD cpu_getr_dw(BYTE reg);

/// General purpose & bank registers, byte
void cpu_setr_b(BYTE reg, BYTE value);

/// General purpose & bank registers, word
void cpu_setr_w(BYTE reg, WORD value);

/// General purpose & bank registers, dword
void cpu_setr_dw(BYTE reg, DWORD value);

BYTE cpu_getR_b(BYTE reg);

WORD cpu_getR_w(BYTE reg);

DWORD cpu_getR_dw(BYTE reg);

void cpu_setR_b(BYTE reg, BYTE value);

void cpu_setR_w(BYTE reg, WORD value);

void cpu_setR_dw(BYTE reg, DWORD value);


BYTE cpu_stack_pop_b(void);
WORD cpu_stack_pop_w(void);
DWORD cpu_stack_pop_dw(void);

void cpu_stack_push_b(BYTE value);
void cpu_stack_push_w(WORD value);
void cpu_stack_push_dw(DWORD value);