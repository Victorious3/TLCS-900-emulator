#pragma once

#include <stdbool.h>

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

struct CPU_STATE {
	// General purpose registers (banks + 32 bit registers)
	union REG {
		BYTE  byte  [64 + 16];
		WORD  word  [32 +  8];
		DWORD qword [16 +  4];
	} REG;

	// Dedicated registers
	union {
		WORD SR; // Status register
		struct SR {
			unsigned SYSM : 1; // System mode
			unsigned IFF2 : 3; // Interrupt mask Flip-Flop
			unsigned MAX  : 1; // Minimum / Maximum (Always at 1 for this CPU)
			unsigned RFP2 : 3; // Register file pointer

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

/// Resets the CPU, puts everything into the default configuration
void cpu_reset(void);

/// Starts the emulation
void cpu_emulate(void (*interrupt)(void));

/// Sets the exit flag
void cpu_exit(void);
