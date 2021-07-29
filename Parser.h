#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <deque>
#include "LexicalAnalyzer.h"
#include <fstream>


using namespace std;


class Parser
{
public:
	void printTabs()
	{
		for (int i = 0; i < tabs; i++)
			fout << " :";
		fout << "|_";
	}
	void incrementTabs()
	{
		tabs++;
	}
	void decrementTabs()
	{
		tabs--;
	}
	Parser(vector<Token_Lexeme>v)
	{
		tabs = -1;
		int num_tokens = v.size();
		fout.open("parsetree.txt");
		parserSymbolTableFile.open("parser_symboltable.txt");
		threeCodeFile.open("tac.txt", ios::out | ios::trunc);
		threeCodeFile.close();
		translatorSymbolTableFile.open("translator_symboltable.txt");


		currIdentifierType = "";

		if (!fout)
		{
			exit(-1);
		}
		for (int i = 0; i < num_tokens; i++)
		{
			Token_Lexeme tl(v[i].tok, v[i].lexeme);
			tokens.push_back(tl);
		}
		lineNumber = 1;
		relativeAddress = 0;
		currIdentifierSize = 1;
		varNo = 1;

	}
	~Parser()
	{
		;
	}
	void checkTokens()
	{
		look = nextTok();
		Start();
		if (!tokens.empty())
			fout << "Irrelevant tokens at the end" << endl;
		else
		{
			fout << "No Error found by Parser" << endl;
		}

	}

private:
	deque<Token_Lexeme> tokens;
	int look;
	Token_Lexeme currToken;
	int tabs;
	ofstream fout;
	ofstream parserSymbolTableFile;
	ofstream translatorSymbolTableFile;
	ofstream threeCodeFile;
	string currIdentifierType;
	int lineNumber ;
	int relativeAddress;
	int varNo;
	int currIdentifierSize;


	void backPatch(int num)
	{
		vector<string> vecOfStrs;
 		std::ifstream in("tac.txt", ios::in);
		std::string str;

		while (std::getline(in, str))
		{
			// Line contains string of length > 0 then save it in vector
			if (str.size() > 0)
				vecOfStrs.push_back(str);
		}
		in.close();
		
		std::ofstream out("tac.txt", ios::out);
		int count = 1;
		for (auto it = vecOfStrs.begin(); it != vecOfStrs.end(); it++)
		{
			if (num == count)
			{
				out << *it + " "+ to_string(lineNumber) + "\n";
			}
			else
			{
				out << *it + "\n";
			}
			
			count++;
		}

	}
	string get_new_var()
	{
		string v = "var" + to_string(varNo);
		varNo++;
		return v;
	}
	void write_To_TAC_File(string str)
	{
		threeCodeFile.open("tac.txt", ios::app);
		threeCodeFile << str;
		threeCodeFile.close();
		lineNumber++;
	}
	void write_to_translator_symbol_table(string str)
	{
		translatorSymbolTableFile << str;
	}

