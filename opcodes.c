#include "opcodes.h"

#include "900L1.h"
#include "memory.h"
#include "register.h"
#include "memory.h"
#include "insn/flags.h"

// Size independent read/write

static inline DWORD read_mem(DWORD addr, enum OP_SIZE size) {
	switch (size) {
	case S_BYTE: return cpu_getmem_b(addr);
	case S_WORD: return cpu_getmem_w(addr);
	case S_DWORD: return cpu_getmem_dw(addr);
	}
}

static inline void write_mem(DWORD addr, enum OP_SIZE size, DWORD value) {
	switch (size) {
	case S_BYTE: cpu_setmem_b(addr, value); break;
	case S_WORD: cpu_setmem_w(addr, value); break;
	case S_DWORD: cpu_setmem_dw(addr, value); break;
	}
}

static inline DWORD read_reg(BYTE* reg, enum OP_SIZE size) {
	switch (size) {
	case S_BYTE: return getr_b(reg);
	case S_WORD: return getr_w(reg);
	case S_DWORD: return getr_dw(reg);
	}
}

static inline void write_reg(BYTE* reg, enum OP_SIZE size, DWORD value) {
	switch (size) {
	case S_BYTE: setr_b(reg, value); break;
	case S_WORD: setr_w(reg, value); break;
	case S_DWORD: setr_dw(reg, value); break;
	}
}

void cpu_exec_insn() {
	CPU_STATE.opc_1 = cpu_pull_op_b();
	cpu_optable[CPU_STATE.opc_1]();
}

static void src() {
	BYTE b = (CPU_STATE.opc_1 >> 3) & 0x1E; // Decode size of the operands
	CPU_STATE.size = b == 0 ? 1 : b;
	CPU_STATE.addr = cpu_getaddr(CPU_STATE.opc_1);
	CPU_STATE.opc_2 = cpu_pull_op_b();

	//cpu_optable_src[CPU_STATE.opc_2]();
}

static void dst() {
	DWORD addr = cpu_getaddr(CPU_STATE.opc_1);
	CPU_STATE.opc_2 = cpu_pull_op_b();

	//cpu_optable_dst[CPU_STATE.opc_2]();
}

static void reg() {
	BYTE b = (CPU_STATE.opc_1 >> 3) & 0x1E; // Decode size of the operands
	CPU_STATE.size = b == 0 ? 1 : b;

	if ((CPU_STATE.opc_1 & 0x7) == 0x7) {
		// Extended register
		CPU_STATE.reg = cpu_getr(cpu_pull_op_b());
	} else {
		CPU_STATE.reg = cpu_getR(CPU_STATE.opc_1);
	}

	CPU_STATE.opc_2 = cpu_pull_op_b();
	///cpu_optable_reg[CPU_STATE.opc_2]();
}

// Argument specifier code
// [read r/mem][read R/imm]<function>[write R/r][write mem]
//

// Where the first argument is rerouted to
enum IN { // 1 bit
	I_A, I_B = 1 << 32
};

enum READ { // 3 bit
	R_r = 1 << 29, R_mem = 2 << 29, R_cr = 3 << 29
};

enum READ2 { // 4 bit
	R_A = 1 << 23, R_R = 2 << 23, R_imm3 = 3 << 23, R_imm4 = 4 << 23, R_imm = 5 << 23, R_cc = 6 << 23
};

enum WRITE { // 3 bit
	W_mem = 1 << 20, W_r = 2 << 20, W_cr = 3 << 20
};

enum WRITE2 { // 3 bit
	W_A = 1 << 17, W_R = 2 << 17
};

enum SIZE { // 2 bit
	S_b = 1 << 27, S_w = 2 << 27, S_d = 3 << 27
};

