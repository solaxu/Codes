#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <string.h>
#include <windows.h>

#define MAX_ID_LENGTH 250

#define BLUE FOREGROUND_BLUE
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define PURPLE FOREGROUND_RED|FOREGROUND_BLUE
#define CYAN FOREGROUND_GREEN|FOREGROUND_BLUE
#define WHITE FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED
#define YELLOW FOREGROUND_GREEN|FOREGROUND_RED

enum TokenCode
{
	TK_START, // 解析开始
	TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_MOD, // + - * / %
	TK_GT, TK_LT, TK_EQUAL, TK_LEQ, TK_GEQ, TK_NEQ, // > < ==  <= >= !=
	TK_ASSIGN, TK_DOT, // = .(成员运算符)
	TK_AND, TK_OR, // && ||
	TK_OPENPA, TK_CLOSEPA, TK_OPENBKT, TK_CLOSEBKT, TK_OPENBRE, TK_CLOSEBRE, // ( ) [ ] { }
	TK_SEMI, TK_COMMA, // ; ,
	TK_INT, TK_FLT, TK_STRING, // int float string
	TK_CSTR, TK_CLASS, TK_FUNC, // 常量字符串 class func
	TK_WHILE, TK_CONTINUE, TK_BREAK, // while continue break
	TK_IF, TK_IFELSE, TK_ELSE, // if ifelse else
	TK_INTNUM, TK_FLTNUM, // 整数 浮点数
	TK_RETURN, // return
	TK_POINTER, // ->
	TK_LIST, // list
	TK_IDENT, // 标识符
	TK_EOF, // end of file
	TK_SELF, // self
	TK_COMMENT, // 注释
	TK_BOOL,
	TK_VAR
};

enum ErrorCode
{
	ERR_NONE,
	ERR_LEXEM_LENGTH,
	ERR_FLOAT_DOTS,
};

#define KEYWORDS_NUM 41

typedef struct keyWordPair
{
	TokenCode m_tkCode;
	std::string m_word;
	unsigned int m_color;
}keyWordPair;

static keyWordPair keyWords[KEYWORDS_NUM] = {
	{ TK_PLUS,"+",RED },
	{ TK_MINUS,"-",RED },
	{ TK_MUL,"*",RED },
	{ TK_DIV,"/",RED },
	{ TK_MOD,"%",RED },
	{ TK_GT,">",RED },
	{ TK_LT,"<",RED },
	{ TK_EQUAL,"==",RED },
	{ TK_LEQ,"<=",RED },
	{ TK_GEQ,">=",RED },
	{ TK_NEQ,"!=",RED },
	{ TK_ASSIGN,"=",RED },
	{ TK_DOT,".",RED },
	{ TK_AND,"&&",RED },
	{ TK_OR,"||",RED },
	{ TK_OPENPA,"(",RED },
	{ TK_CLOSEPA,")",RED },
	{ TK_OPENBKT,"[",RED },
	{ TK_CLOSEBKT,"]",RED },
	{ TK_OPENBRE,"{",RED },
	{ TK_CLOSEBRE,"}",RED },
	{ TK_SEMI,";",WHITE },
	{ TK_COMMA,",",WHITE },
	{ TK_STRING,"string",BLUE },
	{ TK_INT,"int",BLUE },
	{ TK_FLT,"float",BLUE },
	{ TK_CLASS,"class",PURPLE },
	{ TK_FUNC,"func",PURPLE },
	{ TK_WHILE,"while",PURPLE },
	{ TK_CONTINUE,"continue",PURPLE },
	{ TK_BREAK,"break",PURPLE },
	{ TK_IF,"if",PURPLE },
	{ TK_IFELSE,"ifelse",PURPLE },
	{ TK_ELSE,"else",PURPLE },
	{ TK_RETURN,"return",PURPLE },
	{ TK_POINTER,"->",RED },
	{ TK_LIST,"list",BLUE },
	{ TK_SELF,"self",PURPLE },
	{ TK_COMMENT,"//",GREEN },
	{ TK_BOOL,"bool",BLUE },
	{ TK_VAR,"var",BLUE }
};