	int  nextTok()
	{
		if (!tokens.empty())
		{
			currToken = tokens.front();
			tokens.pop_front();
			return currToken.tok;
		}
		return -1;
	}
	void match(int tok) {
		if (look == tok)
			look = nextTok();
		else
		{
			fout << "Bad token ..." << endl;
			cout << "\n\n\nBad token ..." << endl;
			exit(-1);
		}

	}
	void toPrint()
	{
		incrementTabs();
		printTabs();
		if (look == IDENTIFIER)
		{
			fout << "ID : " << currToken.lexeme << endl;
			write_To_TAC_File("out " + currToken.lexeme + "\n");
			match(IDENTIFIER);
		}
		else if (look == STRING_)
		{
			fout << "String : " << currToken.lexeme << endl;
			write_To_TAC_File("out " + currToken.lexeme + "\n");
			match(STRING_);

		}
		else
			fout << "Bad token ..." << endl;
		decrementTabs();

	}
	void comment()
	{
		incrementTabs();
		printTabs();
		if (look == COMMENT)
		{
			fout << currToken.lexeme << endl;
			match(COMMENT);
		}
		else
			fout << "Bad token ..." << endl;
		decrementTabs();
	}
	void A()
	{
		incrementTabs();
		printTabs();
		if (look == COMMA)
		{
			fout << "," << endl;
			match(COMMA);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;
			match(IDENTIFIER);

			printTabs();
			fout << "A" << endl;
			A();

		}
		else
			fout << "^" << endl;
		decrementTabs();
	}
	string var()
	{
		incrementTabs();
		printTabs();
		string str = currToken.lexeme;

		if (look == IDENTIFIER)
		{
			fout << "ID : " << currToken.lexeme << endl;
			match(IDENTIFIER);
		}
		else if (look == NUM)
		{
			str = get_new_var();
			translatorSymbolTableFile << str << "," << "Integer" << "," << relativeAddress << "," << currToken.lexeme << endl;
			relativeAddress++;

			fout << "NUM : " << currToken.lexeme << endl;
			match(NUM);
		}
		else if (look == LITERAL_)
		{
			fout << "LITERAL : " << currToken.lexeme << endl;
			match(LITERAL_);
		}
		else
			fout << "Bad token ..." << endl;
		decrementTabs();
		return str;
	}
	string OP()
	{
		incrementTabs();
		printTabs();
		string str = "";
		if (look == LESS_THAN)
		{
			fout << "<" << endl;
			str = "<";
			match(LESS_THAN);
		}
		else if (look == GREATER_THAN)
		{
			fout << ">" << endl;
			str = ">";
			match(GREATER_THAN);
		}
		else if (look == LESS_OR_EQUAL)
		{
			fout << "<=" << endl;
			str = "<=";
			match(LESS_OR_EQUAL);
		}
		else if (look == GREATER_OR_EQUAL)
		{
			fout << ">=" << endl;
			str = ">=";
			match(GREATER_OR_EQUAL);
		}
		else if (look == EQUAL)
		{
			fout << "=" << endl;
			str = "=";
			match(EQUAL);
		}
		else if (look == NOT_EQUAL)
		{
			fout << "/=" << endl;
			str = "/=";
			match(NOT_EQUAL);
		}
		else
			fout << "Bad token ..." << endl;
		decrementTabs();
		return str;
	}
	string BE()
	{
		incrementTabs();

		printTabs();
		fout << "var" << endl;
		string v1 = var();

		printTabs();
		fout << "OP" << endl;
		string op = OP();

		printTabs();
		fout << "var" << endl;
		string v2 = var();

		decrementTabs();
		return v1 + " " + op + " " + v2;
	}