enum FUNC { // 16 bit
	F_LD = 1, F_PUSH, F_POP, F_NEG, F_AND, F_ANDCF, F_INC, F_DEC, F_OR, F_ORCF, F_XOR, F_XORCF,
	F_EX, F_ADD, F_ADC, F_CP, F_SUB, F_SBC, F_MUL, F_MULS, F_DIV, F_DIVS,
	F_RR, F_RL, F_RLC, F_RRC, F_SLA, F_SRA, F_SLL, F_SRL,
	F_LINK, F_UNLK, F_BS1F, F_BS1B, F_DAA, F_EXTZ, F_EXTS, F_PAA, F_MIRR, F_MULA,
	F_DJNZ, F_LDCF, F_STCF, F_LDC, F_RES, F_SET, F_CHG, F_BIT, F_TSET, 
	F_MINC1, F_MDEC1, F_MINC2, F_MDEC2, F_MINC4, F_MDEC4,
	F_SCC, F_LDI, F_LDIR, F_LDD, F_LDDR, F_CPI, F_CPL, F_CPIR, F_CPD, F_CPDR, F_RLD, F_RRD,
	F_JP, F_CALL, F_RET
};

OPC *const cpu_optable[0x100] = {
	NOP, NULL, PUSH_SR, POP_SR, NULL, HALT, EI_n, RETI, LD_$n_n, PUSH_n, LDW_$n_nn, PUSHW_nn, INCF, DECF, RET, RETD_dd,
	RCF, SCF, CCF, ZCF, PUSH_A, POP_A, EX_F_F$, LDF_n, PUSH_F, POP_F, JP_nn, JP_nnn, CALL_nn, CALL_nnn, CALR, NULL,
	LD_R_n, LD_R_n, LD_R_n, LD_R_n, LD_R_n, LD_R_n, LD_R_n, LD_R_n, PUSH_RR, PUSH_RR, PUSH_RR, PUSH_RR, PUSH_RR, PUSH_RR, PUSH_RR, PUSH_RR,
	LD_RR_nn, LD_RR_nn, LD_RR_nn, LD_RR_nn, LD_RR_nn, LD_RR_nn, LD_RR_nn, LD_RR_nn, PUSH_XRR, PUSH_XRR, PUSH_XRR, PUSH_XRR, PUSH_XRR, PUSH_XRR, PUSH_XRR, PUSH_XRR,
	LD_XRR_nnnn, LD_XRR_nnnn, LD_XRR_nnnn, LD_XRR_nnnn, LD_XRR_nnnn, LD_XRR_nnnn, LD_XRR_nnnn, LD_XRR_nnnn, POP_RR, POP_RR, POP_RR, POP_RR, POP_RR, POP_RR, POP_RR, POP_RR,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, POP_XRR, POP_XRR, POP_XRR, POP_XRR, POP_XRR, POP_XRR, POP_XRR, POP_XRR,
	JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d, JR_d,
	JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd, JRL_dd,
	src, src, src, src, src, src, src, src, src, src, src, src, src, src, src, src,
	src, src, src, src, src, src, src, src, src, src, src, src, src, src, src, src,
	src, src, src, src, src, src, src, src, src, src, src, src, src, src, src, src,
	dst, dst, dst, dst, dst, dst, dst, dst, dst, dst, dst, dst, dst, dst, dst, dst,
	src, src, src, src, src, src, NULL, reg, reg, reg, reg, reg, reg, reg, reg, reg,
	src, src, src, src, src, src, NULL, reg, reg, reg, reg, reg, reg, reg, reg, reg,
	src, src, src, src, src, src, NULL, reg, reg, reg, reg, reg, reg, reg, reg, reg,
	dst, dst, dst, dst, dst, dst, NULL, LDX, SWI, SWI, SWI, SWI, SWI, SWI, SWI, SWI
};

// Repeat 8 times
#define ROW($v) ($v), ($v), ($v), ($v), ($v), ($v), ($v), ($v)
#define ROW_V($o0, $o1, $o2, $o3, $o4, $o5, $o6, $o7, $arg) ($o0) | ($arg), ($o1) | ($arg), ($o2) | ($arg), ($o3) | ($arg), ($o4) | ($arg), ($o5) | ($arg), ($o6) | ($arg), ($o7) | ($arg)