struct Token;
struct LexError;
struct Lexer;

static TokenCode lexState;
static char charStack[MAX_ID_LENGTH + 1];
static int  charStackTop;
static std::unordered_map<std::string, keyWordPair*> keyWordsTable;

typedef struct Token
{
	std::string m_lexeme;
	int m_lineNum;
	int m_tkCode;
	int m_errCode;
}Token;

typedef struct LexError
{
	std::string m_errorLexeme;
	std::string m_errInfo;
	int m_lineNum;
}LexError;

typedef struct Lexer
{
	std::vector<Token*> m_tkStream; // 不包含空格的Token流
	std::vector<Token*> m_tkStreamWithSpace; // 包含空格的Token流，仅仅是为演示语法高亮，后面会删掉
	std::vector<LexError*> m_lexErrors; // 词法错误信息
	std::string m_FileName;
	int m_fileSize;
	int m_lineNum;
}Lexer;

void initLexer();
void getTokens(char* stream, Lexer* lexer);
Lexer* lex(const char* file);
void destroyLexer(Lexer* lexer);
void printTokens(Lexer* lexer);
void printLexErrors(Lexer* lexer);
void reprintWithSyntaxLighting(Lexer* lexer);

void reset()
{
	charStackTop = 0;
	memset(charStack, 0, MAX_ID_LENGTH + 1);
	lexState = TK_START;
}

void errorHandle_LexemTooLong(Token* tk, Lexer* lexer)
{
	if (tk == NULL || lexer == NULL)
	{
		return;
	}

	if (charStackTop >= MAX_ID_LENGTH)
	{
		charStackTop = 0;
		tk->m_lexeme = charStack;
		LexError* err = new LexError;
		err->m_errorLexeme = charStack;
		err->m_errInfo = std::string("Lexeme's Length should be less than ") + std::to_string(MAX_ID_LENGTH);
		err->m_lineNum = lexer->m_lineNum;
		lexer->m_lexErrors.push_back(err);
		tk->m_errCode = ERR_LEXEM_LENGTH;
	}
}

bool isAlpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;
	return false;
}

bool isNumber(char c)
{
	if (c >= '0' && c <= '9')
		return true;
	return false;
}

void printToken(Token* tk)
{
	if (tk == NULL)
		return;
	std::cout << "Type " << tk->m_tkCode << " at Line " << tk->m_lineNum
		<< ": " << tk->m_lexeme << std::endl;
}

Token* readId(char* stream, int& index, Lexer* lexer)
{
	lexState = TK_IDENT;
	Token* tk = new Token;
	tk->m_errCode = ERR_NONE;
	while (index < lexer->m_fileSize)
	{
		if (isAlpha(stream[index]) || isNumber(stream[index]) || stream[index] == '_')
		{
			charStack[charStackTop] = stream[index];
			charStackTop += 1;
			errorHandle_LexemTooLong(tk, lexer);
		}
		else
		{
			//index -= 1;
			break;
		}
		index += 1;
	}
	charStack[charStackTop] = '\0';
	tk->m_lexeme = charStack;
	tk->m_lineNum = lexer->m_lineNum;
	tk->m_tkCode = lexState;
	lexer->m_tkStream.push_back(tk);
	// 演示语法高亮需要
	lexer->m_tkStreamWithSpace.push_back(tk);

	reset();
	return tk;
}

