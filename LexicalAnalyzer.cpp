#ifndef LEXICALANALZER_CPP
#define LEXICALANALZER_CPP

#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer()
{
	data = "";
	index = 0;
	badToken = false;
}

void LexicalAnalyzer::readFile(string fileName)
{
	ifstream file(fileName);
	if (!file)
	{
		cout << "Error while reading the file" << endl;
	}
	else
	{
		file.seekg(0, ios::end);
		int file_size = file.tellg();
		file.seekg(0, ios::beg);

		char* filedata = new char[file_size + 1];
		memset(filedata, '\0', file_size + 1);
		file.read(filedata, file_size);
		//filedata[file_size -2 ] = '$';
		


		std::string temp(filedata);

		data = temp;
		data = data + " \0";
		cout << data << endl;
	}
	file.close();
}

vector<Token_Lexeme> LexicalAnalyzer::getTokens()
{
	vector<Token_Lexeme> vt;
	while (1)
	{
		if (data[index] != '\0' && badToken == false)
		{
			Token_Lexeme t = getSingleToken();
			//cout << "Token: " << t.tok << " Lexeme: " << t.lexeme << endl;

			vt.push_back(t);
		}
		else
		{
			break;
		}
	}
	if (badToken == true)
		vt.clear();
	if (vt.size() > 0)
	{
		vt.pop_back();
	}
	
	return vt;
}

Token_Lexeme LexicalAnalyzer::checkKeyword(Token_Lexeme var)
{
	string lex = var.lexeme;
	Token_Lexeme tl;
	if (lex == "print")
	{
		tl.tok = PRINT;
		return tl;
	}
	else if (lex == "println")
	{
		tl.tok = PRINTLN;
		return tl;
	}
	else if (lex == "func")
	{
		tl.tok = FUNC;
		return tl;
	}
	else if (lex == "in")
	{
		tl.tok = IN;
		return tl;
	}
	else if (lex == "while")
	{
		tl.tok = WHILE;
		return tl;
	}
	else if (lex == "if")
	{
		tl.tok = IF;
		return tl;
	}
	else if (lex == "else")
	{
		tl.tok = ELSE;
		return tl;
	}
	else if (lex == "elif")
	{
		tl.tok = ELIF;
		return tl;
	}
	else if (lex == "Integer")
	{
		tl.tok = INTEGER;
		return tl;
	}
	else if (lex == "char")
	{
		tl.tok = CHAR;
		return tl;
	}
	else if (lex == "ret")
	{
		tl.tok = RET;
		return tl;
	}
	tl.tok = -1;
	return tl;
}


