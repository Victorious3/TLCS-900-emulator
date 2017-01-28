#pragma once

#include "../900L1.h"
#include "../register.h"
#include "../memory.h"

/*
	Header file for generating opcodes
*/

#define HEAD_OP($name) void $name(BYTE f)
#define HEAD_REG($name) void $name(BYTE f, enum OP_SIZE size, BYTE* reg, BYTE s)
#define HEAD_SRC($name) void $name(BYTE f, enum OP_SIZE size, DWORD addr, BYTE s)
#define HEAD_DST($name) void $name(BYTE f, DWORD addr, BYTE s);

#define CASE_B($macro, $body) case S_BYTE: {\
	$macro($body, BYTE, b) break; \
}
#define CASE_W($macro, $body) case S_WORD: {\
	$macro($body, WORD, w) break; \
}
#define CASE_DW($macro, $body) case S_DWORD: {\
	$macro($body, DWORD, dw) break; \
}
#define SWITCH_BWD($macro, $body) \
	switch (size) { \
		CASE_B($macro, $body) \
		CASE_W($macro, $body) \
		CASE_DW($macro, $body) \
	}

#define OP0($name, $body) HEAD_OP($name) $body
#define OP2($name, $p1, $p2, $body) OP_##$p1##_##$p2##($name, $body)

// R, r

#define _OP_R_r($body, $type, $suff) \
	$type R = getr_##$suff(cpu_getR(cpu_pull_op_b())); \
	$type _r = getr_##$suff(reg); \
	$type r = _r; \
	$body\
	setr_##$suff(reg, r);

#define OP_R_r($name, $body) HEAD_REG($name##_R_r) { SWITCH_BWD(_OP_R_r, $body) }

// r, $

#define _OP_r_$($body, $type, $suff) \
	$type $ = cpu_pull_op_##$suff(); \
	$type _r = getr_##$suff(reg); \
	$type r = _r; \
	$body\
	setr_##$suff(reg, r);

#define OP_r_$($name, $body) HEAD_REG($name##_r_$) { SWITCH_BWD(_OP_r_$, $body) }

// r, mem

#define _OP_r_mem($body, $type, $suff) \
	BYTE* reg = cpu_getr(s); \
	$type mem = cpu_getmem_##$suff(addr); \
	$type _r = getr_##$suff(reg); \
	$type r = _r; \
	$body\
	setr_##$suff(reg, r);

#define OP_r_mem($name, $body) HEAD_SRC($name##_r_mem) { SWITCH_BWD(_OP_r_mem, $body) }

// mem, R

#define _OP_mem_R($body, $type, $suff) \
	$type _mem = cpu_getmem_##$suff(addr); \
	$type mem = _mem; \
	$type R = getr_##$suff(cpu_getr(s)); \
	$body\
	cpu_setmem_##$suff(addr, mem);

#define OP_mem_R($name, $body) HEAD_SRC($name##_mem_R) { SWITCH_BWD(_OP_mem_R, $body) }

// mem, $

#define _OP_mem_$($body, $type, $suff) \
	$type _mem = cpu_getmem_##$suff(addr); \
	$type mem = _mem; \
	$type $ = cpu_pull_op_##$suff(); \
	$body\
	cpu_setmem_##$suff(addr, mem);

#define OP_mem_$($name, $body) HEAD_SRC($name##_mem_$) { SWITCH_BWD(_OP_mem_$, $body) }