#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "Parser.h"
using namespace std;

class CodeWriter {
private:
    ofstream outputFile;
    string currentFileName; // Crucial for static variables (e.g., "Main")
    int labelCount = 0;     // To generate unique TRUE/FALSE labels for eq/gt/lt
    unordered_map<string, string> arithmaticAssembly = {
    {"add", "@SP\nAM=M-1\nD=M\nA=A-1\nM=D+M"},
    {"sub", "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D"},
    {"neg", "@SP\nA=M-1\nM=-M"},
    {"and", "@SP\nAM=M-1\nD=M\nA=A-1\nM=D&M"},
    {"or",  "@SP\nAM=M-1\nD=M\nA=A-1\nM=D|M"},
    {"not", "@SP\nA=M-1\nM=!M"},
    };
    unordered_map<string, string> memorySegments = {
        {"local", "LCL"},
        {"argument", "ARG"},
        {"this", "THIS"},
        {"that", "THAT"}
    };
    
 

public:
    //Constructor: Opens the output file/stream
    CodeWriter(string outFileName) {

        int pos = outFileName.find_last_of('.');

        if (pos != string::npos)
        {
            currentFileName = outFileName.substr(0, pos);
        }

        outputFile.open(currentFileName + ".asm");
    }

    // Writes the assembly code for the given arithmetic command
    void writeArithmetic(string command) {

        outputFile << "// " << command << endl;
        
        if (command != "eq" && command != "gt" && command != "lt")
        {
            outputFile << arithmaticAssembly[command] << endl; 
        }
        else
        {
            string jumpInstruction;
            if (command == "eq") jumpInstruction = "JEQ";
            if (command == "gt") jumpInstruction = "JGT";
            if (command == "lt") jumpInstruction = "JLT";

            string label = to_string(labelCount);

            // 1. Pop y and x, then calculate D = x - y
            outputFile << "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n";
            // 2. Jump if true
            outputFile << "@TRUE_" << label << "\n";
            outputFile << "D;" << jumpInstruction << "\n";
            // 3. The False branch (Did not jump)
            outputFile << "@SP\nA=M-1\nM=0\n"; // Set memory to 0 (False)
            outputFile << "@CONTINUE_" << label << "\n";
            outputFile << "0;JMP\n";          // Skip the true branch
            // 4. The True branch (Jumped here)
            outputFile << "(TRUE_" << label << ")\n";
            outputFile << "@SP\nA=M-1\nM=-1\n"; // Set memory to -1 (True)
            // 5. The Continue label
            outputFile << "(CONTINUE_" << label << ")\n";

            labelCount++;
        }
    }

    //Writes the assembly code for push or pop
    void writePushPop(eCommandType command, string segment, int index) {

        outputFile << "// " << (command == C_PUSH ? "PUSH" : "POP") << " " << segment << " " << index << endl;

        string i = to_string(index);

        if (command == C_PUSH)
        {
            if (command == C_PUSH)
            {
                if (segment == "constant") {
                    outputFile << "@" << i << "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
                }
                else if (segment == "local" || segment == "argument" || segment == "this" || segment == "that") {
                    outputFile << "@" << i << "\nD=A\n@" << memorySegments[segment]
                               << "\nA=D+M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
                }
                else if (segment == "temp") {
                    outputFile << "@5\nD=A\n@" << i << "\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
                }
                else if (segment == "pointer") {
                    outputFile << "@" << (index == 0 ? "THIS" : "THAT")
                               << "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
                }
                else if (segment == "static") {
                    outputFile << "@" << currentFileName << "." << i
                               << "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
                }
            }
        }
        else if (command == C_POP)
        {
            if (segment == "local" || segment == "argument" || segment == "this" || segment == "that") {
                // 1. Calculate address (segment_base + index) and store in R13
                // 2. Pop stack to D
                // 3. Write D to address in R13
                outputFile << "@" << i << "\nD=A\n@" << memorySegments[segment]
                    << "\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
            }
            else if (segment == "temp") {
                // Address is 5 + index (Notice D=D+A because 5 is a constant, not a pointer)
                outputFile << "@" << i << "\nD=A\n@5\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
            }
            else if (segment == "pointer") {
                // No need for R13! Address is fixed at THIS (3) or THAT (4)
                outputFile << "@SP\nAM=M-1\nD=M\n@" << (index == 0 ? "THIS" : "THAT") << "\nM=D\n";
            }
            else if (segment == "static") {
                // No need for R13! Address is handled by Assembler labels
                outputFile << "@SP\nAM=M-1\nD=M\n@" << currentFileName << "." << i << "\nM=D\n";
            }
            // Note: There is no "pop constant" because you cannot overwrite a number!
        }

    }

    // 5. Destructor: Close the file!
    ~CodeWriter() {
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }
};