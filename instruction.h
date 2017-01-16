#pragma once

#include "900L1.h"

// 1 byte opcodes
void NOP(BYTE f);
// void NORMAL(BYTE f); Not implemented on 900L1
void PUSH_SR(BYTE f);
void POP_SR(BYTE f);
// void MAX(BYTE f); Not implemented on 900L1
void HALT(BYTE f);
void EI_n(BYTE f);
void RETI(BYTE f);
void LD_$n_n(BYTE f);
void PUSH_n(BYTE f);
void LDW_$n_nn(BYTE f);
void PUSHW_nn(BYTE f);
void INCF(BYTE f);
void DECF(BYTE f);
void RET(BYTE f);
void RETD_dd(BYTE f);

void RCF(BYTE f);
void SCF(BYTE f);
void CCF(BYTE f);
void ZCF(BYTE f);
void PUSH_A(BYTE f);
void POP_A(BYTE f);
void EX_F_F$(BYTE f);
void LDF_n(BYTE f);
void PUSH_F(BYTE f);
void POP_F(BYTE f);
void JP_nn(BYTE f);
void JP_nnn(BYTE f);
void CALL_nn(BYTE f);
void CALL_nnn(BYTE f);
void CALR(BYTE f);

void LD_R_n(BYTE f);
void PUSH_RR(BYTE f);

void LD_RR_nn(BYTE f);
void PUSH_XRR(BYTE f);

void LD_XRR_nnnn(BYTE f);
void POP_RR(BYTE f);

void POP_XRR(BYTE f);

void JR_d(BYTE f);

void JRL_dd(BYTE f);

// reg
void LD_r_$(BYTE f, enum OP_SIZE size, BYTE reg);
void PUSH_r(BYTE f, enum OP_SIZE size, BYTE reg);
void POP_r(BYTE f, enum OP_SIZE size, BYTE reg);

void CPL_r(BYTE f, enum OP_SIZE size, BYTE reg);
void NEG_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MUL_rr_$(BYTE f, enum OP_SIZE size, BYTE reg);
void MULS_rr_$(BYTE f, enum OP_SIZE size, BYTE reg);
void DIV_rr_$(BYTE f, enum OP_SIZE size, BYTE reg);
void DIVS_rr_$(BYTE f, enum OP_SIZE size, BYTE reg);
void LINK_r_dd(BYTE f, enum OP_SIZE size, BYTE reg);
void UNLNK_r(BYTE f, enum OP_SIZE size, BYTE reg);
void BS1F_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void BS1B_A_r(BYTE f, enum OP_SIZE size, BYTE reg);

void DAA_r(BYTE f, enum OP_SIZE size, BYTE reg);
void EXTZ_r(BYTE f, enum OP_SIZE size, BYTE reg);
void EXTS_r(BYTE f, enum OP_SIZE size, BYTE reg);
void PAA_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MIRR_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MULA_r(BYTE f, enum OP_SIZE size, BYTE reg);
void DJNZ_r_d(BYTE f, enum OP_SIZE size, BYTE reg);

void ANDCF_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void ORCF_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void XORCF_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void LDCF_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void STCF_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void ANDCF_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void ORCF_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void XORCF_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void LDCF_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void STCF_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void LDC_cr_r(BYTE f, enum OP_SIZE size, BYTE reg);
void LDC_r_cr(BYTE f, enum OP_SIZE size, BYTE reg);

void RES_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void SET_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void CHG_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void BIT_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void TSET_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MINC1_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MINC2_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MINC4_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MDEC1_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MDEC2_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MDEC4_$_r(BYTE f, enum OP_SIZE size, BYTE reg);

void MUL_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void MULS_R_r(BYTE f, enum OP_SIZE size, BYTE reg);

void DIV_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void DIVS_R_r(BYTE f, enum OP_SIZE size, BYTE reg);

void INC_$3_r(BYTE f, enum OP_SIZE size, BYTE reg);
void DEC_$3_r(BYTE f, enum OP_SIZE size, BYTE reg);

void SCC_cc_r(BYTE f, enum OP_SIZE size, BYTE reg);

void ADD_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void LD_R_r(BYTE f, enum OP_SIZE size, BYTE reg);

void ADC_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void LD_r_R(BYTE f, enum OP_SIZE size, BYTE reg);

void SUB_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void LD_r_$3(BYTE f, enum OP_SIZE size, BYTE reg);

void SBC_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void EX_R_r(BYTE f, enum OP_SIZE size, BYTE reg);

void AND_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void ADD_r_$(BYTE f, enum OP_SIZE size, BYTE reg);
void ADC_r_$(BYTE f, enum OP_SIZE size, BYTE reg);
void SUB_r_$(BYTE f, enum OP_SIZE size, BYTE reg);
void SBC_r_$(BYTE f, enum OP_SIZE size, BYTE reg);
void AND_r_$(BYTE f, enum OP_SIZE size, BYTE reg);
void XOR_r_$(BYTE f, enum OP_SIZE size, BYTE reg);
void OR_r_$(BYTE f, enum OP_SIZE size, BYTE reg);
void CP_r_$(BYTE f, enum OP_SIZE size, BYTE reg);

void XOR_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void CP_r_$3(BYTE f, enum OP_SIZE size, BYTE reg);

