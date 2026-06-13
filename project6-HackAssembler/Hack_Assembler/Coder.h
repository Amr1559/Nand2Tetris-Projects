#pragma once
#include <bitset>
class Coder
{
private:
	map<string, string> dest_map = {
		{"null", "000"},
		{"M",    "001"},
		{"D",    "010"},
		{"MD",   "011"},
		{"A",    "100"},
		{"AM",   "101"},
		{"AD",   "110"},
		{"AMD",  "111"}
	};

	map<string, string> jump_map = {
		{"null", "000"},
		{"JGT",    "001"},
		{"JEQ",    "010"},
		{"JGE",   "011"},
		{"JLT",    "100"},
		{"JNE",   "101"},
		{"JLE",   "110"},
		{"JMP",  "111"}
	};

	std::map<std::string, std::string> comp_map = {
		// Constants (a-bit is 0)
		{"0",   "0101010"}, {"1",   "0111111"}, {"-1",  "0111010"},
		// D register (a-bit is 0)
		{"D",   "0001100"},
		// A register (a-bit is 0)
		{"A",   "0110000"},
		// M register (a-bit is 1)
		{"M",   "1110000"},
		// Negations
		{"!D",  "0001101"}, {"!A",  "0110001"}, {"!M",  "1110001"},
		{"-D",  "0001111"}, {"-A",  "0110011"}, {"-M",  "1110011"},
		// Increments/Decrements
		{"D+1", "0011111"}, {"A+1", "0110111"}, {"M+1", "1110111"},
		{"D-1", "0001110"}, {"A-1", "0110010"}, {"M-1", "1110010"},
		// Arithmetic
		{"D+A", "0000010"}, {"D+M", "1000010"},
		{"D-A", "0010011"}, {"D-M", "1010011"},
		{"A-D", "0000111"}, {"M-D", "1000111"},
		// Logical
		{"D&A", "0000000"}, {"D&M", "1000000"},
		{"D|A", "0010101"}, {"D|M", "1010101"}
	};

public:


	string Convert_A_instruction(ParsedAInstrucion command)
	{
		//check the hack limit
		if (command.value > 32767 || command.value < 0)
		{
			cout << "Error: Value " << command.value << " exceeds Hack limit.\a\n";
		}
		
		bitset<16> binaryVal(command.value);
		return binaryVal.to_string();
	}

	string Convert_C_instruction(ParsedCInstrucion command)
	{
		string d_bits = dest_map[command.dest];
		string c_bits = comp_map[command.comp];
		string j_bits = jump_map[command.jump];

		return "111" + c_bits + d_bits + j_bits;
	}
};

