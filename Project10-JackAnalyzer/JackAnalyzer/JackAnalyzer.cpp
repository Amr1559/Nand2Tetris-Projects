#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "JackTokenizer.h"
#include "CompilationEngine.h"

using namespace std;
namespace fs = filesystem;

// Helper function to handle the compilation of a single file
void compileFile(const string& inputFilename)
{
    // Create the output filename by replacing .jack with .xml
    int dotIndex = inputFilename.find_last_of('.');
    string outputFilename = inputFilename.substr(0, dotIndex) + ".xml";

    cout << "Compiling: " << inputFilename << " -> " << outputFilename << endl;

    try {
        // 1. Start the Tokenizer
        JackTokenizer tokenizer(inputFilename);

        // 2. Pass it to the Engine
        CompilationEngine engine(outputFilename, &tokenizer);

        // 3. Start compiling
        engine.compileClass();

    }
    catch (const exception& e) {
        cerr << "Error compiling " << inputFilename << ": " << e.what() << endl;
    }
}

int main(int argc, char* argv[])
{
    // Check if the user passed a file/folder path
    if (argc != 2) {
        cerr << "Usage: JackAnalyzer <input path>" << endl;
        return 1;
    }

    string inputPath = argv[1];
    vector<string> filesToCompile;

    // --- STEP 1: Check input ---

    // if it's a single file
    if (fs::is_regular_file(inputPath) && inputPath.substr(inputPath.find_last_of('.')) == ".jack") {
        filesToCompile.push_back(inputPath);
    }
    // if it's a directory
    else if (fs::is_directory(inputPath)) {
        // Loop through the directory and grab all .jack files
        for (const auto& entry : fs::directory_iterator(inputPath)) {
            if (entry.path().extension() == ".jack") {
                filesToCompile.push_back(entry.path().string());
            }
        }
    }
    // if invalid input
    else {
        cerr << "Error: Input must be a .jack file or a directory containing .jack files." << endl;
        return 1;
    }

    // --- STEP 2: Compile Everything ---

    if (filesToCompile.empty()) {
        cout << "No .jack files found in the specified path." << endl;
        return 0;
    }

    for (const string& file : filesToCompile) {
        compileFile(file);
    }

    cout << "Compilation completely successful!" << endl;
    return 0;
}