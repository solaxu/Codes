#pragma once

#include "LexerStrctures.h"
#include "Instructions.h"

struct SSVMAssembler
{
	static const int MEMORY_PER_SCRIPT = 8 * 1024 * 1024; // 8MB memory for a script
	static const int TEXT_SEGMENT_OFFSET = 0; // for instructions
	static const int DATA_SEGMENT_OFFSET = 2 * 1024 * 1024; // for Global data
	static const int STACK_SEGMENT_OFFSET = 4 * 1024 * 1024; // for Stack frame
	static const int EXTRA_SEGMENT_OFFSET = 6 * 1024 * 1024; // for heap, may not use

	Lexer lexer;
	unsigned char* script_mem = nullptr;
	std::vector<std::string> errors;
	unsigned int byte_code_size = 0;

	SSVMAssembler();
	~SSVMAssembler();

	void Assemble(const char* file);
protected:
	void ConvertToByteCode();
	void DumpByteCode(const char* file);
};