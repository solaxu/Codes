#include "RuntimeStructures.h"
#include <assert.h>
#include "Instructions.h"
#include "HeaderDatas.h"
#include <math.h>

SScript::SScript():script_memory(nullptr), text_size(0), exit_flag(false)
{
	
}

SScript::~SScript()
{
	Destroy();
}

void SScript::Destroy()
{

}

void SScript::InitEnv(SSVMAssembler* assembler)
{
	Destroy();
	this->script_memory = assembler->script_mem;
	assert(this->script_memory != nullptr);

	for (int i = 0; i < SRegister::BasicReg::BREG_COUNT; ++i)
	{
		memset(&this->basic_regs[i], 0, sizeof(SRegister));
	}

	for (int i = 0; i < SRegister::RegularReg::RREG_COUNT; ++i)
	{
		memset(&this->regular_regs[i], 0, sizeof(SRegister));
	}

	this->basic_regs[SRegister::SRegister::CS].value = (unsigned int)(this->script_memory + SScript::TEXT_SEGMENT_OFFSET);
	this->basic_regs[SRegister::SRegister::DS].value = (unsigned int)(this->script_memory + SScript::DATA_SEGMENT_OFFSET);
	this->basic_regs[SRegister::SRegister::SS].value = (unsigned int)(this->script_memory + SScript::STACK_SEGMENT_OFFSET);
	this->basic_regs[SRegister::SRegister::ES].value = (unsigned int)(this->script_memory + SScript::EXTRA_SEGMENT_OFFSET);

	this->basic_regs[SRegister::SRegister::SP].value = this->basic_regs[SRegister::SRegister::SS].value;
	this->basic_regs[SRegister::SRegister::BP].value = this->basic_regs[SRegister::SRegister::SS].value;

	this->basic_regs[SRegister::SRegister::IP].value = (unsigned int)this->script_memory;
}

void SScript::PrintBasicRegisters()
{
	printf("----------- Basic Registers: -----------");
	printf("IP: %x, ", this->basic_regs[SRegister::BasicReg::IP].value);
	printf("BP: %x, ", this->basic_regs[SRegister::BasicReg::BP].value);
	printf("SP: %x, ", this->basic_regs[SRegister::BasicReg::SP].value);
	printf("FLAG: %x\n", this->basic_regs[SRegister::BasicReg::FLAG].value);
	printf("CS: %x, ", this->basic_regs[SRegister::BasicReg::CS].value);
	printf("SS: %x, ", this->basic_regs[SRegister::BasicReg::SS].value);
	printf("DS: %x, ", this->basic_regs[SRegister::BasicReg::DS].value);
	printf("ES: %x\n", this->basic_regs[SRegister::BasicReg::ES].value);
}

void SScript::PrintRegularRegisters()
{
	printf("----------- Basic Registers: -----------");
	for (int i = 0; i < SRegister::RegularReg::RREG_COUNT; ++i)
	{
		if (this->regular_regs[i].value)
			printf("R%d: %d, ", i - 0, *(int*)this->regular_regs[i].value);
		if (i % 4 == 0)
			printf("\n");
	}
}

void SScript::Check()
{

}