Token* readNumber(char* stream, int& index, Lexer* lexer)
{
	lexState = TK_INTNUM;
	Token* tk = new Token;
	tk->m_errCode = ERR_NONE;
	while (true)
	{
		errorHandle_LexemTooLong(tk, lexer);
		if (stream[index] == '.')
		{
			if (lexState == TK_FLTNUM)
			{
				LexError* err = new LexError;
				err->m_errInfo = std::string("Too many DOTs in a Float number");
				err->m_lineNum = lexer->m_lineNum;
				err->m_errorLexeme = charStack;
				lexer->m_lexErrors.push_back(err);
				tk->m_errCode = ERR_FLOAT_DOTS;
			}
			else
			{
				lexState = TK_FLTNUM;
			}
			charStack[charStackTop] = stream[index];
			charStackTop += 1;
		}
		else if (isNumber(stream[index]))
		{
			charStack[charStackTop] = stream[index];
			charStackTop += 1;
		}
		else
		{
			if (stream[index - 1] == '.')
			{
				charStack[charStackTop++] = '0';
			}
			break;
			//index -= 1;
		}
		index += 1;
	}
	charStack[charStackTop] = '\0';
	tk->m_tkCode = lexState;
	tk->m_lexeme = charStack;
	tk->m_lineNum = lexer->m_lineNum;
	lexer->m_tkStream.push_back(tk);
	// 语法高亮演示
	lexer->m_tkStreamWithSpace.push_back(tk);

	reset();
	return tk;
}

void isKeyWord(Token* tk)
{
	auto it = keyWordsTable.find(tk->m_lexeme);
	if (it != keyWordsTable.end())
	{
		tk->m_tkCode = (it->second)->m_tkCode;
	}
}

void readComment(char* stream, int& index, Lexer* lexer)
{
	Token* tk = new Token;
	tk->m_errCode = ERR_NONE;
	while (stream[index] != '\n')
	{
		tk->m_lexeme.push_back(stream[index]);
		index += 1;
	}
	if (tk->m_lexeme[tk->m_lexeme.size() - 1] == '\r')
		tk->m_lexeme.pop_back();

	tk->m_lineNum = lexer->m_lineNum;
	tk->m_tkCode = TK_COMMENT;
	lexer->m_tkStream.push_back(tk);
	// 语法高亮
	lexer->m_tkStreamWithSpace.push_back(tk);
}

void readConstString(char* stream, int& index, Lexer* lexer)
{
	Token* tk = new Token;
	tk->m_errCode = ERR_NONE;
	index += 1; // 跳过开始引号
	while (true)
	{
		// 只有遇到一个单引号结束，遇到由转义符修饰的不结束
		if (stream[index] == '"' && stream[index - 1] != '\\')
			break;
		tk->m_lexeme.push_back(stream[index++]);
	}
	index += 1; // 跳过结束引号
	tk->m_lineNum = lexer->m_lineNum;
	tk->m_tkCode = TK_CSTR;
	lexer->m_tkStream.push_back(tk);
	// 语法高亮
	lexer->m_tkStreamWithSpace.push_back(tk);
}

void readSingle(char* stream, int& index, Lexer* lexer, TokenCode tkCOde)
{
	Token* tk = new Token;
	tk->m_errCode = ERR_NONE;
	tk->m_lexeme = stream[index];
	tk->m_lineNum = lexer->m_lineNum;
	tk->m_tkCode = tkCOde;
	lexer->m_tkStream.push_back(tk);
	// 语法高亮
	lexer->m_tkStreamWithSpace.push_back(tk);
	index += 1;
}

void readDouble(char* stream, int& index, Lexer* lexer, TokenCode tkCOde)
{
	Token* tk = new Token;
	tk->m_errCode = ERR_NONE;
	tk->m_lexeme.push_back(stream[index]);
	tk->m_lexeme.push_back(stream[index+1]);
	tk->m_lineNum = lexer->m_lineNum;
	tk->m_tkCode = tkCOde;
	lexer->m_tkStream.push_back(tk);
	// 语法高亮
	lexer->m_tkStreamWithSpace.push_back(tk);
	index += 2;
}

void initLexer()
{
	reset();
	keyWordsTable.clear();

	int i = 0;
	while (i < KEYWORDS_NUM)
	{
		std::pair<std::string, keyWordPair*> p(keyWords[i].m_word, &keyWords[i]);
		keyWordsTable.insert(p);
		++i;
	}
}