	void elif_stmt()
	{
		incrementTabs();
		printTabs();

		if (look == ELIF)
		{
			fout << "elif" << endl;
			match(ELIF);

			printTabs();
			fout << "BE" << endl;

			string boolenEx = BE();

			int BE_t = lineNumber;
			write_To_TAC_File("elif " + boolenEx + " goto\n");
			int BE_f = lineNumber;
			write_To_TAC_File("goto\n");

			backPatch(BE_t);


			printTabs();
			fout << ":" << endl;
			match(DECLARATION);

			printTabs();
			fout << "{" << endl;
			match(LEFT_CURLY_BRACKET);

			printTabs();
			fout << "Statement" << endl;
			Statement();

			printTabs();
			fout << "}" << endl;
			match(RIGHT_CURLY_BRACKET);

			int BE_l = lineNumber;
			write_To_TAC_File("goto\n");

			backPatch(BE_f);

			printTabs();
			fout << "elif-stmt" << endl;
			elif_stmt();

			

			backPatch(BE_l);

		}
		else
		{
			
			fout << "else-stmt" << endl;
			else_stmt();
		}
			
		decrementTabs();

	}
	void else_stmt()
	{
		incrementTabs();
		printTabs();

		if (look == ELSE)
		{
			fout << "else" << endl;
			match(ELSE);



			printTabs();
			fout << "{" << endl;
			match(LEFT_CURLY_BRACKET);

			printTabs();
			fout << "Statement" << endl;
			Statement();

			printTabs();
			fout << "}" << endl;
			match(RIGHT_CURLY_BRACKET);


		}
		else
			fout << "^" << endl;

		decrementTabs();
	}
	void R()
	{
		incrementTabs();
		printTabs();
		if (look == COMMA)
		{
			fout << "," << endl;
			match(COMMA);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;
			parserSymbolTableFile << currToken.lexeme << "," << currIdentifierType << endl;
			translatorSymbolTableFile << currToken.lexeme << "," << currIdentifierType << "," << relativeAddress << endl;
			relativeAddress += currIdentifierSize;
			match(IDENTIFIER);

			printTabs();
			fout << "R" << endl;
			R();
		}
		else
		{
			fout << "^" << endl;

		}
		decrementTabs();

	}
	string F()
	{
		incrementTabs();
		printTabs();
		string str;
		if (look == IDENTIFIER)
		{
			str = currToken.lexeme;
			fout << "ID : " << currToken.lexeme << endl;
			match(IDENTIFIER);
			
		}
		else if (look == NUM)
		{

			//str = currToken.lexeme;
			str = get_new_var();
			translatorSymbolTableFile << str << "," << currIdentifierType << "," << relativeAddress << ","<< currToken.lexeme <<  endl;
			relativeAddress++;
			
			fout << "NUM : " << currToken.lexeme << endl;
			match(NUM);
			
		}
		else if (look == LEFT_PARANTHESIS)
		{
			fout << "(" << endl;
			match(LEFT_PARANTHESIS);

			printTabs();
			fout << "E" << endl;
			str = E();

			printTabs();
			fout << ")" << endl;
			match(RIGHT_PARANTHESIS);
			
		}
		else
		{
			fout << "Bad token ..." << endl;
			cout << "\n\n\nBad token ..." << endl;

			exit(1);
		}
		decrementTabs();
		return str;
	}
	string T_prime(string param)
	{
		incrementTabs();
		printTabs();
		string str = "";
		if (look == MULTIPLICATION) {

			string var = get_new_var();

			translatorSymbolTableFile << var << "," << "Integer" << "," << relativeAddress << "," << 0 << endl;
			relativeAddress++;

			fout << "*" << endl;
			match(MULTIPLICATION);

			printTabs();
			fout << "F" << endl;
			string Fn = F();

			write_To_TAC_File(var + " = " + param + " * " + Fn + "\n");

			printTabs();
			fout << "T_prime" << endl;
			str = T_prime(var);
		}
		else if (look == DIVISION)
		{
			string var = get_new_var();

			translatorSymbolTableFile << var << "," << "Integer" << "," << relativeAddress << "," << 0 << endl;
			relativeAddress++;

			fout << "/" << endl;
			match(DIVISION);

			printTabs();
			fout << "F" << endl;
			string Fn = F();

			write_To_TAC_File(var + " = " + param + " / " + Fn + "\n");


			printTabs();
			fout << "T_prime" << endl;
			str = T_prime(var);
		}
		else {
			fout << "^" << endl;
			str =  param;
		}
		decrementTabs();
		return str;
	}
	string T()
	{
		incrementTabs();

		printTabs();
		fout << "F" << endl;
		string Fn = F();

		printTabs();
		fout << "T_prime" << endl;
		string T_prime_n = T_prime(Fn);

		decrementTabs();
		return T_prime_n;
	}
	string E_prime(string param) {
		incrementTabs();
		printTabs();
		string str = "";
		if (look == PLUS) {

			string var = get_new_var();

			translatorSymbolTableFile << var << "," << "Integer" << "," << relativeAddress << "," << 0 << endl;
			relativeAddress++;

			fout << "+" << endl;
			match(PLUS);

			printTabs();
			fout << "T" << endl;
			string Tn = T();

			write_To_TAC_File(var + " = " + param + " + " + Tn + "\n");

			printTabs();
			fout << "E_prime" << endl;
			str = E_prime(var);
			
		}
		else if (look == MINUS)
		{

			string var = get_new_var();
			translatorSymbolTableFile << var << "," << "Integer" << "," << relativeAddress << "," << 0 << endl;
			relativeAddress++;

			fout << "-" << endl;
			match(MINUS);

			printTabs();
			fout << "T" << endl;
			string Tn = T();

			write_To_TAC_File(var + " = " + param + " - " + Tn + "\n");

			printTabs();
			fout << "E_prime" << endl;
			str = E_prime(var);
		}
		else {
			fout << "^" << endl;
			str = param;
		}
		decrementTabs();
		return str;
	}