Token_Lexeme LexicalAnalyzer::getSingleToken()
{
	int state = 0;
	int staring_index = 0;
	int count_chars = 0;

	Token_Lexeme tl;
	for (; data[index] != '\0' && badToken == false; )
	{
		switch (state)
		{
		case 0:
			if (data[index] == ' ' || data[index] == '\n' || data[index] == '\t')
			{
				index++;
				break;

			}
			else
			{
				state = 1;
				break;
			}
		case 1:
			if (data[index] == ';')
			{

				index++;
				tl.tok = SEMICOLON;
				return tl;

			}
			else
			{
				state = 2;
				break;
			}
		case 2:
			if (data[index] == '(')
			{
				index++;
				tl.tok = LEFT_PARANTHESIS;
				return tl;

			}
			else
			{
				state = 3;
				break;
			}
		case 3:
			if (data[index] == ')')
			{
				index++;
				tl.tok = RIGHT_PARANTHESIS;
				return tl;
			}
			else
			{
				state = 4;
				break;
			}

		case 4:
			if (data[index] == '[')
			{
				index++;
				tl.tok = LEFT_SQUARE_BRACKET;
				return tl;
			}
			else
			{
				state = 5;
				break;
			}
		case 5:
			if (data[index] == ']')
			{
				index++;
				tl.tok = RIGHT_SQUARE_BRACKET;
				return tl;
			}
			else
			{
				state = 6;
				break;
			}
		case 6:
			if (data[index] == '{')
			{
				index++;
				tl.tok = LEFT_CURLY_BRACKET;
				return tl;
			}
			else
			{
				state = 7;
				break;
			}

		case 7:
			if (data[index] == '}')
			{
				index++;
				tl.tok = RIGHT_CURLY_BRACKET;
				return tl;
			}
			else
			{
				state = 8;
				break;
			}

		case 8:
			if (data[index] == ',')
			{
				index++;
				tl.tok = COMMA;
				return tl;
			}
			else
			{
				state = 9;
				break;
			}

		case 9:
			if (data[index] == ':')
			{
				index++;
				state = 10;
				break;
			}
			else
			{
				state = 11;
				break;
			}
		case 10:
			if (data[index] == '=')
			{
				index++;
				tl.tok = ASSIGNMENT;
				return tl;
			}
			else
			{

				tl.tok = DECLARATION;
				return tl;
			}

		case 11:
			if (isalpha(data[index]))
			{
				staring_index = index;
				index++;
				state = 12;

			}
			else
			{
				state = 13;
			}
			break;
		case 12:
			if (isalpha(data[index]) || isdigit(data[index]))
			{
				index++;
				break;
			}
			else
			{
				string lex = data.substr(staring_index, index - staring_index);
				tl.lexeme = lex;
				tl.tok = IDENTIFIER;
				Token_Lexeme keyw = checkKeyword(tl);
				if (keyw.tok != -1)
				{
					return keyw;
				}
				return tl;
			}
		case 13:
			if (data[index] == '+')
			{
				index++;
				tl.tok = PLUS;
				//tl.lexeme = PLUS;
				return tl;
			}
			else
			{
				state = 14;
				break;
			}
		case 14:
			if (data[index] == '-')
			{
				index++;
				tl.tok = MINUS;
				//tl.lexeme = MINUS;
				return tl;
			}
			else
			{
				state = 15;
				break;
			}

		case 15:
			if (data[index] == '*')
			{
				index++;
				tl.tok = MULTIPLICATION;
				//tl.lexeme = MULTIPLICATION;
				return tl;
			}
			else
			{
				state = 16;
				break;
			}

		case 16:
			if (data[index] == '/')
			{
				staring_index = index;
				index++;
				state = 17;
				break;
			}
			else
			{
				state = 20;
				break;
			}
		case 17:
			if (data[index] == '*')
			{
				index++;
				state = 18;
				break;
			}
			else if (data[index] == '=')
			{
				index++;
				tl.tok = NOT_EQUAL;
				//tl.lexeme = "/=";
				return tl;
			}
			else
			{
				index++;
				tl.tok = DIVISION;
				//tl.lexeme = DIVISION;
				return tl;
			}
		case 18:
			if (data[index] == '*')
			{
				state = 19;
			}
			index++;
			if (data[index] == '\0')
			{
				badToken = true;
				cout << endl << "Could not locate ending of comment" << endl;
			}
			break;
		case 19:
			if (data[index] == '/')
			{
				index++;
				string lex = data.substr(staring_index, index - staring_index);
				tl.lexeme = lex;
				tl.tok = COMMENT;

				return tl;

			}
			else
			{
				state = 18;
				break;
			}

		case 20:
			if (data[index] == '"')
			{
				state = 21;
				staring_index = index;
				index++;
				break;
			}
			else
			{
				state = 22;
				break;
			}
		case 21:
			if (data[index] == '"')
			{
				index++;
				tl.tok = STRING_;
				string lex = data.substr(staring_index, index - staring_index);
				tl.lexeme = lex;
				return tl;
			}
			else
			{
				index++;
				if (data[index] == '\0')
				{
					badToken = true;
					cout << endl << "Could not locate ending of string" << endl;
				}
				break;
			}
		case 22:
			if (data[index] == '<')
			{
				state = 23;
				index++;
				break;
			}
			else
			{
				state = 24;
				break;
			}
		case 23:
			if (data[index] == '=')
			{
				index++;
				tl.tok = LESS_OR_EQUAL;
				//tl.lexeme = "<=";
				return tl;
			}
			else
			{
				tl.tok = LESS_THAN;
				//tl.lexeme = "<";
				return tl;
			}

		case 24:
			if (data[index] == '>')
			{
				state = 25;
				index++;
				break;
			}
			else
			{
				state = 26;
				break;
			}
		case 25:
			if (data[index] == '>')
			{
				index++;
				tl.tok = INPUT;
				return tl;
			}
			else if (data[index] == '=')
			{
				index++;
				tl.tok = GREATER_OR_EQUAL;
				//tl.lexeme = ">=";
				return tl;
			}
			else
			{
				tl.tok = GREATER_THAN;
				//tl.lexeme = ">";
				return tl;
			}
		case 26:
			if (data[index] == '=')
			{
				index++;
				tl.tok = EQUAL;
				//tl.lexeme = "=";
				return tl;
			}
			else
			{
				state = 27;
				break;
			}
		case 27:
			if (data[index] == '\'')
			{
				staring_index = index;
				index++;
				state = 28;
				break;
			}
			else
			{
				state = 30;
				break;
			}
		case 28:
			index++;
			state = 29;
			break;

		case 29:
			if (data[index] == '\'')
			{
				index++;
				tl.tok = LITERAL_;
				string lex = data.substr(staring_index, index - staring_index);
				tl.lexeme = lex;
				return tl;
				
			}
			else
			{
				badToken = true;
				cout << endl << "Invalid Constant Literal token " << endl;
				break;
			}
		case 30:
			if (isdigit(data[index]))
			{
				staring_index = index;
				index++;
				state = 31;
				break;
			}
			else
			{
				cout << endl << "Unidentifiable Token" << endl;
				cout << "Token : " << data[index] << endl;
				badToken = true;
				break;
			}
		case 31:
			if (isdigit(data[index]))
			{
				
				index++;
				break;
			}
			else
			{
				if (isalpha(data[index]))
				{
					badToken = true;
					string lex = data.substr(staring_index, index+1 - staring_index);

					cout << endl << "Identifier can't start with a digit: "<< lex << endl;
					
				}
				string lex = data.substr(staring_index, index - staring_index);
				tl.lexeme = lex;
				tl.tok = NUM;

				return tl;
			}
		}
	}

	Token_Lexeme t;
	return t;
}

#endif