WORD const cpu_optable_reg[0x100] = {
[0x00] = 0, 0, 0,
	F_LD | R_imm | W_r,
	F_PUSH,
	F_POP | W_r,
	F_CPL | R_r | W_r,
	F_NEG | R_r | W_r,
	F_MUL | R_r | R_imm | W_r,
	F_MULS | R_r | R_imm | W_r,
	F_DIV | R_r | R_imm | W_r,
	F_DIVS | R_r | R_imm | W_r,
	F_LINK | R_r | R_imm | W_r | S_w,
	F_UNLK | R_r | W_r | S_w,
	F_BS1F | R_r | W_A,
	F_BS1B | R_r | W_A,
[0x10] =
	F_DAA | R_r | W_r, 0,
	F_EXTZ | W_r,
	F_EXTS | R_r | W_r,
	F_PAA | R_r | W_r, 0,
	F_MIRR | R_r | W_r | S_w, 0, 0,
	F_MULA | R_r | W_r | S_w, 0, 0,
	F_DJNZ | R_imm | S_b, 0, 0, 0,
[0x20] =
	F_ANDCF | R_r | R_imm4,
	F_ORCF | R_r | R_imm4,
	F_XORCF | R_r | R_imm4,
	F_LDCF | R_r | R_imm4,
	F_STCF | R_r | R_imm4 | W_r, 0, 0, 0,
	F_ANDCF | R_r | R_A,
	F_ORCF | R_r | R_A,
	F_XORCF | R_r | R_A,
	F_LDCF | R_r | R_A,
	F_STCF | R_r | R_A | W_r, 0,
	F_LDC | R_r | W_cr,
	F_LDC | R_cr | W_r,
[0x30] =
	F_RES | R_imm4 | W_r,
	F_SET | R_imm4 | W_r,
	F_CHG | R_r | R_imm4 | W_r,
	F_BIT | R_r | R_imm4,
	F_TSET | R_imm4 | W_r, 0, 0, 0,
	F_MINC1 | R_r | W_r,
	F_MINC2 | R_r | W_r,
	F_MINC4 | R_r | W_r, 0,
	F_MDEC1 | R_r | W_r,
	F_MDEC2 | R_r | W_r,
	F_MDEC4 | R_r | W_r, 0,
[0x40] =
	ROW(F_MUL | R_r | R_R | W_R),
	ROW(F_MULS | R_r | R_R | W_R),
[0x50] =
	ROW(F_DIV | R_r | R_R | W_R),
	ROW(F_DIVS | R_r | R_R | W_R),
[0x60] =
	ROW(F_INC | R_r | R_imm3 | W_r),
	ROW(F_DEC | R_r | R_imm3 | W_r),
[0x70] =
	ROW(F_SCC | R_cc | W_r),
	ROW(F_SCC | R_cc | W_r),
[0x80] =
	ROW(F_ADD | R_r | R_R | W_R),
	ROW(F_LD | R_r | W_R),
[0x90] =
	ROW(F_ADC | R_r | R_R | W_R),
	ROW(F_LD | R_R | W_r | I_B),
[0xA0] =
	ROW(F_SUB | R_r | R_R | W_R),
	ROW(F_LD | R_imm3 | W_R | I_B),
[0xB0] =
	ROW(F_SBC | R_r | R_R | W_R),
	ROW(F_EX | R_r | R_R | W_r | W_R),
[0xC0] =
	ROW(F_AND | R_r | R_R | W_R),
	ROW_V(F_ADD, F_ADC, F_SUB, F_SBC, F_AND, F_XOR, F_OR, F_CP, (R_r | R_imm | W_r)),
[0xD0] = 
	ROW(F_XOR | R_r | R_R | W_R),
	ROW(F_CP | R_r | R_imm3 | W_r),
[0xE0] =
	ROW(F_OR | R_r | R_R | W_R),
	ROW_V(F_RLC, F_RRC, F_RL, F_RR, F_SLA, F_SRA, F_SLL, F_SRL, (R_r | R_imm3 | W_r)),
[0xF0] =
	ROW(F_CP | R_r | R_R | W_R),
	ROW_V(F_RLC, F_RRC, F_RL, F_RR, F_SLA, F_SRA, F_SLL, F_SRL, (R_r | R_A | W_r)),
};


