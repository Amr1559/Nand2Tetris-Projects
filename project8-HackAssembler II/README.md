# ⚙️ VM Translator II — Nand2Tetris Project 8

A continuation of the [Project 7 VM Translator](../Project7-VMTranslator/), extended with **program flow** and **function calling** commands, as part of the [Nand2Tetris](https://www.nand2tetris.org/) course.

This stage completes the VM Translator, turning it into a full backend capable of translating multi-function VM programs into running Hack assembly.

---

## 🔍 What's New vs Project 7

Project 7 covered arithmetic and memory access. Project 8 adds:

| Category | Commands |
|---|---|
| **Program Flow** | `label`, `goto`, `if-goto` |
| **Function Calling** | `function`, `call`, `return` |
| **Bootstrap** | Initializes the stack and calls `Sys.init` automatically |
| **Multi-File Programs** | Can translate an entire folder of `.vm` files into a single `.asm` output |

---

## 🏗️ Architecture

| Module | File | Responsibility |
|---|---|---|
| Parser | `Parser.h` | Parses VM commands, now also recognizing label/branch/function commands |
| CodeWriter | `CodeWriter.h` | Generates Hack assembly, including labels, function frames, and the call/return protocol |
| Main | `VMTranslator.cpp` | Drives the parse → translate loop |

---

## ⚙️ How It Works

### Program Flow
- **`label`** — declares a jump target, scoped to the current function (`functionName$label`) to avoid collisions between functions
- **`goto`** — unconditional jump to a label
- **`if-goto`** — pops the stack and jumps if the value is non-zero

### Function Calling Convention
- **`function f k`** — declares function `f` with `k` local variables, initialized to 0
- **`call f n`** — implements the full calling protocol: pushes the return address and saved segment pointers (LCL, ARG, THIS, THAT), repositions ARG and LCL, then jumps to `f`
- **`return`** — restores the caller's segment pointers from the saved frame, repositions the stack, and jumps back to the return address

### Bootstrap Code
`writeInit()` generates the standard Hack bootstrap: sets `SP=256` and calls `Sys.init`, so a translated program is ready to run as soon as it's loaded.

### Multi-File Translation
The translator accepts either a single `.vm` file **or a folder path**. When given a folder, it automatically:
- Scans the folder for all `.vm` files
- Translates each one in sequence into a single combined `.asm` file (named after the folder)
- Prepends the bootstrap code so the program is immediately runnable
- Sets the correct file name context for each file's `static` variables, so multi-file programs don't collide

This mirrors how real Jack/VM programs are structured — a project folder containing multiple `.vm` files (e.g. `Main.vm`, `Sys.vm`) that all get linked into one final program.

---

## 🚀 How To Use

**Single file:**
1. Place your VM commands in a `.vm` file
2. Run the program, passing the file path as an argument (or use the default `VM_Code.txt`)
3. The translated Hack assembly will be written to a `.asm` file with the same base name

**Folder of files:**
1. Place multiple `.vm` files inside a folder
2. Run the program, passing the folder path as an argument
3. All files are translated and merged into a single `.asm` file named after the folder, with bootstrap code included

---

## ⚙️ Build Requirements

- Windows OS
- Visual Studio 2019 or later (x64)

---

## 📚 Concepts Demonstrated

- Function calling conventions and stack frame management
- Return address handling and saved-segment restoration
- Scoped labels to support branching within and across functions
- Bootstrap/initialization code generation
- Multi-file program linking using C++ `<filesystem>`
- Building on a modular Parser/CodeWriter design from Project 7
