#include "LexicalAnalyzer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Parser.h"
#include "VirtualMachine.h"
using namespace std;

void writeTokensToFile(vector<Token_Lexeme>v, string filename)
{
	ofstream fout(filename+".txt");
	if (!fout)
	{
		cout << "Error while opening file" << endl;
	}
	else
	{
		unordered_map<int, string> tokens_enum_map;
		tokens_enum_map[int(';')] = ";";
		tokens_enum_map[int('[')] = "[";
		tokens_enum_map[int(']')] = "]";
		tokens_enum_map[int('{')] = "{";
		tokens_enum_map[int('}')] = "}";
		tokens_enum_map[int('(')] = "(";
		tokens_enum_map[int(')')] = ")";
		tokens_enum_map[int(',')] = ",";
		tokens_enum_map[int('+')] = "+";
		tokens_enum_map[int('-')] = "-";
		tokens_enum_map[int('*')] = "*";
		tokens_enum_map[int('/')] = "/";

		tokens_enum_map[1] = ":=";
		tokens_enum_map[2] = ":";


		tokens_enum_map[3] = "ID";
		tokens_enum_map[4] = "PRINT";
		tokens_enum_map[5] = "PRINTLN";

		tokens_enum_map[6] = "IF";
		tokens_enum_map[7] = "ELIF";
		tokens_enum_map[8] = "ELSE";

		tokens_enum_map[9] = "WHILE";
		tokens_enum_map[10] = "IN";
		tokens_enum_map[11] = "INTEGER";

		tokens_enum_map[12] = "CHAR";
		tokens_enum_map[13] = "FUNC";

		tokens_enum_map[16] = "COMMENT";
		tokens_enum_map[17] = "STRING";
		tokens_enum_map[18] = "INPUT";
		tokens_enum_map[19] = "NUM";
		tokens_enum_map[20] = "LITERAL";

		tokens_enum_map[21] = "<";
		tokens_enum_map[22] = "<=";
		tokens_enum_map[23] = ">";
		tokens_enum_map[24] = ">=";
		tokens_enum_map[25] = "=";
		tokens_enum_map[26] = "/=";



		
		for (auto it = v.begin(); it != v.end(); it++)
		{
			/*fout << "(" << tokens_enum_map.find(it->tok)->second << " , ";
			if(it->lexeme != "")
				fout << it->lexeme << " )" << endl;
			else
				fout <<" ^ )" << endl;


			cout << "(" << tokens_enum_map.find(it->tok)->second << " , ";
			if (it->lexeme != "")
				cout << it->lexeme << " )" << endl;
			else
				cout << " ^ )" << endl;*/

			fout << "(" << it->tok << ", " << it->lexeme << ")" << endl;
			//cout << "(" << it->tok << ", " << it->lexeme << ")" << endl;


		}
	}
	fout.close();

}  

int main()
{
	LexicalAnalyzer l;
	string fileName;
	cout << "Enter the name of the file without the .go extension" << endl;
	cin >> fileName;
	fileName = fileName + ".go";

	l.readFile(fileName);
	vector<Token_Lexeme>v = l.getTokens();
	
	writeTokensToFile(v, "tokens");

	Parser p(v);
	p.checkTokens();
	
	VM vMachine;
	vMachine.execute();

	
}