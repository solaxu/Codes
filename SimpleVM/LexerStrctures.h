#pragma once

#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>

#define TOKEN_TYPE_NONE 10000
#define TOKEN_TYPE_ID 10001
#define TOKEN_TYPE_INT 10002
#define TOKEN_TYPE_FLT 10003
#define TOKEN_TYPE_SHARP 10004 // comment
#define TOKEN_TYPE_BOOL 10005

struct LexerStateNode
{
	std::string lexeme;
	int type;
	LexerStateNode* next[256];

	LexerStateNode()
	{
		memset(next, 0, 256 * sizeof(LexerStateNode*));
		type = TOKEN_TYPE_NONE;
	}
};

struct Token
{
	int line_num;
	std::string lexeme;
	std::string error_info;
	int type;

	Token() :type(TOKEN_TYPE_NONE), line_num(0)
	{

	}
};

struct Lexer
{
	LexerStateNode* root;

	std::vector<Token> token_stream;

	std::vector<Token*> error_list;

	Lexer();
	~Lexer();
	void AddString(const std::string& str, int type);
	void CreateTokenStream(const char* file);
protected:
	void CheckToken(Token* tk);
	Token* AddToken(int line);
};