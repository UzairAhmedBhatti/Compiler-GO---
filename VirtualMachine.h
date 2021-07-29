#pragma once

#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>

using namespace std;

enum Opcode {
	IN_OP = 1,
	OUT = 2,
	GOTO = 3,


	LT = 4,
	LE = 5,
	GT = 6,
	GE = 7,
	E = 8,
	NE = 9,
	
	ASSIGNMENT_OP = 10,
	ADD = 11,
	SUB = 12,
	MUL = 13,
	DIV = 14
};


class VM
{
public:
	VM();
	~VM();
	string tokenToOpcode(string token)
	{
		if (token == "=")
		{
			return to_string(E);
		}
		else if (token == "/=")
		{
			return to_string(NE);
		}
		else if (token == ">")
		{
			return to_string(GT);
		}
		else if (token == ">=")
		{
			return to_string(GE);
		}
		else if (token == "<")
		{
			return to_string(LT);
		}
		else if (token == "<=")
		{
			return to_string(LE);
		}
		else if (token == "+")
		{
			return to_string(ADD);
		}
		else if (token == "-")
		{
			return to_string(SUB);
		}
		else if (token == "*")
		{
			return to_string(MUL);
		}
		else if (token == "/")
		{
			return to_string(DIV);
		}
		
	}
	vector<string> removeDupWord(string myText, char delimeter)
	{
		vector <string> v;
		std::istringstream iss(myText);
		std::string token;
		while (std::getline(iss, token, delimeter))
		{
			//std::cout << token << std::endl;
			v.push_back(token);
		}
		return v;
	}
	void readSymbolTable(string filename)
	{
		ifstream fin(filename);
		if (!fin)
		{
			cout << "Error while opening the file" << endl;
		}
		else
		{
			string tp;
			while (getline(fin, tp)) { //read data from file object and put it into string.
				vector<string> v = removeDupWord(tp, ',');
				string varName = v[0];
				string varType = v[1];
				string varAddr = v[2];
			
				if (v.size() == 3)
				{
					addressMap[varName] = varAddr;
				}
				else if (v.size() == 4)
				{
					addressMap[varName] = varAddr;
					initalValueMap[varName] = v[3];
				}
			}
		}
	}
	void printTable()
	{
		for (auto& it : addressMap) {
			// Do stuff
			cout << it.first << " " << it.second << endl;
		}
		for (auto& it : initalValueMap) {
			// Do stuff
			cout << it.first << " " << it.second << endl;

		}
	}
	void readMachineCode(string filename)
	{
		int count = 0;
		string line;
		ifstream file(filename);
		while (getline(file, line))
			count++;
		file.close();

		quadSize = count;

		quad = new int* [count];
		for (int i = 0; i < count; i++)
		{
			quad[i] = new int[instructionBytes];
		}
		for (int i = 0; i < quadSize; i++)
		{
			for (int j = 0; j < instructionBytes; j++)
			{
				quad[i][j] = -1;
			}
		}
		

		ifstream fin(filename);
		if (!fin)
		{
			cout << "Error while opening the file" << endl;
		}
		else
		{
			string tp;
			int i = 0;
			int lineNo = 0;
			while (getline(fin, tp)) { //read data from file object and put it into string.
				vector<string> v = removeDupWord(tp, ',');
				i = 0;
				while (i < v.size())
				{
					quad[lineNo][i] = stoi(v[i]);
					i++;
				}
				lineNo++;
			}
		}
		//printQuad();
	}
	void printQuad()
	{
		for (int i = 0; i < quadSize; i++)
		{
			for (int j = 0; j < instructionBytes; j++)
			{
				cout << quad[i][j] << " ";
			}
			cout << endl;
		}
	}
	void writeToMachineCodeFile(string str)
	{
		ofstream fout("machinecode.txt", ios::app);
		fout << str;
		fout.close();
	}
	void covertTACfile(string filename)
	{
		
		ifstream fin(filename);
		if (!fin)
		{
			cout << "Error while opening the file" << endl;
		}
		else
		{
			string tp;
			while (getline(fin, tp)) { //read data from file object and put it into string.
				vector<string> v = removeDupWord(tp, ' ');
				string firstTok = v[0];
				if (firstTok == "in")
				{
					writeToMachineCodeFile(to_string(IN_OP) + "," + addressMap[v[1]]+ "\n");
				}
				else if (firstTok == "out")
				{
					writeToMachineCodeFile(to_string(OUT) + "," + addressMap[v[1]] + "\n");
				}
				else if (firstTok == "if" || firstTok == "elif")
				{
					writeToMachineCodeFile(tokenToOpcode(v[2]) + "," + addressMap[v[1]] + "," + addressMap[v[3]] + "," + v[5] + "\n");
				}
				else if (firstTok == "goto")
				{
					writeToMachineCodeFile(to_string(GOTO) + "," + v[1] + "\n");
				}
				else
				{
					if (addressMap.find(firstTok) == addressMap.end())
					{
						cout << "Variable Used without declaring" << endl;
					}
					else
					{
						if (v.size() == 3)
						{
							writeToMachineCodeFile(to_string(ASSIGNMENT_OP) + "," + addressMap[v[2]] + "," + addressMap[v[0]] + "\n");
						}
						else
						{
							writeToMachineCodeFile(tokenToOpcode(v[3]) + "," + addressMap[v[2]] + "," + addressMap[v[4]] + "," + addressMap[v[0]] + "\n");
						}
					}
					
				}
			}
		}
	}
	void initializeArray()
	{
		ds = new int[addressMap.size()];
		for (int i = 0; i < addressMap.size(); i++)
			ds[i] = 0;

		cout << endl;
		
		for (auto& it : initalValueMap) {
			//cout << it.first << " " << it.second << endl;
			string index = addressMap[it.first];
			
			ds[stoi(index)] = stoi(it.second);
		
		}
		cout << endl;
		/*for (int i = 0; i < addressMap.size(); i++)
			cout << ds[i] << " ";
		cout << endl;*/
	}
	string getVariableName(int i)
	{
		for (auto& it : addressMap) {
			if(stoi(it.second) == i)
				return it.first ;
		}
	}
	void executeInstructions()
	{
		cout << endl;
		cout << "Program output" << endl;
		int i,j,k;

		for (int pc = 0; pc < quadSize; pc++)
		{
			switch (quad[pc][0])
			{
				case IN_OP:
					k = quad[pc][1];
					cout << "Input number: ";
					cin >> ds[k];
					break;
				case OUT:
					k = quad[pc][1];
					cout << getVariableName(k) << ": " << ds[k] << endl;
					break;
				case GOTO:
					pc = quad[pc][1] - 2;
					break;
				case SUB:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					ds[k] = ds[i] - ds[j];
					break;
				case ADD:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					ds[k] = ds[i] + ds[j];
					break;
				case MUL:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					ds[k] = ds[i] * ds[j];
					break;
				case DIV:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					ds[k] = ds[i] / ds[j];
					break;
				case ASSIGNMENT_OP:
					i = quad[pc][1];
					j = quad[pc][2];
					ds[j] = ds[i];
					break;
				case GE:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					if (ds[i] >= ds[j])
						pc = k - 2;
					break;
				case GT:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					if (ds[i] > ds[j])
						pc = k - 2;
					break;

				case LT:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					if (ds[i] < ds[j])
						pc = k - 2;
					break;
				case LE:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					if (ds[i] <= ds[j])
						pc = k - 2;
					break;

				case E:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					if (ds[i] == ds[j])
						pc = k - 2;
					break;

				case NE:
					i = quad[pc][1];
					j = quad[pc][2];
					k = quad[pc][3];
					if (ds[i] != ds[j])
						pc = k - 2;
					break;
				default:
					cout << "default case" << endl;
					break;
			}
		}
	}
	void printSymbolTable()
	{
		cout << endl;
		cout << "Variable values after execution of machine code" << endl;
		for (auto& it : addressMap) {
			//cout << it.first << " " << it.second << endl;
			int index = stoi(it.second);

			cout << it.first << " " << ds[index] << endl;

		}
		cout << endl;
		/*for (int i = 0; i < addressMap.size(); i++)
			cout << ds[i] << " ";
		cout << endl;*/
	}
	void execute()
	{
		readSymbolTable("translator_symboltable.txt");
		covertTACfile("tac.txt");
		readMachineCode("machinecode.txt");
		initializeArray();
		//printSymbolTable();
		executeInstructions();
		//printSymbolTable();
	}
private:
	int** quad;
	int instructionsCount;
	unordered_map<string, string> addressMap;
	unordered_map<string, string> initalValueMap;
	int quadSize;
	int instructionBytes = 4;
	int* ds ;
};

VM::VM()
{
	instructionsCount = 0;
	quad = NULL;
	quadSize = 0;
	ofstream fout("machinecode.txt");
	fout.close();
	ds = NULL;
}

VM::~VM()
{
}

#endif