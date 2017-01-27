#include "../900L1.h"
#include "../register.h"
#include "../memory.h"

#include "flags.h"

/*
	Generator file for Binary operators
*/

// Validating arguments

#ifndef OPC
#	error OPC parmaeter required
#endif

#ifndef FUNCTION
#	error FUNCTION parameter required
#endif

#if defined ARITHMETIC == defined LOGICAL
#	error ARITHMETIC or LOGICAL has to be defined
#endif

#define CONCAT1(opc, suff) opc ## _ ## suff
#define CONCAT2(opc, suff) CONCAT1(opc, suff) // Delay token concatination
#define OPC_(suff) CONCAT2(OPC, suff)

static inline void OPC_(b) (BYTE* dst, BYTE a) {
	BYTE b = *dst;
	BYTE res;
	FUNCTION
#ifdef ARITHMETIC
	flags_ar_b(b, a, res);
#endif
#ifdef LOGICAL
	flags_log_b(res);
#endif
	*dst = res;
}

static inline void OPC_(w) (WORD* dst, WORD a) {
	WORD b = *dst;
	WORD res;
	FUNCTION
#ifdef ARITHMETIC
	flags_ar_w(b, a, res);
#endif
#ifdef LOGICAL
	flags_log_w(res);
#endif
	*dst = res;
}

static inline void OPC_(dw) (DWORD* dst, DWORD a) {
	DWORD b = *dst;
	DWORD res;
	FUNCTION
#ifdef ARITHMETIC
	flags_ar_dw(b, a, res);
#endif
#ifdef LOGICAL
	flags_log_dw(res);
#endif
	*dst = res;
}

// <OP> R, r
// R <- R <op> r
void OPC_(R_r) (BYTE f, enum OP_SIZE size, BYTE* reg, BYTE s) {
	switch (size) {
	case S_BYTE: OPC_(b)(cpu_getR_b(cpu_pull_op_b()), *reg); break;
	case S_WORD: OPC_(w)(cpu_getR_w(cpu_pull_op_b()), *to_w(reg)); break;
	case S_DWORD: OPC_(dw)(cpu_getR_dw(cpu_pull_op_b()), *to_dw(reg)); break;
	}
}

// <OP> r, #
// r <- r op #
void OPC_(r_$) (BYTE f, enum OP_SIZE size, BYTE* reg, BYTE s) {
	switch (size) {
	case S_BYTE: OPC_(b)(reg, cpu_pull_op_b()); break;
	case S_WORD: OPC_(w)(to_w(reg), cpu_pull_op_w()); break;
	case S_DWORD: OPC_(dw)(to_dw(reg), cpu_pull_op_dw()); break;
	}
}

// <OP> r, (mem)
// r <- r op (mem)
void OPC_(R_mem) (BYTE f, enum OP_SIZE size, DWORD addr, BYTE s) {
	switch (size) {
	case S_BYTE: OPC_(b)(cpu_getr_b(s), cpu_getmem_b(addr)); break;
	case S_WORD: OPC_(w)(cpu_getr_w(s), cpu_getmem_w(addr)); break;
	case S_DWORD: OPC_(dw)(cpu_getr_dw(s), cpu_getmem_dw(addr)); break;
	}
}

// <OP> (mem), r
// (mem) <- (mem) op r
void OPC_(mem_R) (BYTE f, enum OP_SIZE size, DWORD addr, BYTE s) {
	switch (size) {
	case S_BYTE: { BYTE b = cpu_getmem_b(addr); OPC_(b)(&b, *cpu_getr_b(s)); cpu_setmem_b(addr, b); break; }
	case S_WORD: { WORD w = cpu_getmem_w(addr); OPC_(w)(&w, *cpu_getr_w(s)); cpu_setmem_w(addr, w); break; }
	case S_DWORD: { DWORD dw = cpu_getmem_dw(addr); OPC_(dw)(&dw, *cpu_getr_dw(s)); cpu_setmem_dw(addr, dw); break; }
	}
}

// <OP><W> (mem), #
// (mem) <- (mem) op #
void OPC_(mem_$) (BYTE f, enum OP_SIZE size, DWORD addr, BYTE s) {
	switch (size) {
	case S_WORD: { WORD w = cpu_getmem_w(addr); OPC_(w)(&w, cpu_pull_op_w()); cpu_setmem_w(addr, w); }
	case S_BYTE: { BYTE b = cpu_getmem_b(addr); OPC_(b)(&b, cpu_pull_op_b()); cpu_setmem_b(addr, b); }
	}
}

// Cleanup
#undef CONCAT1
#undef CONCAT2
#undef OPC_
#undef OPC
#undef FUNCTION
