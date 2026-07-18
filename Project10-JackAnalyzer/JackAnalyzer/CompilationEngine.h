#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "JackTokenizer.h"

using namespace std;

class CompilationEngine {
private:
    ofstream outputFile;
    JackTokenizer* tokenizer; // Pointer to the active tokenizer

    //translate enums back to strings
    string getKeywordString(eKeywordType kw) {
        switch (kw) {
        case K_CLASS: return "class";
        case K_METHOD: return "method";
        case K_FUNCTION: return "function";
        case K_CONSTRUCTOR: return "constructor";
        case K_INT: return "int";
        case K_BOOLEAN: return "boolean";
        case K_CHAR: return "char";
        case K_VOID: return "void";
        case K_VAR: return "var";
        case K_STATIC: return "static";
        case K_FIELD: return "field";
        case K_LET: return "let";
        case K_DO: return "do";
        case K_IF: return "if";
        case K_ELSE: return "else";
        case K_WHILE: return "while";
        case K_RETURN: return "return";
        case K_TRUE: return "true";
        case K_FALSE: return "false";
        case K_NULL: return "null";
        case K_THIS: return "this";
        default: return "unknown";
        }
    }

    bool isOp(char c) {
        string ops = "+-*/&|<>=";
        return ops.find(c) != string::npos;
    }

    void process(eTokenType expectedType) {
        if (tokenizer->tokenType() != expectedType) {
            throw runtime_error("Syntax error: Unexpected token type.");
        }

        if (expectedType == T_KEYWORD) {
            outputFile << "<keyword> " << getKeywordString(tokenizer->keyword()) << " </keyword>\n";
        }
        else if (expectedType == T_SYMBOL) {
            char sym = tokenizer->symbol();
            // XML requires special formatting for these 4 symbols
            if (sym == '<') outputFile << "<symbol> &lt; </symbol>\n";
            else if (sym == '>') outputFile << "<symbol> &gt; </symbol>\n";
            else if (sym == '&') outputFile << "<symbol> &amp; </symbol>\n";
            else if (sym == '"') outputFile << "<symbol> &quot; </symbol>\n";
            else outputFile << "<symbol> " << sym << " </symbol>\n";
        }
        else if (expectedType == T_IDENTIFIER) {
            outputFile << "<identifier> " << tokenizer->identifier() << " </identifier>\n";
        }
        else if (expectedType == T_INT_CONST) {
            outputFile << "<integerConstant> " << tokenizer->intVal() << " </integerConstant>\n";
        }
        else if (expectedType == T_STRING_CONST) {
            outputFile << "<stringConstant> " << tokenizer->stringVal() << " </stringConstant>\n";
        }

        // Advance to the next token
        if (tokenizer->hasMoreTokens()) {
            tokenizer->advance();
        }
    }

public:
    // Creates a new compilation engine
    CompilationEngine(string outFilename, JackTokenizer* activeTokenizer) {
        outputFile.open(outFilename);
        tokenizer = activeTokenizer;

        if (tokenizer->hasMoreTokens()) {
            tokenizer->advance();
        }
    }

    void compileClass()
    {
        outputFile << "<class>\n";

        // 1. Process 'class'
        process(T_KEYWORD);

        // 2. Process className (identifier)
        process(T_IDENTIFIER);

        // 3. Process '{'
        process(T_SYMBOL);

        // 4. Process classVarDecs (static or field variables)
                // Keep looping as long as we see 'static' or 'field'
        while (tokenizer->tokenType() == T_KEYWORD &&
            (tokenizer->keyword() == K_STATIC || tokenizer->keyword() == K_FIELD)) {

            compileClassVarDec();
            // compileClassVarDec() will handle eating the tokens
        }

        // 5. Process subroutineDecs (constructors, methods, functions)
                // Keep looping as long as we see 'constructor', 'function', or 'method'
        while (tokenizer->tokenType() == T_KEYWORD &&
            (tokenizer->keyword() == K_CONSTRUCTOR ||
                tokenizer->keyword() == K_FUNCTION ||
                tokenizer->keyword() == K_METHOD)) {

            compileSubroutine();
        }

        // 6. Process '}'
        process(T_SYMBOL);

        outputFile << "</class>\n";
    }

