#pragma once

#ifndef LEXICALANALZER_H
#define LEXICALANALZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>


using namespace std;

enum Token {
	SEMICOLON = int(';'),
	LEFT_PARANTHESIS = int('('),
	RIGHT_PARANTHESIS = int(')'),
	LEFT_SQUARE_BRACKET = int('['),
	RIGHT_SQUARE_BRACKET = int(']'),
	LEFT_CURLY_BRACKET = int('{'),
	RIGHT_CURLY_BRACKET = int('}'),
	COMMA = int(','),
	ASSIGNMENT = 1,
	DECLARATION = 2,
	IDENTIFIER = 3,
	PRINT = 4,
	PRINTLN = 5,
	IF = 6,
	ELIF = 7,
	ELSE = 8,
	WHILE = 9,
	IN = 10,
	INTEGER = 11,
	CHAR = 12,
	FUNC = 13,
	ARITHMATIC_OPERATOR = 14,
	RELATIONAL_OPERATOR = 15,
	PLUS = int('+'),
	MINUS = int('-'),
	MULTIPLICATION = int('*'),
	DIVISION = int('/'),
	COMMENT = 16,
	STRING_ = 17,
	INPUT = 18,
	NUM = 19,
	LITERAL_ = 20,
	LESS_THAN = 21,
	LESS_OR_EQUAL = 22,
	GREATER_THAN = 23, 
	GREATER_OR_EQUAL = 24,
	EQUAL = 25,
	NOT_EQUAL = 26,
	RET = 27
};







struct Token_Lexeme
{
	int tok;
	string lexeme;
	Token_Lexeme(int tok_, string lexeme_)
	{
		tok = tok_;
		lexeme = lexeme_;
	}
	Token_Lexeme()
	{
		tok = 0;
		lexeme = "";
	}
};

class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	void readFile(string fileName);
	vector<Token_Lexeme> getTokens();
	Token_Lexeme getSingleToken();

private:
	string data;
	int index;
	bool badToken;
	Token_Lexeme checkKeyword(Token_Lexeme var);
};

#endif