#define INS_CODE(ins) (((ins) & 0xff000000) >> 24)
#define INS_OP_2(ins) ((ins) & 0x000000ff)
#define INS_OP_1(ins) (((ins) & 0x0000ff00) >> 8)
#define INS_DEST(ins) (((ins) & 0x00ff0000) >> 16)
#define OP_TYPE(op) (((op) & 128) >> 6)
#define OP_REG(op) ((op) & 127)
void SScript::Run()
{
	while (!this->exit_flag)
	{
		this->cur_instruction = *(unsigned int*)(this->basic_regs[SRegister::BasicReg::IP].value);
		unsigned int op = ((this->cur_instruction & 0xff000000) >> 24);
		switch (op)
		{
		case EOPs::OP_MOV: this->exec_mov(); break;
		case EOPs::OP_PUSH: this->exec_push(); break;
		case EOPs::OP_POP: this->exec_pop(); break;
		case EOPs::OP_LEA: this->exec_lea(); break;
		case EOPs::OP_LDS: this->exec_lds(); break;
		case EOPs::OP_ADD: this->exec_add(); break;
		case EOPs::OP_INC: this->exec_inc(); break;
		case EOPs::OP_SUB: this->exec_sub(); break;
		case EOPs::OP_DEC: this->exec_dec(); break;
		case EOPs::OP_MUL: this->exec_mul(); break;
		case EOPs::OP_DIV: this->exec_div(); break;
		case EOPs::OP_MOD: this->exec_mod(); break;
		case EOPs::OP_POW: this->exec_powf(); break;
		case EOPs::OP_AND: this->exec_and(); break;
		case EOPs::OP_NOT: this->exec_not(); break;
		case EOPs::OP_OR: this->exec_or(); break;
		case EOPs::OP_JMP: this->exec_jmp(); break;
		case EOPs::OP_EQ: this->exec_eq(); break;
		case EOPs::OP_LEQ: this->exec_leq(); break;
		case EOPs::OP_LE: this->exec_le(); break;
		case EOPs::OP_EQF: this->exec_eqf(); break;
		case EOPs::OP_LEQF: this->exec_leqf(); break;
		case EOPs::OP_LEF: this->exec_lef(); break;
		case EOPs::OP_I2F: this->exec_i2f(); break;
		case EOPs::OP_F2I: this->exec_f2i(); break;
		case EOPs::OP_CALL: this->exec_call(); break;
		case EOPs::OP_RET: this->exec_ret(); break;
		case EOPs::OP_ALLOC: this->exec_alloc(); break;
		case EOPs::OP_FREE: this->exec_free(); break;
		case EOPs::OP_EXIT: this->exec_exit(); break;
		case EOPs::OP_LOAD: this->exec_load(); break;
		case EOPs::OP_LOADF: this->exec_loadf(); break;
		case EOPs::OP_LOADB: this->exec_loadb(); break;
		case EOPs::OP_PRINT: this->exec_print(); break;
		case EOPs::OP_PRINTF: this->exec_printf(); break;
		case EOPs::OP_ADDF: this->exec_addf(); break;
		case EOPs::OP_SUBF: this->exec_subf(); break;
		case EOPs::OP_MULF: this->exec_mulf(); break;
		case EOPs::OP_DIVF: this->exec_divf(); break;
		default:break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void SScript::check_address()
{

}

void SScript::check_reg()
{

}

void SScript::check_byte()
{

}

//////////////////////////////////////////////////////////////////////////

void SScript::exec_add()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(int*)this->regular_regs[dest].value = *(int*)this->regular_regs[op1].value + *(int*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("add %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}

void SScript::exec_sub()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(int*)this->regular_regs[dest].value = *(int*)this->regular_regs[op1].value - *(int*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("sub %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}

void SScript::exec_mul()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(int*)this->regular_regs[dest].value = *(int*)this->regular_regs[op1].value * *(int*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("mul %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}

void SScript::exec_div()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(int*)this->regular_regs[dest].value = *(int*)this->regular_regs[op1].value / *(int*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("div %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}

void SScript::exec_addf()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(float*)this->regular_regs[dest].value = *(float*)this->regular_regs[op1].value + *(float*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("addf %s: %f, %s: %f, %s: %f\n", 
		Registers[dest], *(float*)(regular_regs[dest].value),
		Registers[op1], *(float*)(regular_regs[op1].value),
		Registers[op2], *(float*)(regular_regs[op2].value));
#endif
}

void SScript::exec_subf()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(float*)this->regular_regs[dest].value = *(float*)this->regular_regs[op1].value - *(float*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("subf %s: %f, %s: %f, %s: %f\n",
		Registers[dest], *(float*)(regular_regs[dest].value),
		Registers[op1], *(float*)(regular_regs[op1].value),
		Registers[op2], *(float*)(regular_regs[op2].value));
#endif
}

void SScript::exec_mulf()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(float*)this->regular_regs[dest].value = *(float*)this->regular_regs[op1].value * *(float*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("mulf %s: %f, %s: %f, %s: %f\n",
		Registers[dest], *(float*)(regular_regs[dest].value),
		Registers[op1], *(float*)(regular_regs[op1].value),
		Registers[op2], *(float*)(regular_regs[op2].value));
#endif
}

void SScript::exec_divf()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(float*)this->regular_regs[dest].value = *(float*)this->regular_regs[op1].value / *(float*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("divf %s: %f, %s: %f, %s: %f\n",
		Registers[dest], *(float*)(regular_regs[dest].value),
		Registers[op1], *(float*)(regular_regs[op1].value),
		Registers[op2], *(float*)(regular_regs[op2].value));
#endif
}

void SScript::exec_mod()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(int*)this->regular_regs[dest].value = *(int*)this->regular_regs[op1].value % *(int*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("mod %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}

void SScript::exec_powf()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(float*)this->regular_regs[dest].value = pow(*(float*)this->regular_regs[op1].value,*(float*)this->regular_regs[op2].value);
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("pow %s: %f, %s: %f, %s: %f\n",
		Registers[dest], *(float*)(regular_regs[dest].value),
		Registers[op1], *(float*)(regular_regs[op1].value),
		Registers[op2], *(float*)(regular_regs[op2].value));
#endif
}

void SScript::exec_inc()
{
	int dest = INS_DEST(this->cur_instruction);
	++*(int*)this->regular_regs[dest].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("inc %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value));
#endif
}

void SScript::exec_dec()
{
	int dest = INS_DEST(this->cur_instruction);
	--*(int*)this->regular_regs[dest].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("dec %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value));
#endif
}

void SScript::exec_not()
{
	int dest = INS_DEST(this->cur_instruction);
	int op1 = INS_OP_1(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = !*(bool*)this->regular_regs[op1].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("not %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value));
#endif
}

void SScript::exec_or()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = *(bool*)this->regular_regs[op1].value || *(bool*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("or %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}

void SScript::exec_and()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = *(bool*)this->regular_regs[op1].value && *(bool*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("and %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}


void SScript::exec_eqf()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = *(float*)this->regular_regs[op1].value == *(float*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("eqf %s: %d, %s: %f, %s: %f\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(float*)(regular_regs[op1].value),
		Registers[op2], *(float*)(regular_regs[op2].value));
#endif
}

void SScript::exec_leqf()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = *(float*)this->regular_regs[op1].value <= *(float*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("leqf %s: %d, %s: %f, %s: %f\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(float*)(regular_regs[op1].value),
		Registers[op2], *(float*)(regular_regs[op2].value));
#endif
}

void SScript::exec_lef()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = *(float*)this->regular_regs[op1].value < *(float*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("lef %s: %d, %s: %f, %s: %f\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(float*)(regular_regs[op1].value),
		Registers[op2], *(float*)(regular_regs[op2].value));
#endif
}

void SScript::exec_eq()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = *(int*)this->regular_regs[op1].value == *(int*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("eq %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}

void SScript::exec_le()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = *(int*)this->regular_regs[op1].value < *(int*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("le %s: %d, %s: %d, %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}

void SScript::exec_leq()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = *(int*)this->regular_regs[op1].value <= *(int*)this->regular_regs[op2].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("leq %s: %d, %s, %d, %s, %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], *(int*)(regular_regs[op2].value));
#endif
}

void SScript::exec_mov()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	memcpy((unsigned char*)this->regular_regs[dest].value, (unsigned char*)this->regular_regs[op1].value, 4);
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("mov %s: %08x, %s: %08x\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value));
#endif
}

void SScript::exec_lea()
{

}

void SScript::exec_lds()
{

}

void SScript::exec_push()
{
	// move sth to top, stack top pointer += 4
	int op1 = INS_OP_1(this->cur_instruction);
	// set sp to regular register's value member.
	this->regular_regs[op1].value = this->basic_regs[SRegister::BasicReg::SP].value;
	this->basic_regs[SRegister::BasicReg::SP].value += 4;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("push %s: %08x, %s: %08x, %s: %08x\n",
		Registers[op1], (regular_regs[op1].value),
		BRegisters[SRegister::BasicReg::IP], basic_regs[SRegister::BasicReg::IP].value,
		BRegisters[SRegister::BasicReg::SP], basic_regs[SRegister::BasicReg::SP].value);
#endif
}

void SScript::exec_pop()
{
	this->basic_regs[SRegister::BasicReg::SP].value -= 4;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;

#ifdef _DEBUG
	printf("pop %s: %08x, %s: %08x\n",
		BRegisters[SRegister::BasicReg::IP], basic_regs[SRegister::BasicReg::IP].value,
		BRegisters[SRegister::BasicReg::SP], basic_regs[SRegister::BasicReg::SP].value);
#endif
}

void SScript::exec_alloc()
{
	// chunk memory alloc
}

void SScript::exec_free()
{
	// chunk memory free
}

void SScript::exec_jmp()
{
	// jump op1 op2 , op2 is jmp-dest = mem_script + [ri]
	int op1 = INS_OP_1(this->cur_instruction);
	int op2 = INS_OP_2(this->cur_instruction);
	bool cond = *(bool*)(this->regular_regs[op1].value);
	if (cond)
		this->basic_regs[SRegister::BasicReg::IP].value = (unsigned int)(this->script_memory + 4 * *(unsigned int*)this->regular_regs[op2].value);
	else
		this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("jmp %s: %d, %s: %08x\n",
		Registers[op1], *(int*)(regular_regs[op1].value),
		Registers[op2], (unsigned int)(this->script_memory + *(unsigned int*)this->regular_regs[op2].value));
#endif
}

void SScript::exec_call()
{
#ifdef _DEBUG
	printf("before call %s: %08x, %s: %08x, %s: %08x\n",
		BRegisters[SRegister::BasicReg::IP], basic_regs[SRegister::BasicReg::IP].value,
		BRegisters[SRegister::BasicReg::SP], basic_regs[SRegister::BasicReg::SP].value,
		BRegisters[SRegister::BasicReg::BP], basic_regs[SRegister::BasicReg::BP].value);
#endif
	// push bp, push ip, op1 -> ip, bp = sp
	int op1 = this->cur_instruction & 0x00ffffff;
	// push bp to save
	memcpy((unsigned char*)this->basic_regs[SRegister::BasicReg::SP].value, &this->basic_regs[SRegister::BasicReg::BP].value, 4);
	this->basic_regs[SRegister::BasicReg::SP].value += 4;
	// push next ip to save
	int next_ip = this->basic_regs[SRegister::BasicReg::IP].value + 4;
	memcpy((unsigned char*)this->basic_regs[SRegister::BasicReg::SP].value, &next_ip, 4);
	this->basic_regs[SRegister::BasicReg::SP].value += 4;
	// new bp
	this->basic_regs[SRegister::BasicReg::BP].value = this->basic_regs[SRegister::BasicReg::SP].value;
	// 这里是不太准确的 4 * op1
	this->basic_regs[SRegister::BasicReg::IP].value = (unsigned int)(this->script_memory + 4 * op1);

#ifdef _DEBUG
	printf("after call %08x, %s: %08x, %s: %08x, %s: %08x\n", op1,
		BRegisters[SRegister::BasicReg::IP], basic_regs[SRegister::BasicReg::IP].value,
		BRegisters[SRegister::BasicReg::SP], basic_regs[SRegister::BasicReg::SP].value,
		BRegisters[SRegister::BasicReg::BP], basic_regs[SRegister::BasicReg::BP].value);
#endif
}

void SScript::exec_ret()
{
	//
	this->basic_regs[SRegister::BasicReg::IP].value = *(unsigned int*)(this->basic_regs[SRegister::BasicReg::BP].value - 4);
	this->basic_regs[SRegister::BasicReg::SP].value = this->basic_regs[SRegister::BasicReg::BP].value - 8;
	this->basic_regs[SRegister::BasicReg::BP].value = *(unsigned int*)(this->basic_regs[SRegister::BasicReg::BP].value - 8);

#ifdef _DEBUG
	printf("ret %s: %08x, %s: %08x, %s: %08x\n",
		BRegisters[SRegister::BasicReg::IP], basic_regs[SRegister::BasicReg::IP].value,
		BRegisters[SRegister::BasicReg::SP], basic_regs[SRegister::BasicReg::SP].value,
		BRegisters[SRegister::BasicReg::BP], basic_regs[SRegister::BasicReg::BP].value);
#endif
}

void SScript::exec_f2i()
{
	// float(int)
	int op1 = INS_OP_1(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(int*)this->regular_regs[dest].value = (int)*(float*)this->regular_regs[op1].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("f2i %s: %d, %s: %f\n",
		Registers[dest], *(int*)(regular_regs[dest].value),
		Registers[op1], *(float*)(regular_regs[op1].value));
#endif
}

void SScript::exec_i2f()
{
	// int(float)
	int op1 = INS_OP_1(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(float*)this->regular_regs[dest].value = (float)*(int*)this->regular_regs[op1].value;
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
#ifdef _DEBUG
	printf("i2f %s: %f, %s: %d\n",
		Registers[dest], *(float*)(regular_regs[dest].value),
		Registers[op1], *(int*)(regular_regs[op1].value));
#endif
}

void SScript::exec_exit()
{
	this->exit_flag = true;
#ifdef _DEBUG
	printf("exit\n");
#endif
}

void SScript::exec_load()
{
	int dest = INS_DEST(this->cur_instruction);
	*(int*)this->regular_regs[dest].value = *(int*)(this->basic_regs[SRegister::BasicReg::IP].value + 4);
	this->basic_regs[SRegister::BasicReg::IP].value += 8;
#ifdef _DEBUG
	printf("load %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value));
#endif
}

void SScript::exec_loadf()
{
	int dest = INS_DEST(this->cur_instruction);
	*(float*)this->regular_regs[dest].value = *(float*)(this->basic_regs[SRegister::BasicReg::IP].value + 4);
	this->basic_regs[SRegister::BasicReg::IP].value += 8;
#ifdef _DEBUG
	printf("loadf %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value));
#endif
}

void SScript::exec_loadb()
{
	int op1 = INS_OP_1(this->cur_instruction);
	int dest = INS_DEST(this->cur_instruction);
	*(bool*)this->regular_regs[dest].value = *(bool*)(this->basic_regs[SRegister::BasicReg::IP].value + 4);
	this->basic_regs[SRegister::BasicReg::IP].value += 8;
#ifdef _DEBUG
	printf("loadb %s: %d\n",
		Registers[dest], *(int*)(regular_regs[dest].value));
#endif
}

void SScript::exec_print()
{
	int op1 = INS_OP_1(this->cur_instruction);
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
	printf("print %s: %d\n",
		Registers[op1], *(int*)(regular_regs[op1].value));
}

void SScript::exec_printf()
{
	int op1 = INS_OP_1(this->cur_instruction);
	this->basic_regs[SRegister::BasicReg::IP].value += 4;
	printf("print %s: %f\n",
		Registers[op1], *(float*)(regular_regs[op1].value));
}