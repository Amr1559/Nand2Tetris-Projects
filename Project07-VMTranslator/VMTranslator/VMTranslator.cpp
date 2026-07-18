#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "CodeWriter.h"
#include "Parser.h"
using namespace std;

int main(int argc, char* argv[]) {

    string source = (argc > 1) ? argv[1] : "VM_Code.txt"; // if no file passed use default

    ////-------------Construct Parser and CodeWriter-------------
    Parser myParser(source);
    CodeWriter myCodeWriter(source);

    try
    {
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
        }

        cout << "Translation successful!" << endl;
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}




