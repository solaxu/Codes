#include "Assembler.h"

SSVMAssembler::SSVMAssembler():byte_code_size(0)
{
	this->script_mem = new unsigned char[MEMORY_PER_SCRIPT];
	memset(this->script_mem, 0, MEMORY_PER_SCRIPT);

	for (int i = 0; i < EOPs::OP_COUNT; ++i)
	{
		lexer.AddString(instructioin_set[i], i);
	}

	for (int i = 0; i < SRegister::RegularReg::RREG_COUNT; ++i)
	{
		lexer.AddString(Registers[i], i);
	}

	lexer.AddString("#", TOKEN_TYPE_SHARP);
	lexer.AddString("true", TOKEN_TYPE_BOOL);
	lexer.AddString("false", TOKEN_TYPE_BOOL);
}

SSVMAssembler::~SSVMAssembler()
{
	if (this->script_mem != nullptr)
	{
		delete[] this->script_mem;
		this->script_mem = nullptr;
	}
}

void SSVMAssembler::Assemble(const char* file)
{
	lexer.CreateTokenStream(file);

	this->ConvertToByteCode();

#ifdef _DEBUG
	std::string f = std::string(file).substr(0, strlen(file) - 3);
	f.append("_byte.txt");
	this->DumpByteCode(f.c_str());
#endif
}

#define is_reg(a) ((a) >= SRegister::RegularReg::r0 && (a) < SRegister::RegularReg::RREG_COUNT)
#define is_digital(a) ((a) == TOKEN_TYPE_INT || (a) == TOKEN_TYPE_FLT)
#define make_instruction(code, dest, op1, op2) ((code)<<24|(dest)<<16|(op1)<<8|(op2))
#define fill_text_code(offset, src, len) memcpy(this->script_mem + (offset), src, len)

