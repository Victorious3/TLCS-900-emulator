#pragma once

#include "900L.h"

// Struct for external memory modules
struct EX_MEMORY {

	/// Userdata for storing additional data
	void* userdata;

	/// Called to access the extended memory
	void(*access)(DWORD address, BYTE* mem, bool read, void* userdata);

	/// Called when another memory module is inserted,
	/// May be left NULL
	void(*destroy)(struct EX_MEMORY* self);
};

/// Creates a RAM type module
struct EX_MEMORY* mod_ram(DWORD size, DWORD offset);

/// Creates a ROM type module
/// Behaves exactly like a RAM type module but is read only
struct EX_MEMORY* mod_rom(DWORD size, DWORD offset);

/// Copies the specified bytes from the source buffer to the destination
/// This works with both RAM and ROM type modules
void mod_copy_mem(struct EX_MEMORY* dst, BYTE* src, DWORD num);