void OR_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void RLC_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void RRC_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void RL_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void RR_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void SLA_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void SRA_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void SLL_$_r(BYTE f, enum OP_SIZE size, BYTE reg);
void SRL_$_r(BYTE f, enum OP_SIZE size, BYTE reg);

void CP_R_r(BYTE f, enum OP_SIZE size, BYTE reg);
void RLC_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void RRC_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void RL_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void RR_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void SLA_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void SRA_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void SLL_A_r(BYTE f, enum OP_SIZE size, BYTE reg);
void SRL_A_r(BYTE f, enum OP_SIZE size, BYTE reg);

// src
void PUSH_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void RLD_A_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void RRD_A_mem(BYTE f, enum OP_SIZE size, DWORD addr);

void LDI(BYTE f, enum OP_SIZE size, DWORD addr);
void LDIR(BYTE f, enum OP_SIZE size, DWORD addr);
void LDD(BYTE f, enum OP_SIZE size, DWORD addr);
void LDDR(BYTE f, enum OP_SIZE size, DWORD addr);
void CPI(BYTE f, enum OP_SIZE size, DWORD addr);
void CPIR(BYTE f, enum OP_SIZE size, DWORD addr);
void CPD(BYTE f, enum OP_SIZE size, DWORD addr);
void CPDR(BYTE f, enum OP_SIZE size, DWORD addr);
void LD_nn_m(BYTE f, enum OP_SIZE size, DWORD addr);

void LD_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);

void EX_mem_R(BYTE f, enum OP_SIZE size, DWORD addr);
void ADD_mem_$(BYTE f, enum OP_SIZE size, DWORD addr);
void ADC_mem_$(BYTE f, enum OP_SIZE size, DWORD addr);
void SUB_mem_$(BYTE f, enum OP_SIZE size, DWORD addr);
void SBC_mem_$(BYTE f, enum OP_SIZE size, DWORD addr);
void AND_mem_$(BYTE f, enum OP_SIZE size, DWORD addr);
void XOR_mem_$(BYTE f, enum OP_SIZE size, DWORD addr);
void OR_mem_$(BYTE f, enum OP_SIZE size, DWORD addr);
void CP_mem_$(BYTE f, enum OP_SIZE size, DWORD addr);

void MUL_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void MULS_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);

void DIV_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void DIVS_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);

void INC_$3_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void DEC_$3_mem(BYTE f, enum OP_SIZE size, DWORD addr);

void RLC_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void RRC_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void RL_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void RR_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void SLA_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void SRA_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void SLL_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void SRL_mem(BYTE f, enum OP_SIZE size, DWORD addr);

void ADD_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void ADD_mem_R(BYTE f, enum OP_SIZE size, DWORD addr);

void ADC_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void ADC_mem_R(BYTE f, enum OP_SIZE size, DWORD addr);

void SUB_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void SUB_mem_R(BYTE f, enum OP_SIZE size, DWORD addr);

void SBC_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void SBC_mem_R(BYTE f, enum OP_SIZE size, DWORD addr);

void AND_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void AND_mem_R(BYTE f, enum OP_SIZE size, DWORD addr);

void XOR_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void XOR_mem_R(BYTE f, enum OP_SIZE size, DWORD addr);

void OR_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void OR_mem_R(BYTE f, enum OP_SIZE size, DWORD addr);

void CP_R_mem(BYTE f, enum OP_SIZE size, DWORD addr);
void CP_mem_R(BYTE f, enum OP_SIZE size, DWORD addr);

// dst
void LD_m_$(BYTE f, DWORD addr);
void LDW_m_$(BYTE f, DWORD addr);
void POP_mem(BYTE f, DWORD addr);
void POPW_mem(BYTE f, DWORD addr);

void LD_m_nn(BYTE f, DWORD addr);
void LDW_m_nn(BYTE f, DWORD addr);

void LDA_R_mem(BYTE f, DWORD addr);
void ANDCF_A_mem(BYTE f, DWORD addr);
void ORCF_A_mem(BYTE f, DWORD addr);
void XORCF_A_mem(BYTE f, DWORD addr);
void LDCF_A_mem(BYTE f, DWORD addr);
void STCF_A_mem(BYTE f, DWORD addr);

void LD_mem_R(BYTE f, DWORD addr);

void ANDCF_$3_mem(BYTE f, DWORD addr);
void ORCF_$3_mem(BYTE f, DWORD addr);

void XORCF_$3_mem(BYTE f, DWORD addr);
void LDCF_$3_mem(BYTE f, DWORD addr);

void STCF_$3_mem(BYTE f, DWORD addr);
void TSET_$3_mem(BYTE f, DWORD addr);

void RES_$3_mem(BYTE f, DWORD addr);
void SET_$3_mem(BYTE f, DWORD addr);

void CHG_$3_mem(BYTE f, DWORD addr);
void BIT_$3_mem(BYTE f, DWORD addr);

void JP_cc_mem(BYTE f, DWORD addr);

void CALL_cc_mem(BYTE f, DWORD addr);

void RET_cc(BYTE f, DWORD addr);

// Operand tables
extern void(*cpu_optable[0xFF])(BYTE f);
extern void(*cpu_optable_reg[0xFF])(BYTE f, enum OP_SIZE size, BYTE reg);
extern void(*cpu_optable_src[0xFF])(BYTE f, enum OP_SIZE size, DWORD addr);
extern void(*cpu_optable_dst[0xFF])(BYTE f, DWORD addr);
