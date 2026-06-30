#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "CodeWriter.h"
#include "Parser.h"
using namespace std;
namespace fs = filesystem;

int main(int argc, char* argv[]) {

    string sourcePath = (argc > 1) ? argv[1] : "VM_Code.txt"; // if no file passed use default

    string outFilePath;
    vector<string> vmFiles;
    bool isDirectory = false;



    try
    {
        if (fs::is_regular_file(sourcePath))
        {
            vmFiles.push_back(sourcePath);
            outFilePath = sourcePath;
        }
        else if (fs::is_directory(sourcePath))
        {
            isDirectory = true;

            string dirName = fs::path(sourcePath).filename().string();
            if (dirName.empty()) {
                dirName = fs::path(sourcePath).parent_path().filename().string();
            }

            outFilePath = sourcePath + '/' + dirName;

            for (const auto& entry : fs::directory_iterator(sourcePath))
            {
                if (entry.path().extension() == ".vm")
                {
                    vmFiles.push_back(entry.path().string());
                }
            }
        }
        else {
            cerr << "Error: Path is neither a file nor a directory." << endl;
            return 1;
        }


        ////-------------Construct CodeWriter-------------
        CodeWriter myCodeWriter(outFilePath);

        if (isDirectory) {
            myCodeWriter.writeInit();
        }

        for (const string& vmFile : vmFiles)
        {
            myCodeWriter.setFileName(fs::path(vmFile).stem().string());

            Parser myParser(vmFile);

            while (myParser.hasMoreCommand())
            {
                myParser.advance();

                if (myParser.commandType() == C_PUSH || myParser.commandType() == C_POP)
                {
                    myCodeWriter.writePushPop(myParser.commandType(), myParser.arg1(), myParser.arg2());
                }
                else if (myParser.commandType() == C_ARITHMETIC)
                {
                    myCodeWriter.writeArithmetic(myParser.arg1());
                }
                else if (myParser.commandType() == C_LABEL)
                {
                    myCodeWriter.writeLabel(myParser.arg1());
                }
                else if (myParser.commandType() == C_GOTO)
                {
                    myCodeWriter.writeGoto(myParser.arg1());
                }
                else if (myParser.commandType() == C_IF)
                {
                    myCodeWriter.writeIf(myParser.arg1());
                }
                else if (myParser.commandType() == C_FUNCTION)
                {
                    myCodeWriter.writeFunction(myParser.arg1(), myParser.arg2());
                }
                else if (myParser.commandType() == C_CALL)
                {
                    myCodeWriter.writeCall(myParser.arg1(), myParser.arg2());
                }
                else if (myParser.commandType() == C_RETURN)
                {
                    myCodeWriter.writeReturn();
                }

            }
        }

        std::cout << "Translation successful!" << endl;
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}