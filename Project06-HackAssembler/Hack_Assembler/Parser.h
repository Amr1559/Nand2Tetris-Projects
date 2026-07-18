#pragma once

using namespace std;

struct ParsedCInstrucion
{
	string dest = "null";
	string comp = "null";
	string jump = "null";
};
struct ParsedAInstrucion
{
	short value = 0;
};

class Parser
{
private:


public:

	static string cleanLine(string line) {

		short pos = line.find("//"); //find comment position
		if (pos != string::npos)
		{
			line.erase(pos);
		}

		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end()); //remove white space

		return line;
	}

	//is not used in program
	string getAContent(string line)
	{
		return line.substr(1);
	}

	ParsedCInstrucion ParseCInstruction(string line) {

		ParsedCInstrucion Parsed_Command;

		char FirstDelim = '=';
		char SecondDelim = ';';
		short pos = 0;

		// if has = stroe dest
		pos = line.find(FirstDelim);
		if (pos != string::npos)
		{
			Parsed_Command.dest = line.substr(0, pos);
			line.erase(0, pos + 1);
		}

		// if has ; store comp and jump else store comp
		pos = line.find(SecondDelim);
		if (pos != string::npos)
		{
			Parsed_Command.comp = line.substr(0, pos);
			line.erase(0, pos + 1);

			Parsed_Command.jump = line;
		}
		else
		{
			Parsed_Command.comp = line;
		}

		return Parsed_Command;
	}
};