    void compileClassVarDec() {
        outputFile << "<classVarDec>\n";

        // 1. Process 'static' or 'field'
        process(T_KEYWORD);

        // 2. Process the type ('int', 'char', 'boolean' or custom className)
        if (tokenizer->tokenType() == T_KEYWORD) {
            process(T_KEYWORD); // It's a built-in type
        }
        else {
            process(T_IDENTIFIER);// It's a custom object type
        }

        // 3. Process the varName
        process(T_IDENTIFIER);

        // 4. Handle multiple variables on the same line (e.g., field int x, y, z;)
        while (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == ',')
        {
            process(T_SYMBOL); // Eat the ','
            process(T_IDENTIFIER); // Eat the next varName
        }

        // 5. Process the closing ';'
        process(T_SYMBOL);
        
        outputFile << "</classVarDec>\n";
    }

    void compileSubroutine() {
        outputFile << "<subroutineDec>\n";

        // 1. Process 'constructor', 'function', or 'method'
        process(T_KEYWORD);

        // 2. Process return type ('void', 'int', 'char', 'boolean' OR a custom className)
        if (tokenizer->tokenType() == T_KEYWORD) {
            process(T_KEYWORD);    // It's a built-in type or 'void'
        }
        else {
            process(T_IDENTIFIER); // It's a custom object type
        }

        // 3. Process the subroutineName
        process(T_IDENTIFIER);

        // 4. Process the opening '('
        process(T_SYMBOL);

        // 5. compileParameterList to handle the variables inside the parentheses
        compileParameterList();

        // 6. Process the closing ')'
        process(T_SYMBOL);

        // --- START OF SUBROUTINE BODY ---
        outputFile << "<subroutineBody>\n";

        // 7. Process the opening '{'
        process(T_SYMBOL);

        // 8. Process local variable declarations (varDec*)
                // Keep looping as long as the current token is the 'var' keyword
        while (tokenizer->tokenType() == T_KEYWORD && tokenizer->keyword() == K_VAR)
        {
            compileVarDec();
        }

        // 9. compileStatements to handle all the actual code (let, do, if, while, return)
        compileStatements();

        // 10. Process the closing '}'
        process(T_SYMBOL);

        outputFile << "</subroutineBody>\n";
        // --- END OF SUBROUTINE BODY ---

        outputFile << "</subroutineDec>\n";
    }

    void compileParameterList() {
        outputFile << "<parameterList>\n";

        bool hasParameters = false;

        if (tokenizer->tokenType() == T_KEYWORD &&
            (tokenizer->keyword() == K_INT ||
             tokenizer->keyword() == K_CHAR ||
             tokenizer->keyword() == K_BOOLEAN))
        {
            hasParameters = true;
        }
        else if (tokenizer->tokenType() == T_IDENTIFIER)
        {
            hasParameters = true;
        }

        // If it's not empty then process the parameters
        if (hasParameters)
        {
            // 1. Process the first type
            if (tokenizer->tokenType() == T_KEYWORD) {
                process(T_KEYWORD);
            }
            else {
                process(T_IDENTIFIER);
            }

            // 2. Process the first varName
            process(T_IDENTIFIER);

            // 3. Process any other parameters separated by commas
            while (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == ',')
            {
                process(T_SYMBOL); // Eat the ','

                // Process the next type
                if (tokenizer->tokenType() == T_KEYWORD) {
                    process(T_KEYWORD);
                }
                else {
                    process(T_IDENTIFIER);
                }

                // Process the next varName
                process(T_IDENTIFIER);
            }
        }

        outputFile << "</parameterList>\n";
    }