void getTokens(char* stream, Lexer* lexer)
{
	if (stream == NULL || lexer == NULL)
	{
		return;
	}

	int index = 0;
	while (index < lexer->m_fileSize)
	{
		if (lexState == TK_START)
		{
			if (isAlpha(stream[index]) || stream[index] == '_')
			{
				Token* tk = readId(stream, index, lexer);
				isKeyWord(tk);
			}
			else if (isNumber(stream[index]))
			{
				readNumber(stream, index, lexer);
			}
			else if (stream[index] == ' ' || stream[index] == '\t')
			{
				Token* tk = new Token;
				tk->m_errCode = ERR_NONE;
				tk->m_lexeme = stream[index];
				tk->m_lineNum = lexer->m_lineNum;
				lexer->m_tkStreamWithSpace.push_back(tk);
				index += 1;
			}
			else if (stream[index] == '\n')
			{
				lexer->m_lineNum += 1;
				index += 1;
			}
			else if (stream[index] == '.')
			{
				readSingle(stream, index, lexer, TK_DOT);
			}
			else if (stream[index] == '+')
			{
				readSingle(stream, index, lexer, TK_PLUS);
			}
			else if (stream[index] == '-')
			{
				readSingle(stream, index, lexer, TK_MINUS);
			}
			else if (stream[index] == '*')
			{
				readSingle(stream, index, lexer, TK_MUL);
			}
			else if (stream[index] == '/')
			{
				if (stream[index + 1] == '/')
					readComment(stream, index, lexer);
				else
					readSingle(stream, index, lexer, TK_DIV);
			}
			else if (stream[index] == '%')
			{
				readSingle(stream, index, lexer, TK_MOD);
			}
			else if (stream[index] == '(')
			{
				readSingle(stream, index, lexer, TK_OPENPA);
			}
			else if (stream[index] == ')')
			{
				readSingle(stream, index, lexer, TK_CLOSEPA);
			}
			else if (stream[index] == '[')
			{
				readSingle(stream, index, lexer, TK_OPENBKT);
			}
			else if (stream[index] == ']')
			{
				readSingle(stream, index, lexer, TK_CLOSEBKT);
			}
			else if (stream[index] == '{')
			{
				readSingle(stream, index, lexer, TK_OPENBRE);
			}
			else if (stream[index] == '}')
			{
				readSingle(stream, index, lexer, TK_CLOSEBRE);
			}
			else if (stream[index] == ';')
			{
				readSingle(stream, index, lexer, TK_SEMI);
			}
			else if (stream[index] == ',')
			{
				readSingle(stream, index, lexer, TK_COMMA);
			}
			else if (stream[index] == '<')
			{
				if (stream[index + 1] == '=')
					readDouble(stream, index, lexer, TK_LEQ);
				else
					readSingle(stream, index, lexer, TK_LT);
			}
			else if (stream[index] == '>')
			{
				if (stream[index + 1] == '=')
					readDouble(stream, index, lexer, TK_GEQ);
				else
					readSingle(stream, index, lexer, TK_GT);
			}
			else if (stream[index] == '=')
			{
				if (stream[index + 1] == '=')
					readDouble(stream, index, lexer, TK_EQUAL);
				else
					readSingle(stream, index, lexer, TK_ASSIGN);
			}
			else if (stream[index] == '"')
			{
				readConstString(stream, index, lexer);
			}
			else if (stream[index] == '-')
			{
				if (stream[index + 1] == '>')
					readDouble(stream, index, lexer, TK_POINTER);
			}
			else if (stream[index] == '&')
			{
				if (stream[index + 1] == '&')
					readDouble(stream, index, lexer, TK_AND);
			}
			else if (stream[index] == '|')
			{
				if (stream[index + 1] == '|')
					readDouble(stream, index, lexer, TK_OR);
			}
			else if (stream[index] == '!')
			{
				if (stream[index + 1] == '=')
					readDouble(stream, index, lexer, TK_NEQ);
			}
			else
			{
				index += 1;
			}
		}
	}
}

