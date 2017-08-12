#pragma once
#include <vector>
#include <map>
#include <wtypes.h>
#include "Instructions.h"
#include "Assembler.h"

struct SRuntimeStack
{
	int* elements; // stack elements
	int size; // stack size
	int top_index; // top of the stack
	int frame_index; // index of frame
};

struct SFunctionPrototype
{
	int entry_point;
	int param_count;
	int local_data_size;
	int stack_frame_size;
};

struct SInstruction
{
	int op_code;
	int oprands_count; 
	std::vector<int> oprands_list;
};

struct SInstructionStream
{
	std::vector<SInstruction*> instructions;
	int pc = 0;
};

struct SScript
{
	static const int MEMORY_PER_SCRIPT = 8 * 1024 * 1024; // 8MB memory for a script
	static const int TEXT_SEGMENT_OFFSET = 0; // for instructions
	static const int DATA_SEGMENT_OFFSET = 2 * 1024 * 1024; // for Global data
	static const int STACK_SEGMENT_OFFSET = 4 * 1024 * 1024; // for Stack frame
	static const int EXTRA_SEGMENT_OFFSET = 6 * 1024 * 1024; // for heap, may not use

	std::vector<SFunctionPrototype*> function_table;
	SInstructionStream instruction_stream;
	SRuntimeStack stack;
	unsigned char* script_memory;
	std::map<std::string, unsigned int> host_call_table;

	unsigned int text_size = 0;
	unsigned int cur_instruction = 0;
	bool exit_flag = false;

	//
	SRegister basic_regs[SRegister::BasicReg::BREG_COUNT];
	SRegister regular_regs[SRegister::RegularReg::RREG_COUNT];

	SScript();
	~SScript();

	void InitEnv(SSVMAssembler* assembler);
	void Check();		// check instructions
	void Run();			// run script
	void Destroy();		// destroy

	void PrintBasicRegisters();
	void PrintRegularRegisters();
protected:

	void check_reg();  // is the operands viable?
	void check_address(); // is the address viable? out of range?
	void check_byte(); // is the instruction viable?

	void exec_add();
	void exec_sub();
	void exec_mul();
	void exec_div();
	void exec_addf();
	void exec_subf();
	void exec_mulf();
	void exec_divf();
	void exec_mod();
	void exec_powf();
	void exec_inc();
	void exec_dec();
	void exec_eq();
	void exec_leq();
	void exec_le();
	void exec_eqf();
	void exec_leqf();
	void exec_lef();
	void exec_mov();
	void exec_push();
	void exec_pop();
	void exec_lea();
	void exec_lds();
	void exec_and();
	void exec_or();
	void exec_not();
	void exec_alloc();
	void exec_free();
	void exec_jmp();
	void exec_call();
	void exec_ret();
	void exec_f2i();
	void exec_i2f();
	void exec_exit();
	void exec_load();
	void exec_loadf();
	void exec_loadb();
	void exec_print();
	void exec_printf();
};