WORD const cpu_optable_src[0x100] = {
[0x00] = 0, 0, 0, 0,
	F_PUSH | R_mem, 0,
	F_RLD | R_A | R_mem | W_A | W_mem,
	F_RRD | R_A | R_mem | W_A | W_mem,
	ROW(0),
	[0x10] =
	F_LDI,
	F_LDIR,
	F_LDD,
	F_LDDR,
	F_CPI,
	F_CPD,
	F_CPDR, 0,
	F_LD | R_mem | R_imm | W_mem | S_d,



	LDI, LDIR, LDD, LDDR, CPI, CPIR, CPD, CPDR, NULL, LD_nn_m, NULL, NULL, NULL, NULL, NULL, NULL,
	LD_R_mem, LD_R_mem, LD_R_mem, LD_R_mem, LD_R_mem, LD_R_mem, LD_R_mem, LD_R_mem, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	EX_mem_R, EX_mem_R, EX_mem_R, EX_mem_R, EX_mem_R, EX_mem_R, EX_mem_R, EX_mem_R, ADD_mem_$, ADC_mem_$, SUB_mem_$, SBC_mem_$, AND_mem_$, XOR_mem_$, OR_mem_$, CP_mem_$,
	MUL_R_mem, MUL_R_mem, MUL_R_mem, MUL_R_mem, MUL_R_mem, MUL_R_mem, MUL_R_mem, MUL_R_mem, MULS_R_mem, MULS_R_mem, MULS_R_mem, MULS_R_mem, MULS_R_mem, MULS_R_mem, MULS_R_mem, MULS_R_mem,
	DIV_R_mem, DIV_R_mem, DIV_R_mem, DIV_R_mem, DIV_R_mem, DIV_R_mem, DIV_R_mem, DIV_R_mem, DIVS_R_mem, DIVS_R_mem, DIVS_R_mem, DIVS_R_mem, DIVS_R_mem, DIVS_R_mem, DIVS_R_mem, DIVS_R_mem,
	INC_$3_mem, INC_$3_mem, INC_$3_mem, INC_$3_mem, INC_$3_mem, INC_$3_mem, INC_$3_mem, INC_$3_mem, DEC_$3_mem, DEC_$3_mem, DEC_$3_mem, DEC_$3_mem, DEC_$3_mem, DEC_$3_mem, DEC_$3_mem, DEC_$3_mem,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, RLC_mem, RRC_mem, RL_mem, RR_mem, SLA_mem, SRA_mem, SLL_mem, SRL_mem,
	ADD_R_mem, ADD_R_mem, ADD_R_mem, ADD_R_mem, ADD_R_mem, ADD_R_mem, ADD_R_mem, ADD_R_mem, ADD_mem_R, ADD_mem_R, ADD_mem_R, ADD_mem_R, ADD_mem_R, ADD_mem_R, ADD_mem_R, ADD_mem_R,
	ADC_R_mem, ADC_R_mem, ADC_R_mem, ADC_R_mem, ADC_R_mem, ADC_R_mem, ADC_R_mem, ADC_R_mem, ADC_mem_R, ADC_mem_R, ADC_mem_R, ADC_mem_R, ADC_mem_R, ADC_mem_R, ADC_mem_R, ADC_mem_R,
	SUB_R_mem, SUB_R_mem, SUB_R_mem, SUB_R_mem, SUB_R_mem, SUB_R_mem, SUB_R_mem, SUB_R_mem, SUB_mem_R, SUB_mem_R, SUB_mem_R, SUB_mem_R, SUB_mem_R, SUB_mem_R, SUB_mem_R, SUB_mem_R,
	SBC_R_mem, SBC_R_mem, SBC_R_mem, SBC_R_mem, SBC_R_mem, SBC_R_mem, SBC_R_mem, SBC_R_mem, SBC_mem_R, SBC_mem_R, SBC_mem_R, SBC_mem_R, SBC_mem_R, SBC_mem_R, SBC_mem_R, SBC_mem_R,
	AND_R_mem, AND_R_mem, AND_R_mem, AND_R_mem, AND_R_mem, AND_R_mem, AND_R_mem, AND_R_mem, AND_mem_R, AND_mem_R, AND_mem_R, AND_mem_R, AND_mem_R, AND_mem_R, AND_mem_R, AND_mem_R,
	XOR_R_mem, XOR_R_mem, XOR_R_mem, XOR_R_mem, XOR_R_mem, XOR_R_mem, XOR_R_mem, XOR_R_mem, XOR_mem_R, XOR_mem_R, XOR_mem_R, XOR_mem_R, XOR_mem_R, XOR_mem_R, XOR_mem_R, XOR_mem_R,
	OR_R_mem, OR_R_mem, OR_R_mem, OR_R_mem, OR_R_mem, OR_R_mem, OR_R_mem, OR_R_mem, OR_mem_R, OR_mem_R, OR_mem_R, OR_mem_R, OR_mem_R, OR_mem_R, OR_mem_R, OR_mem_R,
	CP_R_mem, CP_R_mem, CP_R_mem, CP_R_mem, CP_R_mem, CP_R_mem, CP_R_mem, CP_R_mem, CP_mem_R, CP_mem_R, CP_mem_R, CP_mem_R, CP_mem_R, CP_mem_R, CP_mem_R, CP_mem_R
};

