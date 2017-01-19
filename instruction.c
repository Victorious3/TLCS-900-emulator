#include "instruction.h"

#include "memory.h"
#include "register.h"
#include "memory.h"

// NOP, Skips a CPU cycle.
void NOP(BYTE f) {}

// HALT
void HALT(BYTE f) {

}

// PUSH SR, push the status register to the stack.
// Decrements xsp by 2, then places the 2 byte status register at the top of the stack.
// (-XSP) <- SR
void PUSH_SR(BYTE f) {
	cpu_stack_push_w(CPU_STATE.SR);
}

// PUSH A, push register A to the stack,
// (-XSP) <- A
void PUSH_A(BYTE f) {
	cpu_stack_push_b(*cpu_getR_b(0x1));
}

// PUSH F, push register F to the stack,
// (-XSP) <- F
void PUSH_F(BYTE f) {
	cpu_stack_push_b(CPU_STATE.F);
}

// PUSH r, push register to the stack
// (-XSP) <- r
void PUSH_r(BYTE f, enum OP_SIZE size, BYTE* reg, BYTE s) {
	switch (size) {
	case S_BYTE: cpu_stack_push_b(*reg); break;
	case S_WORD: cpu_stack_push_w(*(WORD*)reg); break;
	case S_DWORD: cpu_stack_push_dw(*(DWORD*)reg); break;
	}
}

// PUSH R, push current bank register (word) to the stack
// (-XSP) <- R
void PUSH_RR(BYTE f) {
	cpu_stack_push_w(*cpu_getR_w(f));
}

// PUSH R, push current bank register (dword) to the stack
// (-XSP) <- R
void PUSH_XRR(BYTE f) {
	cpu_stack_push_dw(*cpu_getR_dw(f));
}

// PUSH #, push a byte to the stack.
// (-XSP) <- #
void PUSH_n(BYTE f) {
	cpu_stack_push_b(cpu_pull_op_b());
}

// PUSHW #, push a word to the stack.
// (-XSP) <- #
void PUSHW_nn(BYTE f) {
	cpu_stack_push_w(cpu_pull_op_w());
}

// PUSH<W> mem, push data at a location in memory to the stack.
// (-XSP) <- (mem)
void PUSH_mem(BYTE f, enum OP_SIZE size, DWORD addr, BYTE s) {
	switch (size) {
	case S_BYTE: cpu_stack_push_b(cpu_getmem_b(addr)); break;
	case S_WORD: cpu_stack_push_w(cpu_getmem_w(addr)); break;
	}
}

// POP SR, pop the status register from the stack.
// SR <- (XSP+)
void POP_SR(BYTE f) {
	CPU_STATE.SR = cpu_stack_pop_w();
}

// POP A
// A <- (XSP+)
void POP_A(BYTE f) {
	*cpu_getR_b(0x1) = cpu_stack_pop_b();
}

// POP F
// F <- (XSP+)
void POP_F(BYTE f) {
	CPU_STATE.F = cpu_stack_pop_b();
}

// POP R
// R <- (XSP+)
void POP_RR(BYTE f) {
	*cpu_getR_w(f) = cpu_stack_pop_w();
}

// POP R
// R <- (XSP+)
void POP_XRR(BYTE f) {
	*cpu_getR_dw(f) = cpu_stack_pop_dw();
}

// POP r
// r <- (XSP+)
void POP_r(BYTE f, enum OP_SIZE size, BYTE* reg, BYTE s) {
	switch (size) {
	case S_BYTE: *reg = cpu_stack_pop_b(); break;
	case S_WORD: *(WORD*)reg = cpu_stack_pop_w(); break;
	case S_DWORD: *(DWORD*)reg = cpu_stack_pop_dw(); break;
	}
}

// POP mem
// (mem) <- (XSP+)
void POP_mem(BYTE f, DWORD addr, BYTE s) {
	cpu_setmem_b(addr, cpu_stack_pop_b());
}

// POPW mem
// (mem) <- (XSP+)
void POPW_mem(BYTE f, DWORD addr, BYTE s) {
	cpu_setmem_w(addr, cpu_stack_pop_w());
}

// LD<W> (#8), 8
// dst <- src
void LD_$n_n(BYTE f) {
	BYTE addr = cpu_pull_op_b();
	if (f & 0x2) { // word
		WORD data = cpu_pull_ob_w();
	} else { // byte
		BYTE data = cpu_pull_op_b();
	}
}

static void src(BYTE f) {
	BYTE b = (f >> 3) & 0x1E;
	enum OP_SIZE size = b == 0 ? 1 : b;
	DWORD addr = cpu_getaddr(f);
	BYTE s = cpu_pull_op_b();

	cpu_optable_src[s](f, size, addr, s);
}

static void dst(BYTE f) {
	DWORD addr = cpu_getaddr(f);
	BYTE s = cpu_pull_op_b();

	cpu_optable_dst[s](f, addr, s);
}

