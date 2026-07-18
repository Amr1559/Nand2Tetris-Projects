# ⚙️ Hack Assembler — Nand2Tetris Project 6

A **Hack Assembler** written in C++ that translates Hack assembly language (`.asm`) into 16-bit binary machine code (`.txt`), as part of the [Nand2Tetris](https://www.nand2tetris.org/) course.

---

## 🔍 What It Does

The assembler takes a Hack assembly program and produces a binary file ready to be loaded into the Hack computer built in Projects 1–5.

**Example:**

Input (`AssemblyCode.txt`):
```
@2
D=A
@3
D=D+A
@0
M=D
```

Output (`HackBinary.txt`):
```
0000000000000010
1110110000010000
0000000000000011
0000000000000010
1110000010010000
0000000000000000
1111001100001000
```

---

## 🏗️ Architecture

The assembler follows the proposed architecture from the Nand2Tetris course, split into 3 modules + main:

| Module | File | Responsibility |
|---|---|---|
| Parser | `Parser.h` | Cleans lines, strips comments/whitespace, parses C-instructions into dest/comp/jump fields |
| Coder | `Coder.h` | Translates parsed fields into binary using lookup maps for dest, comp, and jump |
| Symbol Table | `SymbolTable.h` | Manages predefined symbols (R0–R15, SCREEN, KBD, etc.), labels, and variables |
| Main | `Hack_Assembler.cpp` | Orchestrates the two-pass assembly process |

---

## ⚙️ How It Works — Two-Pass Assembly

**Pass 1 — Label Resolution:**
Scans all instructions and records label symbols (e.g. `(LOOP)`) with their ROM address into the symbol table — without generating any output.

**Pass 2 — Translation:**
Iterates through instructions again and translates each one:
- **A-instruction** (`@value` or `@symbol`) → 16-bit binary, resolving symbols via the symbol table (new variables assigned to RAM starting at address 16)
- **C-instruction** (`dest=comp;jump`) → `111` + 7-bit comp + 3-bit dest + 3-bit jump

---

## 🚀 How To Use

1. Paste your Hack assembly code into `AssemblyCode.txt`
2. Build and run the project in Visual Studio
3. The binary output will be written to `HackBinary.txt`

---

## ⚙️ Build Requirements

- Windows OS
- Visual Studio 2019 or later (x64)

---

## 📚 Concepts Demonstrated

- Two-pass assembler design
- Symbol table with predefined symbols, label resolution, and variable allocation
- Instruction parsing and binary encoding
- File I/O in C++
- Modular design with separated Parser, Coder, and SymbolTable classes