	string E()
	{
		incrementTabs();

		printTabs();
		fout << "T" << endl;

		//string var = get_new_var();

		string Tn = T();

		printTabs();
		fout << "E_prime" << endl;
		string E_prime_n = E_prime(Tn);

		//write_To_TAC_File(var + " = " + E_prime_n + "\n");

		decrementTabs();

		return E_prime_n;
	}
	string L()
	{
		incrementTabs();
		printTabs();
		string En = "";
		if (look == LITERAL_)
		{
			En = currToken.lexeme;
			fout << "LITERAL : " << currToken.lexeme << endl;
			match(LITERAL_);
		}
		else
		{
			fout << "E" << endl;
			En = E();
		}
		decrementTabs();
		return En;
	}
	void dataType()
	{
		incrementTabs();
		printTabs();
		if (look == INTEGER) {
			fout << "Integer" << endl;
			match(INTEGER);
		}
		else if (look == CHAR)
		{
			fout << "char" << endl;
			match(CHAR);
		}
		else
		{
			fout << "Bad Token ..." << endl;
		}
		decrementTabs();

	}
	void Z()
	{
		incrementTabs();
		printTabs();

		if (look == COMMA)
		{
			fout << ", " << endl;
			match(COMMA);

			printTabs();
			if (look == INTEGER) {
				fout << "Integer" << endl;
				match(INTEGER);

				printTabs();
				fout << ":" << endl;
				match(DECLARATION);

				printTabs();
				fout << "ID: " << currToken.lexeme << endl;
				match(IDENTIFIER);

				printTabs();
				fout << "Z" << endl;
				Z();


			}
			else if (look == CHAR)
			{
				fout << "char" << endl;
				match(CHAR);

				printTabs();
				fout << ":" << endl;
				match(DECLARATION);

				printTabs();
				fout << "ID: " << currToken.lexeme << endl;
				match(IDENTIFIER);

				printTabs();
				fout << "Z" << endl;
				Z();
			}
			else
			{
				fout << "^" << endl;
			}
		}
		else
		{
			fout << "^" << endl;
		}
	}
	void Param()
	{
		incrementTabs();
		printTabs();

		if (look == INTEGER) {
			fout << "Integer" << endl;
			match(INTEGER);

			printTabs();
			fout << ":" << endl;
			match(DECLARATION);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;
			match(IDENTIFIER);

			printTabs();
			fout << "Z" << endl;
			Z();


		}
		else if (look == CHAR)
		{
			fout << "char" << endl;
			match(CHAR);

			printTabs();
			fout << ":" << endl;
			match(DECLARATION);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;
			match(IDENTIFIER);

			printTabs();
			fout << "Z" << endl;
			Z();
		}
		else
		{
			fout << "^" << endl;
		}


		decrementTabs();

	}
	void more_args()
	{
		incrementTabs();

		printTabs();
		if (look == COMMA)
		{
			fout << "," << endl;
			match(COMMA);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;
			match(IDENTIFIER);

			printTabs();
			fout << "more_Args: " << endl;
			more_args();
		}
		else
		{
			fout << "^" << endl;
		}


		decrementTabs();

	}
	void args()
	{
		incrementTabs();
		printTabs();
		if (look == IDENTIFIER)
		{
			fout << "ID: " << currToken.lexeme << endl;
			match(IDENTIFIER);

			printTabs();
			fout << "more_Args: " << endl;
			more_args();
		}
		else
		{
			fout << "^" << endl;
		}


		decrementTabs();

	}
	void Statement()
	{
		incrementTabs();
		printTabs();

		if (look == PRINT)
		{
			fout << "PRINT" << endl;
			match(PRINT);


			printTabs();
			fout << "(" << endl;
			match(LEFT_PARANTHESIS);

			printTabs();
			fout << "ToPrint" << endl;
			toPrint();


			printTabs();
			fout << ")" << endl;
			match(RIGHT_PARANTHESIS);

			printTabs();
			fout << ";" << endl;
			match(SEMICOLON);

			printTabs();
			fout << "Statement" << endl;
			Statement();
		}
		else if (look == PRINTLN)
		{
			fout << "PRINTLN" << endl;
			match(PRINTLN);


			printTabs();
			fout << "(" << endl;
			match(LEFT_PARANTHESIS);



			printTabs();
			fout << "ToPrint" << endl;
			toPrint();


			printTabs();
			fout << ")" << endl;
			match(RIGHT_PARANTHESIS);

			printTabs();
			fout << ";" << endl;
			match(SEMICOLON);

			printTabs();
			fout << "Statement" << endl;
			Statement();

		}
		else if (look == COMMENT)
		{
			fout << "COMMENT" << endl;
			comment();

			printTabs();
			fout << "Statement" << endl;
			Statement();

		}
		else if (look == IN)
		{
			fout << "In" << endl;
			match(IN);

			printTabs();
			fout << "<<" << endl;
			match(INPUT);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;

			write_To_TAC_File("in " + currToken.lexeme + "\n");

			match(IDENTIFIER);

			printTabs();
			fout << "A" << endl;
			A();

			printTabs();
			fout << ";" << endl;
			match(SEMICOLON);

			printTabs();
			fout << "Statement" << endl;
			Statement();

		}
		else if (look == WHILE)
		{
			fout << "while" << endl;
			match(WHILE);

			printTabs();
			fout << "BE" << endl;

			string boolenEx = BE();

			int BE_t = lineNumber;
			write_To_TAC_File("if " + boolenEx + " goto\n");
			int BE_f = lineNumber;
			write_To_TAC_File("goto\n");

			backPatch(BE_t);

			printTabs();
			fout << ":" << endl;
			match(DECLARATION);

			printTabs();
			fout << "{" << endl;
			match(LEFT_CURLY_BRACKET);

			printTabs();
			fout << "Statement" << endl;
			Statement();

			printTabs();
			fout << "}" << endl;
			match(RIGHT_CURLY_BRACKET);

			
			write_To_TAC_File("goto " + to_string(BE_t) + "\n");

			backPatch(BE_f);

			printTabs();
			fout << "Statement" << endl;
			Statement();
		}
		else if (look == IF)
		{
			fout << "if" << endl;
			match(IF);

			printTabs();
			fout << "BE" << endl;
			string boolenEx = BE();

			int BE_t = lineNumber;
			write_To_TAC_File("if " + boolenEx + " goto\n");
			int BE_f = lineNumber;
			write_To_TAC_File("goto\n");

			backPatch(BE_t);


			printTabs();
			fout << ":" << endl;
			match(DECLARATION);

			printTabs();
			fout << "{" << endl;
			match(LEFT_CURLY_BRACKET);

			printTabs();
			fout << "Statement" << endl;
			Statement();


			printTabs();
			fout << "}" << endl;
			match(RIGHT_CURLY_BRACKET);
			
			int BE_l = lineNumber;
			write_To_TAC_File("goto\n");

			backPatch(BE_f);

			printTabs();
			fout << "elif-stmt" << endl;
			elif_stmt();

			backPatch(BE_l);

			printTabs();
			fout << "Statement" << endl;
			Statement();
		}
		else if (look == INTEGER) {
			currIdentifierType = "Integer";
			fout << "Integer" << endl;
			match(INTEGER);

			printTabs();
			fout << ":" << endl;
			match(DECLARATION);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;
			currIdentifierSize = 1;
			parserSymbolTableFile << currToken.lexeme << "," << currIdentifierType << endl;
			translatorSymbolTableFile << currToken.lexeme << "," << currIdentifierType << "," << relativeAddress << endl;
			relativeAddress += currIdentifierSize;
			
			match(IDENTIFIER);

			printTabs();
			fout << "R" << endl;
			R();

			printTabs();
			fout << ";" << endl;
			match(SEMICOLON);

			printTabs();
			fout << "Statement" << endl;
			Statement();

		}
		else if (look == CHAR) {
			currIdentifierType = "char";

			fout << "Char" << endl;
			match(CHAR);

			printTabs();
			fout << ":" << endl;
			match(DECLARATION);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;
			currIdentifierSize = 1;
			parserSymbolTableFile << currToken.lexeme << "," << currIdentifierType << endl;
			translatorSymbolTableFile << currToken.lexeme << "," << currIdentifierType << "," << relativeAddress << endl;
			relativeAddress += currIdentifierSize;
			match(IDENTIFIER);

			printTabs();
			fout << "R" << endl;
			R();

			printTabs();
			fout << ";" << endl;
			match(SEMICOLON);

			printTabs();
			fout << "Statement" << endl;
			Statement();

		}
		else if (look == IDENTIFIER)
		{
			fout << "ID: " << currToken.lexeme << endl;

			string myID = currToken.lexeme;

			match(IDENTIFIER);
			if (look == ASSIGNMENT)
			{
				printTabs();
				fout << ":=" << endl;
				match(ASSIGNMENT);

				printTabs();
				fout << "L" << endl;
				string myE = L();
				write_To_TAC_File(myID + " = "+ myE + "\n");


				printTabs();
				fout << ";" << endl;
				match(SEMICOLON);

				printTabs();
				fout << "Statement" << endl;
				Statement();
			}
			else if (look == LEFT_PARANTHESIS)
			{
				printTabs();
				fout << "(" << endl;
				match(LEFT_PARANTHESIS);

				printTabs();
				fout << "args" << endl;
				args();

				printTabs();
				fout << ")" << endl;
				match(RIGHT_PARANTHESIS);

				printTabs();
				fout << ";" << endl;
				match(SEMICOLON);

				printTabs();
				fout << "Statement" << endl;
				Statement();

			}
		}
		else if (look == RET)
		{
			fout << "ret" << endl;
			match(RET);

			printTabs();
			fout << "L" << endl;
			L();

			printTabs();
			fout << ";" << endl;
			match(SEMICOLON);
		}

		else
		{
			fout << "^" << endl;
		}
		decrementTabs();
	}

