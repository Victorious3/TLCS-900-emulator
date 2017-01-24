#include "module.h"

static void destroy_mem(struct EX_MEMORY* self) {
	struct MEM_USERDATA* userdata = (struct MEM_USERDATA*)self->userdata;
	
	free(userdata->memory);
	free(userdata);
	free(self);
}

static void access_ram(DWORD address, BYTE* mem, bool read, struct MEM_USERDATA* userdata) {
	if (read) *mem = userdata->memory[address + userdata->offset];
	else userdata->memory[address + userdata->offset] = *mem;
}

static void access_rom(DWORD address, BYTE* mem, bool read, struct MEM_USERDATA* userdata) {
	if (read) *mem = userdata->memory[address + userdata->offset];
}

struct EX_MEMORY* mod_ram(DWORD size, DWORD offset) {
	struct EX_MEMORY* ram = malloc(sizeof(struct EX_MEMORY));
	ram->access = access_ram;
	ram->destroy = destroy_mem;
	struct MEM_USERDATA* userdata = malloc(sizeof(struct MEM_USERDATA));
	userdata->offset = offset;
	userdata->memory = malloc(sizeof(BYTE) * size);
	ram->userdata = userdata;
	return ram;
}

struct EX_MEMORY* mod_rom(DWORD size, DWORD offset) {
	struct EX_MEMORY* ram = mod_ram(size, offset);
	ram->access = access_rom;
	return ram;
}

void mod_copy_mem(struct EX_MEMORY* dst, BYTE* src, DWORD num) {
	struct MEM_USERDATA* userdata = (struct MEM_USERDATA*)dst->userdata;
	memcpy(userdata->memory, src, num);
}