    void compileVarDec() {
        outputFile << "<varDec>\n";
        // 1. Process 'var'
        process(T_KEYWORD);

        // 2. Process the type ('int', 'char', 'boolean' OR a custom className identifier)
        if (tokenizer->tokenType() == T_KEYWORD) {
            process(T_KEYWORD);    // It's a built-in type
        }
        else {
            process(T_IDENTIFIER); // It's a custom class type
        }

        // 3. Process the first variable name
        process(T_IDENTIFIER);

        // 4. Handle multiple variables declared on the same line (var int x, y, z;)
        while (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == ',') {
            process(T_SYMBOL);      // Eat the ','
            process(T_IDENTIFIER);  // Eat the next varName
        }

        // 5. Process the trailing ';'
        process(T_SYMBOL);

        outputFile << "</varDec>\n";
    }

    void compileStatements() {
        outputFile << "<statements>\n";
      
        while (tokenizer->tokenType() == T_KEYWORD) {

            eKeywordType kw = tokenizer->keyword();

            if (kw == K_LET) {
                compileLet();
            }
            else if (kw == K_DO) {
                compileDo();
            }
            else if (kw == K_IF) {
                compileIf();
            }
            else if (kw == K_WHILE) {
                compileWhile();
            }
            else if (kw == K_RETURN) {
                compileReturn();
            }
            else {
                // If we see a keyword that isn't a statement starter 
                // we've reached the end of the statements block
                break;
            }
        }

        outputFile << "</statements>\n";
    }

    void compileDo() {
        outputFile << "<doStatement>\n";

        // 1. Process the 'do' keyword
        process(T_KEYWORD);

        // 2. Process the first identifier 
        // (This could be a subroutineName, a className, or a varName)
        process(T_IDENTIFIER);

        // 3. Is there a dot? (Output.printInt)
        if (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == '.') {
            process(T_SYMBOL);      // Eat the '.'
            process(T_IDENTIFIER);  // Eat the subroutineName
        }

        // 4. Process the opening '('
        process(T_SYMBOL);

        // 5. compileExpressionList for the arguments inside the parentheses
        compileExpressionList();

        // 6. Process the closing ')'
        process(T_SYMBOL);

        // 7. Process the terminating ';'
        process(T_SYMBOL);

        outputFile << "</doStatement>\n";
    }

    void compileLet() {
        outputFile << "<letStatement>\n";

        //'let' varName('[' expression ']') ? '=' expression ';'

        // 1. Process 'let'
        process(T_KEYWORD);

        // 2. Process the varName
        process(T_IDENTIFIER);

        // 3. Check for OPTIONAL array indexing
        if (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == '[') {
            process(T_SYMBOL);       // Eat the '['

            compileExpression();     // calculate the math inside the brackets

            process(T_SYMBOL);       // Eat the ']'
        }

        // 4. Process the '=' sign
        process(T_SYMBOL);

        // 5. compileExpression to handle the right side of the equation
        compileExpression();

        // 6. Process ';'
        process(T_SYMBOL);

        outputFile << "</letStatement>\n";
    }

    void compileWhile() {
        outputFile << "<whileStatement>\n";

        // 1. Process 'while'
        process(T_KEYWORD);

        // 2. Process the opening '('
        process(T_SYMBOL);

        // 3. compileExpression for the loop condition (e.g., x < 5)
        compileExpression();

        // 4. Process the closing ')'
        process(T_SYMBOL);

        // 5. Process the opening '{' for the loop body
        process(T_SYMBOL);

        // 6. compileStatements to handle the code inside the loop!
        compileStatements();

        // 7. Process the closing '}'
        process(T_SYMBOL);

        outputFile << "</whileStatement>\n";
    }

    void compileReturn() {
        outputFile << "<returnStatement>\n";
        
        // 1. Process 'return' keyword
        process(T_KEYWORD);

        // 2. Check for an OPTIONAL expression.
        // If the current token is NOT a ';', there is an expression.
        if (!(tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == ';')) {
            compileExpression();
        }

        // 3. Process the terminating ';'
        process(T_SYMBOL);

        outputFile << "</returnStatement>\n";
    }