	void S()
	{
		incrementTabs();
		if (look == FUNC)
		{
			printTabs();
			fout << "func" << endl;
			match(FUNC);

			printTabs();
			fout << "dataType" << endl;
			dataType();

			printTabs();
			fout << ":" << endl;
			match(DECLARATION);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;
			match(IDENTIFIER);

			printTabs();
			fout << "(" << endl;
			match(LEFT_PARANTHESIS);

			printTabs();
			fout << "Param" << endl;
			Param();

			printTabs();
			fout << ")" << endl;
			match(RIGHT_PARANTHESIS);


			printTabs();
			fout << "{" << endl;
			match(LEFT_CURLY_BRACKET);

			printTabs();
			fout << "Statement" << endl;
			Statement();

			printTabs();
			fout << "}" << endl;
			match(RIGHT_CURLY_BRACKET);

			printTabs();
			fout << "Statement" << endl;
			Statement();

			S();
		}
		else
		{
			printTabs();
			fout << "^" << endl;
		}

		decrementTabs();
	}
	void Start()
	{
		incrementTabs();
		if (look == FUNC)
		{
			printTabs();
			fout << "func" << endl;
			match(FUNC);

			printTabs();
			fout << "dataType" << endl;
			dataType();

			printTabs();
			fout << ":" << endl;
			match(DECLARATION);

			printTabs();
			fout << "ID: " << currToken.lexeme << endl;
			match(IDENTIFIER);

			printTabs();
			fout << "(" << endl;
			match(LEFT_PARANTHESIS);

			printTabs();
			fout << "Param" << endl;
			Param();

			printTabs();
			fout << ")" << endl;
			match(RIGHT_PARANTHESIS);


			printTabs();
			fout << "{" << endl;
			match(LEFT_CURLY_BRACKET);

			printTabs();
			fout << "Statement" << endl;
			Statement();

			printTabs();
			fout << "}" << endl;
			match(RIGHT_CURLY_BRACKET);

			printTabs();
			fout << "Statement" << endl;
			Statement();

			printTabs();
			fout << "S" << endl;
			S();
		}
		else
		{
			fout << "Statement" << endl;
			Statement(); S();
		}
		decrementTabs();

	}
};

#endif