static void reg(BYTE f) {
	BYTE b = (f >> 3) & 0x1E;
	enum OP_SIZE size = b == 0 ? 1 : b;
	BYTE* reg = NULL;
	
	if ((f & 0x7) == 0x7) {
		// Extended register
		reg = cpu_getr_b(cpu_pull_op_b());
	} else {
		reg = cpu_getR_b(f);
	}

	BYTE s = cpu_pull_op_b();
	cpu_optable_reg[s](f, size, reg, s);
}

OPC* cpu_optable[0xFF] = {
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
	src, src, src, src, src, src, NULL, reg, reg, reg, reg, reg, reg, reg, reg, reg
};

OPC_REG* cpu_optable_reg[0xFF] = {
	NULL, NULL, NULL, LD_r_$, PUSH_r, POP_r, CPL_r, NEG_r, MUL_rr_$, MULS_rr_$, DIVS_rr_$, LINK_r_dd, UNLNK_r, BS1F_A_r, BS1B_A_r,
	DAA_r, NULL, EXTZ_r, EXTS_r, PAA_r, NULL, MIRR_r, NULL, NULL, MULA_r, NULL, NULL, DJNZ_r_d, NULL, NULL, NULL,
	ANDCF_$_r, ORCF_$_r, XORCF_$_r, LDCF_$_r, STCF_$_r, NULL, NULL, NULL, ANDCF_A_r, ORCF_A_r, XORCF_A_r, LDCF_A_r, STCF_A_r, NULL, LDC_cr_r, LDC_r_cr,
	RES_$_r, SET_$_r, CHG_$_r, BIT_$_r, TSET_$_r, NULL, NULL, NULL, MINC1_$_r, MINC2_$_r, MINC4_$_r, NULL, MDEC1_$_r, MDEC2_$_r, MDEC4_$_r, NULL,
	MUL_R_r, MUL_R_r, MUL_R_r, MUL_R_r, MUL_R_r, MUL_R_r, MUL_R_r, MUL_R_r, MULS_R_r, MULS_R_r, MULS_R_r, MULS_R_r, MULS_R_r, MULS_R_r, MULS_R_r, MULS_R_r,
	DIV_R_r, DIV_R_r, DIV_R_r, DIV_R_r, DIV_R_r, DIV_R_r, DIV_R_r, DIV_R_r, DIVS_R_r, DIVS_R_r, DIVS_R_r, DIVS_R_r, DIVS_R_r, DIVS_R_r, DIVS_R_r, DIVS_R_r,
	INC_$3_r, INC_$3_r, INC_$3_r, INC_$3_r, INC_$3_r, INC_$3_r, INC_$3_r, INC_$3_r, DEC_$3_r, DEC_$3_r, DEC_$3_r, DEC_$3_r, DEC_$3_r, DEC_$3_r, DEC_$3_r, DEC_$3_r,
	SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r, SCC_cc_r,
	ADD_R_r, ADD_R_r, ADD_R_r, ADD_R_r, ADD_R_r, ADD_R_r, ADD_R_r, ADD_R_r, LD_R_r, LD_R_r, LD_R_r, LD_R_r, LD_R_r, LD_R_r, LD_R_r, LD_R_r,
	ADC_R_r, ADC_R_r, ADC_R_r, ADC_R_r, ADC_R_r, ADC_R_r, ADC_R_r, ADC_R_r, LD_r_R, LD_r_R, LD_r_R, LD_r_R, LD_r_R, LD_r_R, LD_r_R, LD_r_R,
	SUB_R_r, SUB_R_r, SUB_R_r, SUB_R_r, SUB_R_r, SUB_R_r, SUB_R_r, SUB_R_r, LD_r_$3, LD_r_$3, LD_r_$3, LD_r_$3, LD_r_$3, LD_r_$3, LD_r_$3, LD_r_$3,
	SBC_R_r, SBC_R_r, SBC_R_r, SBC_R_r, SBC_R_r, SBC_R_r, SBC_R_r, SBC_R_r, EX_R_r, EX_R_r, EX_R_r, EX_R_r, EX_R_r, EX_R_r, EX_R_r, EX_R_r,
	AND_R_r, AND_R_r, AND_R_r, AND_R_r, AND_R_r, AND_R_r, AND_R_r, AND_R_r, ADD_r_$, ADC_r_$, SUB_r_$, SBC_r_$, AND_r_$, XOR_r_$, OR_r_$, CP_r_$,
	XOR_R_r, XOR_R_r, XOR_R_r, XOR_R_r, XOR_R_r, XOR_R_r, XOR_R_r, XOR_R_r, CP_r_$3, CP_r_$3, CP_r_$3, CP_r_$3, CP_r_$3, CP_r_$3, CP_r_$3, CP_r_$3,
	OR_R_r, OR_R_r, OR_R_r, OR_R_r, OR_R_r, OR_R_r, OR_R_r, OR_R_r, RLC_$_r, RRC_$_r, RL_$_r, RR_$_r, SLA_$_r, SRA_$_r, SLL_$_r, SRL_$_r,
	CP_R_r, CP_R_r, CP_R_r, CP_R_r, CP_R_r, CP_R_r, CP_R_r, CP_R_r, RLC_A_r, RRC_A_r, RL_A_r, RR_A_r, SLA_A_r, SRA_A_r, SLL_A_r, SRL_A_r,
};
