# 🖥️ Nand2Tetris — From Logic Gates to a Working Computer

This repository contains my project submissions for the [Nand2Tetris](https://www.nand2tetris.org/) course — a bottom-up journey through computer science, building a fully functional computer system from scratch starting with a single NAND gate.

---

## 📂 Projects Overview

| Project | Topic | What I Built |
|---|---|---|
| [Project 1](./Project1/) | Boolean Logic | 15 fundamental logic gates (And, Or, Not, Xor, Mux, DMux, and 16-bit variants) |
| [Project 2](./Project2/) | Boolean Arithmetic | ALU, 16-bit adder, half adder, full adder, incrementer |
| [Project 3](./Project3/) | Sequential Logic | Registers, RAM units (8 to 16K), Program Counter |
| [Project 4](./Project4/) | Machine Language | Assembly programs: Mult (multiplication) and Fill (keyboard-screen I/O) |
| [Project 5](./Project5/) | Computer Architecture | CPU, Memory, and full Hack Computer chip |
| [Project 6](./Project6-HackAssembler/) | Assembler | A Hack Assembler that translates assembly language into binary machine code |
| [Project 7](./Project7-VMTranslator/) | VM Translator I | A VM Translator that converts stack-based VM commands into Hack assembly (arithmetic, memory access) |
| [Project 8](./Project8-VMTranslator/) | VM Translator II | Extended the VM Translator with program flow, function calling, and the ability to translate a full folder of `.vm` files into one program |

---

## 🗺️ The Big Picture

```
NAND gate
    └── Boolean Logic (Project 1)
            └── Arithmetic Logic Unit (Project 2)
                    └── Memory & Registers (Project 3)
                            └── Machine Language (Project 4)
                                    └── CPU & Computer (Project 5)
                                                └── Hack Assembler (Project 6)
                                                        └── VM Translator I (Project 7)
                                                                └── VM Translator II (Project 8) ✅
```

---

## 🛠️ Tools & Languages

- **Projects 1–5:** Hardware Description Language (HDL) — simulated in the Nand2Tetris Hardware Simulator
- **Projects 6–8:** C++ (Visual Studio, Windows x64)

---

## 📌 Notes

- Projects 1–5 use `.hdl` files runnable in the [Nand2Tetris Hardware Simulator](https://www.nand2tetris.org/software)
- Project 6 (Hack Assembler), Project 7, and Project 8 (VM Translator) are standalone C++ Visual Studio projects — see their own READMEs for build and usage instructions
