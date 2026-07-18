#pragma once
using namespace std;

class SymbolTable {
private:
private:
    // Only ONE map for everything
    map<string, int> table;
    int romAddress = 0;
    int nextVarAddress = 16; // Variables start at RAM 16

public:
    SymbolTable() {
        // Initialize the map with all Predefined values
        table = {
            {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3},
            {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7},
            {"R8", 8}, {"R9", 9}, {"R10", 10}, {"R11", 11},
            {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
            {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4},
            {"SCREEN", 16384}, {"KBD", 24576}
        };
    }

    void firstPass(const vector<string>& instructions) {

        for (const string& line : instructions) {

            if (line[0] == '(') {
                // If label
                // Extract everything between '(' and ')'
                string label = line.substr(1, line.size() - 2);

                // The label points to the CURRENT romAddress 
                // (the address of the NEXT actual instruction)
                table[label] = romAddress;
            }
            else {
                // It's an A_instruction or C_instruction
                romAddress++;
            }
        }
    }

    int getAddress(string symbol) {
        if (table.find(symbol) == table.end())
        {
            table[symbol] = nextVarAddress;
            nextVarAddress++;
        }
        return table[symbol];
    }
};
