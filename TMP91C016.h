#pragma once

#include "900L.h"
#include "module.h"

/// The amount of memory modules that can be installed
enum { EX_MEMORY_SIZE = 4 };

/// Set an external memory slot
/// See module.h
void mc_setexmem(int slot, struct EX_MEMORY* memory);

// mc_ prefix stands for Microcontroller

// Address map SFRs
enum MC_REG {
	// Ports
	P1 = 0x1, P1CR = 0x4, P2 = 0x6, P2FC = 0x9, P5CR, P5FC, P5FC2, P5, P5UDE,
	P6FC3 = 0x10, P6 = 0x12, P7, P6CR, P6FC, P7CR, P7FC, P6UE, P9, P6FC2 = 0x1B, P7FC2, P9FC, P9UE, P7UDE,
	PBUDE = 0x20, PB = 0x22, PC, PBCR, PBFC, PCCR, PCFC, PCUDOE, PD, PDFC, PDCR, PDUE,

	// INTC
	DMA0V = 0x80, DMA1V, DMA2V, DMA3V, INTCLR = 0x88, DMAR, DMAB, IIMC = 0x8C,
	INTE0 = 0x90, INTE12, INTE3ALM4, INTEALM01, INTEALM23, INTETA01, INTETA23, INTERTCKEY, INTES0, INTES1, INTELCD, INTETC01, INTETC23, INTEP01,
	
	// CS/WAIT
	B0CS = 0xC0, B1CS, B2CS, B3CS, BEXCS = 0xC7, MSAR0, MAMR0, MSAR1, MAMR1, MSAR2, MAMR2, MSAR3, MAMR3,
	
	// CGEAR, DFM
	SYSCR0 = 0xE0, SYSCR1, SYSCR3, EMCCR0, EMCCR1, EMCCR2, EMCCR3, EMCCR4, DFMCR0, DFMCR1,
	
	// TMRA
	TA01RUN = 0x100, TA0REG = 0x102, TA1REG, TA01MOD, TA1FFCR, TA23RUN = 0x108, TA2REG = 0x10A, TA3REG, TA23MOD, TA3FFCR,
	
	// UART/SIO
	SC0BUF = 0x200, SC0CR, SC0MOD0, BR0CR, BR0ADD, SC0MOD1, SIRCR = 0x209, SC1BUF, SC1CR, SC1MOD0, BR1CR, BR1ADD, SC1MOD1,
	
	// DRAMC
	DREFCR = 0x430, DMEMCR,
	
	// WDT
	WDMOD = 0x300, WDCR,
	
	// RTC
	SECR = 0x320, MINR, HOURR, DAYR, DATER, MONTHR, YEARR, PAGER, RESTR,
	
	// MLD
	ALM = 0x330, MELALMC, MELFL, MELFH, ALMINT,
	
	// MMU
	LOCAL0 = 0x350, LOCAL1, LOCAL2, LOCAL3,
	
	// LCD
	LCDSAL = 0x360, LCDSAH, LCDSIZE, LCDCTL, LCDFFP, LCDCTL2 = 0x366,
	
	// VLD
	VLDCR0 = 0x440, VLDCR1, VLDCR2, HPCTST1 = 0x445, HPCTST2, VLDCTL = 0x449,
	
	// HVC
	HVREGA0 = 0x450, HVREGA1, HVREGA2, HVREGA3, HVREGA4, HVREGA5, HVREGA6, HVREGA7, HVREGB0, HVREGB1, HVREGB2, HVREGB3, HVREGB4, HVREGB5, HVREGB6, HVREGB7
};

/// Initializes the microcontroller, calls cpu_init
void mc_init(void);

/// Resets the microconctroller to the default state, calls cpu_reset
void mc_reset(void);

/// Starts the emulation
void mc_run(void(*interrupt)(void));

/// Returns the value of a register, preferred over cpu_getmem_b(addr) due to direct access
inline BYTE mc_getr(enum MC_REG reg);

/// Sets the value of a register, preferred over cpu_setmem_b(addr) due to direct access
inline void mc_setr(enum MC_REG reg, BYTE value);