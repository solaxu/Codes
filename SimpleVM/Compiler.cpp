#include "Compiler.h"

SCompiler::SCompiler()
{
	for (int i = 0; i < EShaderTokenType::TYPE_COUNT; ++i)
	{
		lexer.AddString(std::string(sl_tokens[i]), i);
	}

	lexer.AddString("#", TOKEN_TYPE_SHARP);
	lexer.AddString("true", TOKEN_TYPE_BOOL);
	lexer.AddString("false", TOKEN_TYPE_BOOL);
}

SCompiler::~SCompiler()
{

}

void SCompiler::Compile(const char* file)
{
	lexer.CreateTokenStream(file);
	BuildAST();
}

void SCompiler::BuildAST()
{

}