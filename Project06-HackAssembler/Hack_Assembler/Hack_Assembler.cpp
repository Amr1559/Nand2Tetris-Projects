#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <bitset>
#include <unordered_map>
#include "Parser.h"
#include "Coder.h"
#include "SymbolTable.h"
using namespace std;

/*
Instrucions to build assembler

STAGED DEVELOPMENT
1. Develop basice assembler translates assembly without sympols
2. develop an ability to handle sympols (sympol table)
3. integerate two parts

PROPOSED SOFTWARE ARCHTECTURE
1. Parser: unpack instruction into fields
2. Code: translate each field into binary
3. SymbolTable: manage symbols
4. Main: intialize I/O and drives process
FOR MORE INFO look for Unit 6.5: Developing a Hack Assembler
*/


int main()
{
   Parser Program_Parser;
   Coder Program_Coder;
   SymbolTable program_symbol_table;

	ifstream inFile("AssemblyCode.txt");
	vector<string> Program_Instructions;
	string rawLine;

	// --- LOADING & CLEANING ---
	if (inFile.is_open())
	{
		while (getline(inFile, rawLine))
		{
			string line = Parser::cleanLine(rawLine);

			if (!line.empty())
				Program_Instructions.push_back(line);
		}
		inFile.close();
	}

	// --- PASS 1: Build Label Table ---
	program_symbol_table.firstPass(Program_Instructions);

	// --- PASS 2: Translate ---
	ofstream outFile("HackBinary.txt");

	for (const string& line : Program_Instructions)
	{
		if (line[0] == '(') // it is a labeel don't translate
			continue;

		if (line[0] == '@') {
			// Logic to handle Symbols or Numbers
			string symbolOrNum = line.substr(1); //remove '@'
			short finalValue;

			if (isdigit(symbolOrNum[0])) 
			{
				finalValue = stoi(symbolOrNum);
			}
			else
			{
				finalValue = program_symbol_table.getAddress(symbolOrNum);
			}

			ParsedAInstrucion Acommand;
			Acommand.value = finalValue;
			outFile << Program_Coder.Convert_A_instruction(Acommand) << endl;
		}
		else
		{
			outFile << Program_Coder.Convert_C_instruction(Program_Parser.ParseCInstruction(line)) << endl;
		}

	}

	cout << "Assembling Complete!\a\n";
}

