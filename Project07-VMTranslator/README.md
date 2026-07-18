# ⚙️ VM Translator — Nand2Tetris Project 7

A **VM Translator** written in C++ that translates stack-based VM commands (`.vm`) into Hack assembly language (`.asm`), as part of the [Nand2Tetris](https://www.nand2tetris.org/) course.

This is **Stage I** of the VM Translator — covering arithmetic/logical commands and memory access commands (push/pop across all memory segments). Program flow (branching) and function calls are handled in Project 8.

---

## 🔍 What It Does

The translator reads a VM program and converts each stack-based command into the equivalent low-level Hack assembly that manipulates the stack.

**Example:**

Input (`VM_Code.txt`):
```
push constant 7
push constant 8
add
```

Output (`VM_Code.asm`):
```
// PUSH constant 7
@7
D=A
@SP
A=M
M=D
@SP
M=M+1
// PUSH constant 8
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
AM=M-1
D=M
A=A-1
M=D+M
```

---

## 🏗️ Architecture

| Module | File | Responsibility |
|---|---|---|
| Parser | `Parser.h` | Reads the VM file, strips comments, identifies command type, and extracts arguments |
| CodeWriter | `CodeWriter.h` | Generates the corresponding Hack assembly for each VM command |
| Main | `VMTranslator.cpp` | Drives the parse → translate loop and handles file I/O |

---

## ⚙️ Supported Commands

**Arithmetic / Logical:**
`add`, `sub`, `neg`, `eq`, `gt`, `lt`, `and`, `or`, `not`

**Memory Access (push/pop):**

| Segment | Description |
|---|---|
| `constant` | Push a literal constant onto the stack |
| `local` | LCL segment |
| `argument` | ARG segment |
| `this` / `that` | THIS / THAT segments |
| `temp` | Fixed RAM addresses 5–12 |
| `pointer` | THIS (0) / THAT (1) base addresses |
| `static` | File-scoped static variables, mapped to unique labels per VM file |

**Comparison operators** (`eq`, `gt`, `lt`) are implemented using unique generated labels (`TRUE_n` / `CONTINUE_n`) to branch correctly without label collisions across multiple comparisons.

---

## 🚀 How To Use

1. Place your VM commands in `VM_Code.txt` (or pass a filename as a command-line argument)
2. Build and run the project in Visual Studio
3. The translated Hack assembly will be written to a `.asm` file with the same base name

---

## ⚙️ Build Requirements

- Windows OS
- Visual Studio 2019 or later (x64)

---

## 📚 Concepts Demonstrated

- Stack-based virtual machine semantics
- Translating high-level VM operations into low-level assembly
- Symbolic label generation to avoid naming collisions
- Modular design separating parsing from code generation
- File-scoped static variable handling