    void compileIf() {
        outputFile << "<ifStatement>\n";

        // 1. Process 'if'
        process(T_KEYWORD);

        // 2. Process the opening '('
        process(T_SYMBOL);

        // 3. compileExpression for the condition
        compileExpression();

        // 4. Process the closing ')'
        process(T_SYMBOL);

        // 5. Process the opening '{' for the if-body
        process(T_SYMBOL);

        // 6. compileStatements for the true-branch code
        compileStatements();

        // 7. Process the closing '}' for the if-body
        process(T_SYMBOL);

        // 8. Check for the OPTIONAL 'else' block
        if (tokenizer->tokenType() == T_KEYWORD && tokenizer->keyword() == K_ELSE) {

            process(T_KEYWORD); // Eat 'else'

            process(T_SYMBOL);  // Eat '{'

            compileStatements(); // handle the false-branch code

            process(T_SYMBOL);  // Eat '}'
        }

        outputFile << "</ifStatement>\n";
    }

    void compileExpression() {
        outputFile << "<expression>\n";

        // 1. Every expression starts with at least one term
        compileTerm();

        // 2. While the next token is an operator (+, -, *, /, &, |, <, >, =)
        while (tokenizer->tokenType() == T_SYMBOL && isOp(tokenizer->symbol())) {

            process(T_SYMBOL); // Eat the operator

            compileTerm();     // Process the next term
        }

        outputFile << "</expression>\n";
    }

    void compileTerm() {
        outputFile << "<term>\n";

        // 1. Integer Constant (e.g., 5)
        if (tokenizer->tokenType() == T_INT_CONST) {
            process(T_INT_CONST);
        }

        // 2. String Constant (e.g., "Hello")
        else if (tokenizer->tokenType() == T_STRING_CONST) {
            process(T_STRING_CONST);
        }

        // 3. Keyword Constant (true, false, null, this)
        else if (tokenizer->tokenType() == T_KEYWORD &&
            (tokenizer->keyword() == K_TRUE || tokenizer->keyword() == K_FALSE ||
                tokenizer->keyword() == K_NULL || tokenizer->keyword() == K_THIS)) {
            process(T_KEYWORD);
        }

        // 4. Nested Expression (e.g., (x + 5))
        else if (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == '(') {
            process(T_SYMBOL);      // Eat '('
            compileExpression();    // Do the math inside
            process(T_SYMBOL);      // Eat ')'
        }

        // 5. Unary Operator (e.g., -x or ~y)
        else if (tokenizer->tokenType() == T_SYMBOL &&
            (tokenizer->symbol() == '-' || tokenizer->symbol() == '~')) {
            process(T_SYMBOL);      // Eat the '-' or '~'
            compileTerm();          // Process the term it applies to
        }

        // 6. The Lookahead Identifier (Variables, Arrays, Subroutine Calls)
        else if (tokenizer->tokenType() == T_IDENTIFIER) {

            // Eat the identifier first (e.g., 'x', 'arr', 'Screen')
            process(T_IDENTIFIER);

            // Now look at the NEXT token to see what kind of identifier this was.

            // Case A: It's an array index (e.g., arr[i])
            if (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == '[') {
                process(T_SYMBOL);      // Eat '['
                compileExpression();    // Evaluate the index
                process(T_SYMBOL);      // Eat ']'
            }

            // Case B: It's a direct method call (e.g., run())
            else if (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == '(') {
                process(T_SYMBOL);      // Eat '('
                compileExpressionList();
                process(T_SYMBOL);      // Eat ')'
            }

            // Case C: It's a class function or object method call (e.g., Screen.draw())
            else if (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == '.') {
                process(T_SYMBOL);      // Eat '.'
                process(T_IDENTIFIER);  // Eat the subroutineName
                process(T_SYMBOL);      // Eat '('
                compileExpressionList();
                process(T_SYMBOL);      // Eat ')'
            }

            // Case D: It's just a normal variable. Do nothing else
        }

        outputFile << "</term>\n";
    }

    void compileExpressionList() {
        outputFile << "<expressionList>\n";

        // If the current token is NOT a closing ')', we have at least one expression
        if (!(tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == ')')) {

            compileExpression();

            // Keep looking for comma-separated expressions
            while (tokenizer->tokenType() == T_SYMBOL && tokenizer->symbol() == ',') {
                process(T_SYMBOL); // Eat the ','
                compileExpression();
            }
        }

        outputFile << "</expressionList>\n";
    }

    ~CompilationEngine() {
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }
};