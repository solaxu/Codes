#pragma once


#include "LexerStrctures.h"

enum EShaderTokenType
{
	TK_LAYOUT_IN,
	TK_LAYOUT_OUT,
	TK_VOID,
	TK_LBRACE,
	TK_RBRACE,
	TK_LBRACKET,
	TK_RBRACKET,
	TK_LPAREN,
	TK_RPAREN,
	TK_COMMA,
	TK_MATRIX,
	TK_FLOAT,
	TK_INT,
	TK_DOT,
	TK_SEMI,
	TK_ADD,
	TK_SUB,
	TK_MUL,
	TK_DIV,
	TK_MOD,
	TK_POW,
	TK_EQ,
	TK_LESS,
	TK_LEQ,
	TK_ASSIGN,
	TK_GREATER,
	TK_GREATEQ,
	TK_IF,
	TK_ELSE,
	TK_POSITION,
	TK_NORMAL,
	TK_TEXCOORD,
	TK_COLOR,
	TK_AND,
	TK_OR,
	TK_VECTOR,
	TK_WHILE,
	TK_CONTINUE,
	TK_STRUCT,
	TK_BREAK,
	TK_VS_MAIN,
	TK_PS_MAIN,
	TK_RETURN,
	TYPE_COUNT
};

static const char* sl_tokens[] = {
	"layout_in", "layout_out", "void", "{", "}", "[", "]", "(", ")",
	",", "matrix", "float", "int", ".", ";", "+", "-", "*", "/", "%",
	"^", "==", "<", "<=", "=", ">", ">=", "if", "else", "position", "normal",
	"texcoord", "color", "and", "or", "vector", "while", "continue", "struct",
	"break", "vs_main", "ps_main", "return"
};

struct SCompiler
{
	Lexer lexer;

	SCompiler();
	~SCompiler();

	void Compile(const char* file);
	void BuildAST();
};