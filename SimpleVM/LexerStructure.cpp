#include "LexerStrctures.h"

#define is_dig(c) ((c) >= '0' && (c) <= '9')
#define is_alpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))

Lexer::Lexer() : root(new LexerStateNode())
{

}

Lexer::~Lexer()
{

}

void Lexer::AddString(const std::string& str, int type)
{
	auto r = this->root;
	int index = 0;
	
	while (index < str.length())
	{
		if (r->next[(int)str[index]] == nullptr)
		{
			auto node = new LexerStateNode();
			node->lexeme = str.substr(0, index + 1);
			node->type = TOKEN_TYPE_ID;
			r->next[(int)str[index]] = node;
			r = node;
		}
		else
		{
			r = r->next[(int)str[index]];
		}
		++index;
	}
	r->type = type;
}

void Lexer::CheckToken(Token* tk)
{
	if (tk)
	{
		std::string& lexeme = tk->lexeme;
		if (lexeme.length() == 0)
		{
			tk->error_info = std::string("there's no lexeme. Line: ") + std::string(std::to_string(tk->line_num));
			this->error_list.push_back(tk);
			return;
		}
		else
		{
			if (is_dig(lexeme[0]))
			{
				int type = TOKEN_TYPE_INT;
				int index = 0;

				while (index < lexeme.length())
				{
					if (lexeme[index] == '.')
					{
						if (type == TOKEN_TYPE_INT)
							type = TOKEN_TYPE_FLT;
						else if (type == TOKEN_TYPE_FLT)
						{
							tk->error_info = std::string("float number has multiple '.' . Line: ") + std::string(std::to_string(tk->line_num));
							this->error_list.push_back(tk);
							return;
						}
						++index;
					}
					else if (is_dig(lexeme[index]))
					{
						++index;
					}
					else
					{
						tk->error_info = std::string("error number. Line: ") + std::string(std::to_string(tk->line_num));
						this->error_list.push_back(tk);
						return;
					}
				}
				tk->type = type;
			}
			else
			{
				auto r = this->root;
				int index = 0;

				while (index < lexeme.length())
				{
					if (r->next[(int)lexeme[index]] == nullptr)
					{
						tk->type == r->type;
						return;
					}
					else
					{
						r = r->next[(int)lexeme[index]];
					}
					++index;
				}
				tk->type = r->type;
			}
		}
	}
}

Token* Lexer::AddToken(int line)
{
	Token* tk = nullptr;
	this->token_stream.push_back(Token());
	tk = &this->token_stream.back();
	tk->line_num = line;
	return tk;
}

void Lexer::CreateTokenStream(const char* file)
{
	FILE*  pf = nullptr;
	fopen_s(&pf, file, "r");
	fseek(pf, 0, SEEK_END);
	int flen = ftell(pf);
	char* buf = new char[flen + 2];
	memset(buf, '\n', flen + 2);
	buf[flen + 1] = '\0';
	rewind(pf);
	fread(buf + 1, 1, flen, pf);
	fclose(pf);

	
	int index = 0;
	int line = 0;
	Token* tk = nullptr;
	while (index <= flen && buf[index] != EOF)
	{		
		if (buf[index] == '\n' || buf[index] == ' ' || buf[index] == '\t')
		{
			while (buf[index] == '\n' || buf[index] == ' ' || buf[index] == '\t')
			{
				if (buf[index] == '\n')
					++line;
				++index;
			}
			continue;
		}
		if (is_dig(buf[index]))
		{
			tk = AddToken(line);
			while (is_dig(buf[index]) || buf[index] == '.')
				tk->lexeme.push_back(buf[index++]);
			CheckToken(tk);
			continue;
		}
		if (is_alpha(buf[index]) || buf[index] == '_')
		{
			tk = AddToken(line);
			while (is_alpha(buf[index]) || buf[index] == '_' || is_dig(buf[index]))
				tk->lexeme.push_back(buf[index++]);
			CheckToken(tk);
			continue;
		}
		if (buf[index] == '+')
		{
			if (buf[index + 1] == '+')
			{
				tk = AddToken(line);
				tk->lexeme.push_back(buf[index]);
				tk->lexeme.push_back(buf[index + 1]);
				CheckToken(tk);
				index += 2;
				continue;
			}
		}
		if (buf[index] == '<')
		{
			if (buf[index + 1] == '=')
			{
				tk = AddToken(line);
				tk->lexeme.push_back(buf[index]);
				tk->lexeme.push_back(buf[index + 1]);
				CheckToken(tk);
				index += 2;
				continue;
			}
		}
		if (buf[index] == '>')
		{
			if (buf[index + 1] == '=')
			{
				tk = AddToken(line);
				tk->lexeme.push_back(buf[index]);
				tk->lexeme.push_back(buf[index + 1]);
				CheckToken(tk);
				index += 2;
				continue;
			}
		}
		if (buf[index] == '-')
		{
			if (buf[index + 1] == '-')
			{
				tk = AddToken(line);
				tk->lexeme.push_back(buf[index]);
				tk->lexeme.push_back(buf[index + 1]);
				CheckToken(tk);
				index += 2;
				continue;
			}
		}
		if (buf[index] == '=')
		{
			if (buf[index + 1] == '=')
			{
				tk = AddToken(line);
				tk->lexeme.push_back(buf[index]);
				tk->lexeme.push_back(buf[index + 1]);
				CheckToken(tk);
				index += 2;
				continue;
			}
		}
		if (buf[index] == '[' || buf[index] == ']' || buf[index] == '(' || buf[index] == ')' ||
			buf[index] == '{' || buf[index] == '}' || buf[index] == ':' || buf[index] == '>' ||
			buf[index] == '+' || buf[index] == '-' || buf[index] == '*' || buf[index] == '/' ||
			buf[index] == '=' || buf[index] == '%' || buf[index] == ',' || buf[index] == ';' ||
			buf[index] == '<' || buf[index] == '.' || buf[index] == '^')
		{
			tk = AddToken(line);
			tk->lexeme.push_back(buf[index]);
			CheckToken(tk);
			++index;
			continue;
		}
		if (buf[index] == '#')
		{
			while (buf[index] != '\n') ++index;
			continue;
		}
	}
	delete[] buf;

#ifdef _DEBUG
	for (auto tk : this->token_stream)
	{
		printf("%s\n", tk.lexeme.c_str());
	}
#endif // _DEBUG
}