/*
WORD const cpu_optable_dst[0x100] = {
	LD_m_$, NULL, LD_m_$, NULL, POP_mem, NULL, POP_mem, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, LD_m_nn, NULL, LD_m_nn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, ANDCF_A_mem, ORCF_A_mem, XORCF_A_mem, LDCF_A_mem, STCF_A_mem, NULL, NULL, NULL,
	LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, LDA_R_mem, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, LD_mem_R, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	ANDCF_$3_mem, ANDCF_$3_mem, ANDCF_$3_mem, ANDCF_$3_mem, ANDCF_$3_mem, ANDCF_$3_mem, ANDCF_$3_mem, ANDCF_$3_mem, ORCF_$3_mem, ORCF_$3_mem, ORCF_$3_mem, ORCF_$3_mem, ORCF_$3_mem, ORCF_$3_mem, ORCF_$3_mem, ORCF_$3_mem,
	XORCF_$3_mem, XORCF_$3_mem, XORCF_$3_mem, XORCF_$3_mem, XORCF_$3_mem, XORCF_$3_mem, XORCF_$3_mem, XORCF_$3_mem, LDCF_$3_mem, LDCF_$3_mem, LDCF_$3_mem, LDCF_$3_mem, LDCF_$3_mem, LDCF_$3_mem, LDCF_$3_mem, LDCF_$3_mem,
	STCF_$3_mem, STCF_$3_mem, STCF_$3_mem, STCF_$3_mem, STCF_$3_mem, STCF_$3_mem, STCF_$3_mem, STCF_$3_mem, TSET_$3_mem, TSET_$3_mem, TSET_$3_mem, TSET_$3_mem, TSET_$3_mem, TSET_$3_mem, TSET_$3_mem, TSET_$3_mem,
	RES_$3_mem, RES_$3_mem, RES_$3_mem, RES_$3_mem, RES_$3_mem, RES_$3_mem, RES_$3_mem, RES_$3_mem, SET_$3_mem, SET_$3_mem, SET_$3_mem, SET_$3_mem, SET_$3_mem, SET_$3_mem, SET_$3_mem, SET_$3_mem,
	CHG_$3_mem, CHG_$3_mem, CHG_$3_mem, CHG_$3_mem, CHG_$3_mem, CHG_$3_mem, CHG_$3_mem, CHG_$3_mem, BIT_$3_mem, BIT_$3_mem, BIT_$3_mem, BIT_$3_mem, BIT_$3_mem, BIT_$3_mem, BIT_$3_mem, BIT_$3_mem,
	JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem, JP_cc_mem,
	CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem, CALL_cc_mem,
	RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc, RET_cc
};
*/