void SSVMAssembler::ConvertToByteCode()
{
	int index = 0;
	int mem_offset = 0;
	int byte_code_num = 0;
	std::vector<Token>& tokens = this->lexer.token_stream;
	while (index <tokens.size())
	{
		if (tokens[index].type == OP_MOV)
		{
			if (!is_reg(tokens[index + 1].type) || !is_reg(tokens[index + 2].type))
			{
				this->errors.push_back(std::string("want statement 'mov dest_reg reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, tokens[index + 2].type, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 3;
		}
		else if (tokens[index].type == OP_LOAD)
		{
			if (!is_reg(tokens[index + 1].type) || ! tokens[index + 2].type == TOKEN_TYPE_INT)
			{
				this->errors.push_back(std::string("want statement 'load _dest_reg integer', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, 0, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				int digital = atoi(tokens[index + 2].lexeme.c_str());
				fill_text_code(mem_offset, &digital, 4);
				mem_offset += 4;
				byte_code_num += 2;
			}
			index += 3;
		}
		else if (tokens[index].type == OP_LOADF)
		{
			if (!is_reg(tokens[index + 1].type) || !tokens[index + 2].type == TOKEN_TYPE_FLT)
			{
				this->errors.push_back(std::string("want statement 'load dest_reg float', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, 0, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				float digital = atof(tokens[index + 2].lexeme.c_str());
				fill_text_code(mem_offset, &digital, 4);
				mem_offset += 4;
				byte_code_num += 2;
			}
			index += 3;
		}
		else if (tokens[index].type == OP_LOADB)
		{
			if (!is_reg(tokens[index + 1].type) || tokens[index + 2].type != TOKEN_TYPE_BOOL)
			{
				this->errors.push_back(std::string("want statement 'loadb dest_reg boolen', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, 0, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				unsigned int b = (tokens[index + 2].lexeme == "true") ? 1 : 0;
				fill_text_code(mem_offset, &b, 4);
				mem_offset += 4;
				byte_code_num += 2;
			}
			index += 3;
		}
		else if (tokens[index].type == OP_ADD || tokens[index].type == OP_ADDF || 
			tokens[index].type == OP_SUB || tokens[index].type == OP_SUBF ||
			tokens[index].type == OP_DIV || tokens[index].type == OP_DIVF ||
			tokens[index].type == OP_MUL || tokens[index].type == OP_MULF ||
			tokens[index].type == OP_MOD || tokens[index].type == OP_POW )
		{
			if (!is_reg(tokens[index + 1].type) || !is_reg(tokens[index + 2].type) || !is_reg(tokens[index + 3].type))
			{
				this->errors.push_back(std::string("want statement 'arithmetic-op dest_reg, reg, reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, tokens[index + 2].type, tokens[index + 3].type);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 4;
		}
		else if (tokens[index].type == OP_DEC || tokens[index].type == OP_INC)
		{
			if (!is_reg(tokens[index + 1].type))
			{
				this->errors.push_back(std::string("want statement 'dec reg or inc reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, 0, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 2;
		}
		else if (tokens[index].type == OP_I2F || tokens[index].type == OP_F2I)
		{
			if (!is_reg(tokens[index + 1].type )|| !is_reg(tokens[index + 2].type))
			{
				this->errors.push_back(std::string("want statement 'cast-op dest_reg reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, tokens[index + 2].type, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 3;
		}
		else if (tokens[index].type == OP_AND || tokens[index].type == OP_OR)
		{
			if (!is_reg(tokens[index + 1].type) || !is_reg(tokens[index + 2].type) || !is_reg(tokens[index + 3].type))
			{
				this->errors.push_back(std::string("want statement 'and/or dest_reg reg reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, tokens[index + 2].type, tokens[index + 3].type);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 4;
		}
		else if (tokens[index].type == OP_NOT)
		{
			if (!is_reg(tokens[index + 1].type) || !is_reg(tokens[index + 2].type))
			{
				this->errors.push_back(std::string("want statement 'not dest_reg reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, tokens[index + 2].type, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 3;
		}
		else if (tokens[index].type == OP_JMP)
		{
			if (!is_reg(tokens[index + 1].type) || !is_reg(tokens[index + 2].type))
			{
				this->errors.push_back(std::string("want statement 'jump reg reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, 0, tokens[index + 1].type, tokens[index + 2].type);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 3;
		}
		else if (tokens[index].type == OP_EQ || tokens[index].type == OP_LE || tokens[index].type == OP_LEQ ||
			tokens[index].type == OP_EQF || tokens[index].type == OP_LEF || tokens[index].type == OP_LEQF)
		{
			if (!is_reg(tokens[index + 1].type) || !is_reg(tokens[index + 2].type) || !is_reg(tokens[index + 3].type))
			{
				this->errors.push_back(std::string("want statement 'cmp-op dest_reg reg reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, tokens[index + 2].type, tokens[index + 3].type);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 4;
		}
		else if (tokens[index].type == OP_CALL)
		{
			if (!is_digital(tokens[index + 1].type) )
			{
				this->errors.push_back(std::string("want statement 'call func_addr', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int func_ptr = atoi(tokens[index + 1].lexeme.c_str()) & 0x00ffffff;
				unsigned int ins = ((tokens[index].type & 0xff) << 24) | func_ptr;
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 2;
		}
		else if (tokens[index].type == OP_RET)
		{
			{
				unsigned int ins = make_instruction(tokens[index].type, 0, 0, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 1;
		}
		else if (tokens[index].type == OP_ALLOC)
		{
			if (!is_reg(tokens[index + 1].type) || !is_digital(tokens[index + 2].type))
			{
				this->errors.push_back(std::string("want statement 'alloc dest_reg reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, 0, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
				unsigned int mem_size = atoi(tokens[index + 2].lexeme.c_str());
				fill_text_code(mem_offset, &mem_size, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 3;
		}
		else if (tokens[index].type == OP_FREE)
		{
			if (!is_reg(tokens[index + 1].type))
			{
				this->errors.push_back(std::string("want statement 'free reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, tokens[index + 1].type, 0, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 2;
		}
		else if (tokens[index].type == OP_EXIT || tokens[index].type == OP_POP)
		{
			{
				unsigned int ins = make_instruction(tokens[index].type, 0, 0, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 1;
		}
		else if (tokens[index].type == OP_PUSH)
		{
			if (!is_reg(tokens[index + 1].type))
			{
				this->errors.push_back(std::string("want statement 'push reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, 0, tokens[index + 1].type, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 2;
		}
		else if (tokens[index].type == OP_PRINT)
		{
			if (!is_reg(tokens[index + 1].type))
			{
				this->errors.push_back(std::string("want statement 'push reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, 0, tokens[index + 1].type, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 2;
		}
		else if (tokens[index].type == OP_PRINTF)
		{
			if (!is_reg(tokens[index + 1].type))
			{
				this->errors.push_back(std::string("want statement 'push reg', line: %d\n" + std::to_string(tokens[index].line_num)));
			}
			else
			{
				unsigned int ins = make_instruction(tokens[index].type, 0, tokens[index + 1].type, 0);
				fill_text_code(mem_offset, &ins, 4);
				mem_offset += 4;
				++byte_code_num;
			}
			index += 2;
		}
	}
	this->byte_code_size = 4 * byte_code_num;
}

void SSVMAssembler::DumpByteCode(const char* file)
{
	FILE*  pf = nullptr;
	fopen_s(&pf, file, "wb");
	int off = 0;
	fwrite(this->script_mem, 1, this->byte_code_size, pf);
	fclose(pf);
}