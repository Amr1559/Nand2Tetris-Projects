#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

using namespace std;

// types of tokens in Jack
enum eTokenType {
    T_KEYWORD,
    T_SYMBOL,
    T_IDENTIFIER,
    T_INT_CONST,
    T_STRING_CONST,
    T_NONE // for initialization
};

//keywords in Jack
enum eKeywordType {
    K_CLASS, K_METHOD, K_FUNCTION, K_CONSTRUCTOR, K_INT,
    K_BOOLEAN, K_CHAR, K_VOID, K_VAR, K_STATIC, K_FIELD,
    K_LET, K_DO, K_IF, K_ELSE, K_WHILE, K_RETURN,
    K_TRUE, K_FALSE, K_NULL, K_THIS
};

unordered_set<string> validKeywords = {
        "class", "constructor", "function", "method", "field",
        "static", "var", "int", "char", "boolean", "void",
        "true", "false", "null", "this", "let", "do",
        "if", "else", "while", "return"
};

class JackTokenizer {
private:
    ifstream inputFile;
    string currentToken;
    eTokenType currentTokenType;

    string currentLine = "";
    bool insideCommentBlock = false;

    // Helper for string cleaning
    string removeCommentsAndWhitespace(string line) {

        // 1. Check if we are stuck in a block from the previous line
        if (insideCommentBlock) {
            int endPos = line.find("*/");
            if (endPos != string::npos) {
                line.erase(0, endPos + 2);
                insideCommentBlock = false;
            }
            else {
                line.clear();
                return line; // Nothing left to do on this line
            }
        }

        // 2. clean in line comments
        int pos = line.find("//");
        if (pos != string::npos)
        {
            line.erase(pos);
        }

        // 3. check for block comments
        int startpos = line.find("/*");
        if (startpos != string::npos)
        {
            int endPos = line.find("*/");
            if (endPos != string::npos) {
                // It starts and ends on the same line! Erase the block.
                line.erase(startpos, (endPos + 2) - startpos);
            }
            else
            {
                line.erase(startpos);
                insideCommentBlock = true;
            }
        }

        // 4. clean white spaces on the right (indents)
        pos = line.find_first_not_of(" \t\n\r\f\v");
        // If the string is all spaces clear it completely
        if (pos == string::npos) {
            line.clear();
        }
        else {
            // Erase everything from index 0 up to the first non-space character
            line.erase(0, pos);
        }

        return line;
    }

    bool isKeyword(const string& word) {
        //return validKeywords.contains(word); // C++20
        return validKeywords.find(word) != validKeywords.end(); // C++17
    }

public:
    // Opens the input file
    JackTokenizer(string filename) {
        inputFile.open(filename);
        if (!inputFile.is_open()) {
            throw runtime_error("Error: Could not open file " + filename);
        }
        currentTokenType = T_NONE;
    }

    // Do we have more tokens in the input?
    bool hasMoreTokens() {
        // Keep reading until we get a line that isn't completely empty
        while (currentLine.empty()) {
            if (!getline(inputFile, currentLine)) {
                return false; // We reached the end of the file
            }

            // clean the line
            currentLine = removeCommentsAndWhitespace(currentLine);
        }

        return true;
    }

    // Gets the next token and makes it the current token
    void advance() {

        if (currentLine.empty()) return; // Safety check

        char firstChar = currentLine[0];

        // 1. Check if SYMBOL
        string symbols = "{}()[].,;+-*/&|<>=~";
        if (symbols.find(firstChar) != string::npos)
        {
            currentToken = string(1, firstChar);
            currentTokenType = T_SYMBOL;
            currentLine.erase(0, 1);
        }
        // 2. Check if STRING_CONST? (Starts with a quote)
        else if (firstChar == '"')
        {
            int endquote = currentLine.find('"', 1);
            currentToken = currentLine.substr(1, endquote - 1);
            currentTokenType = T_STRING_CONST;
            currentLine.erase(0, endquote + 1);
        }
        // 3. Check if INT_CONST? (Starts with a number)
        else if (isdigit(firstChar))
        {
            int pos = 0;
            while (pos < currentLine.length() && isdigit(currentLine[pos]))
            {
                pos++;
            }

            currentToken = currentLine.substr(0, pos);
            currentTokenType = T_INT_CONST;
            currentLine.erase(0, pos);
        }
        // 4. It is a KEYWORD or IDENTIFIER
        else
        {
            int pos = 0;
            while (pos < currentLine.length() && (isalnum(currentLine[pos]) || currentLine[pos] == '_'))
            {
                pos++;
            }
            currentToken = currentLine.substr(0, pos);

            if (isKeyword(currentToken)) {
                currentTokenType = T_KEYWORD;
            } else {
                currentTokenType = T_IDENTIFIER;
            }
            currentLine.erase(0, pos);
        }

        // --- FINAL CLEANUP ---
        int nextSpace = currentLine.find_first_not_of(" \t");
        if (nextSpace == string::npos) {
            currentLine.clear();
        }
        else {
            currentLine.erase(0, nextSpace);
        }
    }

    // Returns the type of the current token
    eTokenType tokenType() {
        return currentTokenType;
    }

    // Returns the keyword which is the current token
    eKeywordType keyword() {
        if (currentToken == "class") return K_CLASS;
        if (currentToken == "method") return K_METHOD;
        if (currentToken == "function") return K_FUNCTION;
        if (currentToken == "constructor") return K_CONSTRUCTOR;
        if (currentToken == "int") return K_INT;
        if (currentToken == "boolean") return K_BOOLEAN;
        if (currentToken == "char") return K_CHAR;
        if (currentToken == "void") return K_VOID;
        if (currentToken == "var") return K_VAR;
        if (currentToken == "static") return K_STATIC;
        if (currentToken == "field") return K_FIELD;
        if (currentToken == "let") return K_LET;
        if (currentToken == "do") return K_DO;
        if (currentToken == "if") return K_IF;
        if (currentToken == "else") return K_ELSE;
        if (currentToken == "while") return K_WHILE;
        if (currentToken == "return") return K_RETURN;
        if (currentToken == "true") return K_TRUE;
        if (currentToken == "false") return K_FALSE;
        if (currentToken == "null") return K_NULL;
        if (currentToken == "this") return K_THIS;

        throw runtime_error("Error: Current token is not a keyword.");
    }

    // Returns the character which is the current token
    char symbol() {
        return currentToken[0];
    }

    // Returns the identifier which is the current token
    string identifier() {
        return currentToken;
    }

    // Returns the integer value of the current token
    int intVal() {
        return stoi(currentToken);
    }

    // Returns the string value of the current token (without quotes)
    string stringVal() {
        return currentToken;
    }

    ~JackTokenizer() {
        if (inputFile.is_open()) {
            inputFile.close();
        }
    }
};
