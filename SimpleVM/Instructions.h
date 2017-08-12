#pragma once

// 6 bits op code
enum EOPs
{
	// binary ops
	OP_ADDF,
	OP_SUBF,
	OP_MULF,
	OP_DIVF,
    OP_MOV,
	OP_PUSH,
	OP_POP,
	OP_LEA,
	OP_LDS,
	OP_ADD,
	OP_INC,
	OP_SUB,
	OP_DEC,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_POW,
	OP_I2F,
	OP_F2I,
	OP_AND,
	OP_OR,
	OP_NOT,
	OP_JMP,
	OP_EQ,
	OP_LE,
	OP_LEQ,
	OP_CALL,
	OP_RET,
	OP_ALLOC,
	OP_FREE,
	OP_EQF,
	OP_LEF,
	OP_LEQF,
	OP_EXIT,
	OP_LOAD,
	OP_LOADF,
	OP_LOADB,
	OP_PRINT,
	OP_PRINTF,
	OP_CALLHOST,
	OP_COUNT,
};

static const char* instructioin_set[OP_COUNT] =
{
	"addf", "subf", "mulf", "divf",
	"mov", "push", "pop", "lea", "lds", "add",
	"inc", "sub", "dec", "mul", "div", "mod",
	"pow", "i2f", "f2i", "and", "or", "not",
	"jmp", "eq", "le", "leq", "call", "ret",
	"alloc", "free", "eqf", "lef", "leqf",
	"exit", "load", "loadf", "loadb", "print",
	"printf", "callh"
};

#define show_instruction(instruction) printf("%s\n", instructioin_set[instruction])

struct SRegister
{
	enum BasicReg
	{
		IP, BP, SP, FLAG,
		CS, SS, DS, ES,
		BREG_COUNT,
	};

	enum RegularReg
	{
		r0, r1, r2, r3, r4,
		r5, r6, r7, r8, r9,
		r10, r11, r12, r13,
		r14, r15, r16, r17,
		r18, r19, r20, r21,
		r22, r23, r24, r25,
		r26, r27, r28, r29,
		r30, r31, r32, r33,
		r34, r35, r36, r37,
		r38, r39, r40, r41,
		r42, r43, r44, r45,
		r46, r47, r48, r49,
		r50, r51, r52, r53,
		r54, r55, r56, r57,
		r58, r59, r60, r61,
		r62, r63, r64, r65,
		r66, r67, r68, r69,
		r70, r71, r72, r73,
		r74, r75, r76, r77,
		r78, r79, r80, r81,
		r82, r83, r84, r85,
		r86, r87, r88, r89,
		r90, r91, r92, r93,
		r94, r95,
		RREG_COUNT,
	};
	int is_used = 0; // is the register used, for register allocate.
	unsigned int value = 0; // immediate number or address
};

static const char* BRegisters[SRegister::BasicReg::BREG_COUNT] = {
	"IP", "BP", "SP", "FLAG",
	"CS", "SS", "DS", "ES"
};

static const char* Registers[SRegister::RegularReg::RREG_COUNT] = {
	"r0","r1","r2","r3","r4",
	"r5","r6","r7","r8","r9",
	"r10","r11","r12","r13",
	"r14","r15","r16","r17",
	"r18","r19","r20","r21",
	"r22","r23","r24","r25",
	"r26","r27","r28","r29",
	"r30","r31","r32","r33",
	"r34","r35","r36","r37",
	"r38","r39","r40","r41",
	"r42","r43","r44","r45",
	"r46","r47","r48","r49",
	"r50","r51","r52","r53",
	"r54","r55","r56","r57",
	"r58","r59","r60","r61",
	"r62","r63","r64","r65",
	"r66","r67","r68","r69",
	"r70","r71","r72","r73",
	"r74","r75","r76","r77",
	"r78","r79","r80","r81",
	"r82","r83","r84","r85",
	"r86","r87","r88","r89",
	"r90","r91","r92","r93",
	"r94","r95"
};