Lexer* lex(const char* file)
{
	initLexer();
	FILE* fp = NULL;
	if (NULL == (fp = fopen(file, "rb")))
	{
		return NULL;
	}

	Lexer* lexer = NULL;
	char* stream = NULL;
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	stream = new char[fileSize + 1]; // fileSize有可能为0
	stream[fileSize] = '\0';
	if (0 == fread((void*)stream, 1, fileSize, fp))
	{
		goto FINAL;
	}

	lexer = new Lexer;
	lexer->m_FileName = file;
	lexer->m_fileSize = fileSize;
	lexer->m_lineNum = 1;
	getTokens(stream, lexer);

FINAL:
	delete[] stream;
	return lexer;
}

void destroyLexer(Lexer* lexer)
{
	for (auto it = lexer->m_tkStream.begin(); it != lexer->m_tkStream.end(); it++)
	{
		delete *it;
	}

	for (auto it = lexer->m_tkStreamWithSpace.begin(); it != lexer->m_tkStreamWithSpace.end(); it++)
	{
		delete *it;
	}

	for (auto it = lexer->m_lexErrors.begin(); it != lexer->m_lexErrors.end(); it++)
	{
		delete *it;
	}
}

void printTokens(Lexer* lexer)
{
	for (auto it = lexer->m_tkStream.begin(); it != lexer->m_tkStream.end(); it++)
	{
		printToken(*it);
	}
}

void reprintWithSyntaxLighting(Lexer* lexer)
{
	HANDLE hOut;
	//  获取输出流的句柄
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	int lineNum = 1;
	for (auto it = lexer->m_tkStreamWithSpace.begin(); it != lexer->m_tkStreamWithSpace.end(); it++)
	{
		Token* tk = *it;
		if (tk->m_lineNum != lineNum)
		{
			std::cout << std::endl;
			lineNum += 1;
		}
		if (tk->m_tkCode == TK_IDENT)
		{
			SetConsoleTextAttribute(hOut, WHITE);
			std::cout << tk->m_lexeme;
		}
		else if (tk->m_tkCode == TK_CSTR)
		{
			SetConsoleTextAttribute(hOut, YELLOW);
			std::cout << "\"" << tk->m_lexeme << "\"";
		}
		else if (tk->m_tkCode == TK_INTNUM || tk->m_tkCode == TK_FLTNUM)
		{
			SetConsoleTextAttribute(hOut, CYAN);
			std::cout << tk->m_lexeme;
		}
		else if (tk->m_tkCode == TK_COMMENT)
		{
			SetConsoleTextAttribute(hOut, GREEN);
			std::cout << tk->m_lexeme;
		}
		else
		{
			auto ti = keyWordsTable.find(tk->m_lexeme);
			if (ti != keyWordsTable.end())
			{
				SetConsoleTextAttribute(hOut, (ti->second)->m_color);
				std::cout << tk->m_lexeme;
			}
			else
			{
				SetConsoleTextAttribute(hOut, WHITE);
				std::cout << tk->m_lexeme;
			}
		}
	}
}

void printLexErrors(Lexer* lexer)
{
	for (auto it = lexer->m_lexErrors.begin(); it != lexer->m_lexErrors.end(); it++)
	{
		LexError* err = *it;
		std::cout << "Error at line " << err->m_lineNum << ": " << err->m_errInfo
			<< ", with lexeme \"" << err->m_errorLexeme << "\"" << std::endl;
	}
}

int main(int argc, char** argv)
{
	Lexer* lexer = lex("test.txt");
	printTokens(lexer);
	printLexErrors(lexer);
	std::cout << std::endl;
	reprintWithSyntaxLighting(lexer);
	return 0;
}
