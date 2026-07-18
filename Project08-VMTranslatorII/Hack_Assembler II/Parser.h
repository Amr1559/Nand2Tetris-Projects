#pragma once
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
using namespace std;

enum eCommandType
{
	C_ARITHMETIC, C_PUSH, C_POP, C_LABEL,
	C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL
};

class Parser
{
private:

	ifstream Source;
	string nextCommand;
	string currentCommand;
	bool hasNext;
	vector<string> container = { "", "", "" };
	eCommandType currentType;

	static string cleanLine(string line) {

		short pos = line.find("//"); //find comment position
		if (pos != string::npos)
		{
			line.erase(pos);
		}

		line.erase(remove(line.begin(), line.end(), '\r'), line.end());

		pos = line.find_first_not_of(" \t\n\r\f\v");

		// If the string is all spaces, clear it completely
		if (pos == string::npos) {
			line.clear();
		}
		else {
			// Erase everything from index 0 up to the first non-space character
			line.erase(0, pos);
		}

		//line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end()); //remove white space

		return line;
	}

	void determineCommandType() {

		if (container[0] == "push") {
			currentType = eCommandType::C_PUSH;
			return;
		}

		if (container[0] == "pop") {
			currentType = eCommandType::C_POP;
			return;
		}

		if (container[0] == "add" ||
			container[0] == "sub" ||
			container[0] == "neg" ||
			container[0] == "and" ||
			container[0] == "or"  ||
			container[0] == "not" ||
			container[0] == "eq"  ||
			container[0] == "gt"  ||
			container[0] == "lt")
		{
			currentType = eCommandType::C_ARITHMETIC;
			return;
		}
		if (container[0] == "label")
		{
			currentType = eCommandType::C_LABEL;
			return;
		}
		if (container[0] == "goto")
		{
			currentType = eCommandType::C_GOTO;
			return;
		}
		if (container[0] == "if-goto")
		{
			currentType = eCommandType::C_IF;
			return;
		}
		if (container[0] == "function")
		{
			currentType = eCommandType::C_FUNCTION;
			return;
		}
		if (container[0] == "call")
		{
			currentType = eCommandType::C_CALL;
			return;
		}
		if (container[0] == "return")
		{
			currentType = eCommandType::C_RETURN;
			return;
		}

		throw invalid_argument("Unknown command type: " + container[0]);
	}

	void lookNext() {
		string rawline, cleaned;

		hasNext = false;

		while (getline(Source, rawline))
		{
			cleaned = cleanLine(rawline);
			if (!cleaned.empty())
			{
				nextCommand = cleaned;
				hasNext = true;
				break; // Found one! Stop looking.
			}
		}
	}

public:

	Parser(string filename) : hasNext(false)
	{
		Source.open(filename);

		if (!Source.is_open()) {
			throw runtime_error("Error: Could not open file " + filename);
		}

		lookNext();
	}

	bool hasMoreCommand() {
		return hasNext;
	}

	void advance() {
		
		currentCommand = nextCommand;

		// Clear the container from the previous command
		container[0] = "";
		container[1] = "";
		container[2] = "";

		stringstream ss(currentCommand);

		ss >> container[0] >> container[1] >> container[2];

		lookNext();
		determineCommandType();
	}

	eCommandType commandType() {
		return currentType;
	}

	string arg1() {
		if (currentType == eCommandType::C_ARITHMETIC) {
			return container[0];
		}
		return container[1];

	}

	int arg2() {
		
		if (currentType == C_PUSH || currentType == C_POP || currentType == C_FUNCTION || currentType == C_CALL) {
			return stoi(container[2]);
		}
		return 0;
	}
};