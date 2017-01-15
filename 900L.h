#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

/// All memory
struct CPU_STATE {
	/*  
		General purpose bank registers
		
		r           +3                +2                +1                +0
		----+-----------------+-----------------+-----------------+-----------------+
		00H |      QW0    (QWA|0)    QA0    <XWA|0>    RW0    (RWA|0)    RA0        |
		04H |      QB0    (QBC|0)    QC0    <XBC|0>    RB0    (RBC|0)    RC0        |  BANK 0
		08H |      QD0    (QDE|0)    QE0    <XDE|0>    RD0    (RDE|0)    RE0        |
		0CH |      QH0    (QHL|0)    QL0    <XHL|0>    RH0    (RHL|0)    RL0        |
		----+-----------------+-----------------+-----------------+-----------------+
		10H |      QW1    (QWA|1)    QA1    <XWA|1>    RW1    (RWA|1)    RA1        |
		14H |      QB1    (QBC|1)    QC1    <XBC|1>    RB1    (RBC|1)    RC1        |  BANK 1
		18H |      QD1    (QDE|1)    QE1    <XDE|1>    RD1    (RDE|1)    RE1        |
		1CH |      QH1    (QHL|1)    QL1    <XHL|1>    RH1    (RHL|1)    RL1        |
		----+-----------------+-----------------+-----------------+-----------------+
		20H |      QW2    (QWA|2)    QA2    <XWA|2>    RW2    (RWA|2)    RA2        |
		24H |      QB2    (QBC|2)    QC2    <XBC|2>    RB2    (RBC|2)    RC2        |  BANK 2
		28H |      QD2    (QDE|2)    QE2    <XDE|2>    RD2    (RDE|2)    RE2        |
		2CH |      QH2    (QHL|2)    QL2    <XHL|2>    RH2    (RHL|2)    RL2        |
		----+-----------------+-----------------+-----------------+-----------------+
		30H |      QW3    (QWA|3)    QA3    <XWA|3>    RW3    (RWA|3)    RA3        |
		34H |      QB3    (QBC|3)    QC3    <XBC|3>    RB3    (RBC|3)    RC3        |  BANK 3
		38H |      QD3    (QDE|3)    QE3    <XDE|3>    RD3    (RDE|3)    RE3        |
		3CH |      QH3    (QHL|3)    QL3    <XHL|3>    RH3    (RHL|3)    RL3        |
		----+-----------------+-----------------+-----------------+-----------------+

		Current bank selected by RTF
		Will point at the above mentioned bank registers

		----+-----------------+-----------------+-----------------+-----------------+
		D0H |      QW     (QWA| )    QA     <XWA| >      W      (W|A )   A          |
		D4H |      QB     (QBC| )    QC     <XBC| >      B      (B|C )   C          |
		D8H |      QD     (QDE| )    QE     <XDE| >      D      (D|E )   E          |
		DCH |      QH     (QHL| )    QL     <XHL| >      H      (H|L )   L          |
		----+-----------------+-----------------+-----------------+-----------------+

		Previous bank selected by (RTF + 7) % 8
		Will point at the above mentioned bank registers

		----+-----------------+-----------------+-----------------+-----------------+
		E0H |      QW'    (QWA|')    QA'    <XWA|'>      W'     (W|A')   A'         |
		E4H |      QB'    (QBC|')    QC'    <XBC|'>      B'     (B|C')   C'         |
		E8H |      QD'    (QDE|')    QE'    <XDE|'>      D'     (D|E')   E'         |
		ECH |      QH'    (QHL|')    QL'    <XHL|'>      H'     (H|L')   L'         |
		----+-----------------+-----------------+-----------------+-----------------+
	*/
	union {
		BYTE byte[64];
		WORD word[32];
		DWORD dword[16];
	} BANK;

	/*   
		General purpose registers & Stack Pointer
				
		r           +3                +2                +1                +0
		----+-----------------+-----------------+-----------------+-----------------+
		F0H |     QIXH    (QIX| )    QIXL   <XIX| >    IXH      (I|X )   IXL        |
		F4H |     QIYH    (QBC| )    QIYL   <XIY| >    IYH      (I|Y )   IYL        |
		F8H |     QIZH    (QDE| )    QIZL   <XIZ| >    IZH      (I|Z )   IZL        |
		FCH |     QSPH    (QHL| )    QSPL   <XSP| >    SPH      (S|P )   SPL        |
		----+-----------------+-----------------+-----------------+-----------------+
	*/
	union {
		BYTE byte[16];
		WORD word[8];
		DWORD dword[4];
	} REG;

	// Dedicated registers
	union {
		WORD SR; // Status register
		struct SR {
			unsigned SYSM : 1; // System mode
			unsigned IFF  : 3; // Interrupt mask Flip-Flop
			unsigned MAX  : 1; // Minimum / Maximum (Always at 1 for this CPU)
			unsigned RFP  : 3; // Register file pointer

			union F {
				BYTE F; // Flag register
				struct {
					unsigned S : 1; // Sign flag
					unsigned Z : 1; // Zero flag
					unsigned   : 1;
					unsigned H : 1; // Half carry flag
					unsigned   : 1;
					unsigned V : 1; // Parity/Overflow flag
					unsigned N : 1; // ADD/SUB flag
					unsigned C : 1; // Carry flag
				};
			};
		};
	};

	WORD  FS; // Flags shadow register
	DWORD PC; // Program counter

	// Control registers

	// DMA 
	DWORD DMA_source  [4];
	DWORD DMA_dest    [4];
	WORD  DMA_counter [4];
	BYTE  DMA_mode    [4];

	WORD INTNEST; // Interrupt/Nesting counter

} CPU_STATE;

/// Resets the CPU (see cpu_reset) and does a few assertions to ensure platform compatibility
void cpu_init(void);

/// Resets the CPU, puts everything into the default configuration
void cpu_reset(void);

/// Starts the emulation
void cpu_run(void (*interrupt)(void));

/// Sets the exit flag
void cpu_exit(void);

