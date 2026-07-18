# ⚙️ Jack Analyzer — Nand2Tetris Project 10

A **syntax analyzer** for the Jack programming language, written in C++, as part of the [Nand2Tetris](https://www.nand2tetris.org/) course. This is the **first module of a full Jack compiler**, responsible for lexical analysis (tokenizing) and syntax analysis (parsing), producing an XML representation of the program's parse tree.

Later stages of the course replace the XML output with actual Hack VM code generation, turning this analyzer into a complete compiler.

---

## 🔍 What It Does

Given a `.jack` source file (or a folder containing several), the analyzer:
1. **Tokenizes** the source code into keywords, symbols, identifiers, integer constants, and string constants
2. **Parses** the token stream using recursive descent, following the full Jack grammar
3. **Outputs an XML file** representing the exact structure of the parsed program

**Example:**

Input (`Main.jack`):
```jack
class Main {
   function void main() {
      return;
   }
}
```

Output (`Main.xml` excerpt):
```xml
<class>
  <keyword> class </keyword>
  <identifier> Main </identifier>
  <symbol> { </symbol>
  <subroutineDec>
    <keyword> function </keyword>
    ...
  </subroutineDec>
  <symbol> } </symbol>
</class>
```

---

## 🏗️ Architecture

| Module | File | Responsibility |
|---|---|---|
| Tokenizer | `JackTokenizer.h` | Strips comments/whitespace and breaks source code into a stream of typed tokens |
| Compilation Engine | `CompilationEngine.h` | Recursive-descent parser that consumes tokens and emits XML matching the Jack grammar |
| Main | `JackAnalyzer.cpp` | Routes input (single file or directory) and drives tokenizer + engine per file |

---

## ⚙️ How It Works

### Tokenizer
Reads the source line by line, stripping `//` line comments and `/* ... */` block comments (including ones spanning multiple lines), then classifies each token as one of 5 types: keyword, symbol, identifier, integer constant, or string constant.

### Compilation Engine
Implements one `compileXxx()` method per Jack grammar rule (`compileClass`, `compileSubroutine`, `compileStatements`, `compileExpression`, etc.), each recursively calling the next rule as needed. A generic `process()` helper validates the current token's type, writes its XML tag, and advances the tokenizer — keeping the grammar rules themselves clean and declarative.

### Multi-File Support
Like the VM Translator, `JackAnalyzer.cpp` accepts either a single `.jack` file or a folder path. When given a folder, it compiles every `.jack` file inside it, producing one `.xml` output per source file.

---

## 🚀 How To Use

**Single file:**
```
JackAnalyzer.exe Main.jack
```
→ produces `Main.xml`

**Folder of files:**
```
JackAnalyzer.exe MyJackProject/
```
→ produces one `.xml` file per `.jack` file in the folder

---

## ⚙️ Build Requirements

- Windows OS
- Visual Studio 2019 or later (x64)

---

## 📚 Concepts Demonstrated

- Lexical analysis (tokenizing) with comment/whitespace handling
- Recursive-descent parsing following a formal grammar
- XML generation representing a program's parse tree
- Multi-file compilation using C++ `<filesystem>`
- Foundational architecture for a full compiler front-end
