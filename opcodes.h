#pragma once

#include "900L1.h"

typedef void OPC(void);

void cpu_exec_insn(void);

// 1 byte opcodes
// (NORMAL and MAX are not implemented on the 900L)

OPC NOP, PUSH_SR, POP_SR, HALT, EI_n, RETI, LD_$n_n, PUSH_n, LDW_$n_nn, PUSHW_nn, INCF, DECF, 
	RET, RETD_dd,RCF, SCF, CCF, ZCF, PUSH_A, POP_A, EX_F_F$, LDF_n,PUSH_F, POP_F,JP_nn, JP_nnn,
	CALL_nn, CALL_nnn, CALR, LD_R_n, PUSH_RR, LD_RR_nn, PUSH_XRR, LD_XRR_nnnn, POP_RR, POP_XRR, 
	JR_d, JRL_dd, LDX, SWI;

// reg
OPC LD_r_$, PUSH_r, POP_r, CPL_r, NEG_r, MUL_rr_$, MULS_rr_$, DIV_rr_$, DIVS_rr_$, LINK_r_dd,
	UNLNK_r, BS1F_A_r, BS1B_A_r, DAA_r, EXTZ_r, EXTS_r, PAA_r, MIRR_r, MULA_r, DJNZ_r_d, ANDCF_$_r,
	ORCF_$_r, XORCF_$_r, LDCF_$_r, STCF_$_r, ANDCF_A_r, ORCF_A_r, XORCF_A_r, LDCF_A_r, STCF_A_r,
	LDC_cr_r, LDC_r_cr, RES_$_r, SET_$_r, CHG_$_r, BIT_$_r, TSET_$_r, MINC1_$_r, MINC2_$_r,
	MINC4_$_r, MDEC1_$_r, MDEC2_$_r, MDEC4_$_r, MUL_R_r, MULS_R_r, DIV_R_r, DIVS_R_r, INC_$3_r,
	DEC_$3_r, SCC_cc_r, ADD_R_r, LD_R_r, ADC_R_r, LD_r_R, SUB_R_r, LD_r_$3, SBC_R_r, EX_R_r, AND_R_r,
	ADD_r_$, ADC_r_$, SUB_r_$, SBC_r_$, AND_r_$, XOR_r_$, OR_r_$, CP_r_$, XOR_R_r, CP_r_$3, OR_R_r,
	RLC_$_r, RRC_$_r, RL_$_r, RR_$_r, SLA_$_r, SRA_$_r, SLL_$_r, SRL_$_r, CP_R_r, RLC_A_r, RRC_A_r,
	RL_A_r, RR_A_r, SLA_A_r, SRA_A_r, SLL_A_r, SRL_A_r;

// src
OPC PUSH_mem, RLD_A_mem, RRD_A_mem, LDI, LDIR, LDD, LDDR, CPI, CPIR, CPD, CPDR, LD_nn_m,
	LD_R_mem, EX_mem_R, ADD_mem_$, ADC_mem_$, SUB_mem_$, SBC_mem_$, AND_mem_$, XOR_mem_$, OR_mem_$,
	CP_mem_$, MUL_R_mem, MULS_R_mem, DIV_R_mem, DIVS_R_mem, INC_$3_mem, DEC_$3_mem, RLC_mem, RRC_mem,
	RL_mem, RR_mem, SLA_mem, SRA_mem, SLL_mem, SRL_mem, ADD_R_mem, ADD_mem_R, ADC_R_mem, ADC_mem_R,
	SUB_R_mem, SUB_mem_R, SBC_R_mem, SBC_mem_R, AND_R_mem, AND_mem_R, XOR_R_mem, XOR_mem_R, OR_R_mem,
	OR_mem_R, CP_R_mem, CP_mem_R; 

// dst
OPC LD_m_$, LDW_m_$, POP_mem, POPW_mem, LD_m_nn, LDW_m_nn, LDA_R_mem, ANDCF_A_mem, ORCF_A_mem, 
	XORCF_A_mem, LDCF_A_mem, STCF_A_mem, LD_mem_R, ANDCF_$3_mem, ORCF_$3_mem, XORCF_$3_mem, LDCF_$3_mem,
	STCF_$3_mem, TSET_$3_mem, RES_$3_mem, SET_$3_mem, CHG_$3_mem, BIT_$3_mem, JP_cc_mem, CALL_cc_mem, RET_cc;

// Operand tables
extern OPC *const cpu_optable[0x100];
extern DWORD const cpu_optable_reg[0x100];
extern DWORD const cpu_optable_src[0x100];
extern DWORD const cpu_optable_dst[0x100];
