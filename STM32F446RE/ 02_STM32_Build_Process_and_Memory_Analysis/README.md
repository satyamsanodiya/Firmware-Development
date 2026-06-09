# STM32F446RE - Build Process and Memory Analysis

## Objective

Understand how an Embedded C program is transformed from source code into executable machine code and how the program is stored and executed inside the STM32F446RE microcontroller.

---

## Hardware Platform

* STM32F446RE Nucleo Board
* ARM Cortex-M4 Core

## Development Environment

* STM32CubeIDE
* GNU ARM Toolchain
* GNU Objdump
* GNU Readelf

---

# Embedded Software Build Process

An Embedded C program passes through multiple stages before execution.

```text
main.c
   ↓
Preprocessor
   ↓
Compiler
   ↓
Assembler
   ↓
Object File
   ↓
Linker
   ↓
Executable ELF File
```

---

## Stage 1: Preprocessing

Generated File:

```text
.i
```

Purpose:

* Expand macros
* Include header files
* Process conditional compilation directives

Example:

```c
#include <stdio.h>
```

becomes the complete contents of the header file.

---

## Stage 2: Compilation

Generated File:

```text
.s
```

Purpose:

Convert C code into ARM Assembly instructions.

Example:

```c
x = x + 1;
```

becomes ARM assembly instructions.

---

## Stage 3: Assembly

Generated File:

```text
.o
```

Purpose:

Convert assembly instructions into machine code.

Object files contain:

* Machine instructions
* Symbol information
* Relocation information

---

## Stage 4: Linking

Generated File:

```text
.elf
```

Purpose:

Combine multiple object files and libraries into a single executable image.

The linker also assigns memory locations according to the linker script.

---

# ELF File Analysis

The generated ELF file contains multiple sections.

Important sections:

## .text

Contains:

* Executable machine instructions
* Program code

Stored in:

```text
Flash Memory
```

---

## .rodata

Contains:

* Constant data
* String literals

Stored in:

```text
Flash Memory
```

---

## .data

Contains:

* Initialized global variables

Stored:

```text
Flash (initial value)
↓
Copied to SRAM during startup
```

---

## .bss

Contains:

* Uninitialized global variables
* Static variables initialized to zero

Stored in:

```text
SRAM
```

Initialized by startup code.

---

# Memory Analysis

Using the Memory Browser in STM32CubeIDE, memory contents were inspected directly.

## Flash Memory Inspection

Using Flash base address from the Reference Manual:

```text
0x08000000
```

Observed:

* Program machine code
* Instruction bytes
* Stored constants

---

## SRAM Inspection

Using SRAM base address from the Reference Manual:

```text
0x20000000
```

Observed:

* Runtime variables
* Program data
* Stack and memory allocations

---

# Startup Sequence Analysis

After reset:

```text
Reset
↓
Reset Handler
↓
Data Initialization
↓
BSS Initialization
↓
main()
```

The startup file performs critical initialization.

Responsibilities:

* Copy .data section from Flash to SRAM
* Initialize .bss section with zeros
* Configure stack pointer
* Jump to main()

---

# Disassembly Analysis

The Disassembly View was used to inspect generated machine instructions.

Observed:

* ARM Cortex-M4 instructions
* Function calls
* Branch instructions
* Memory access operations

This demonstrates how high-level C code is translated into machine instructions executed by the processor.

---

# Instruction-Level Debugging

The debugger was used to execute code instruction by instruction.

Features used:

* Step Into
* Step Over
* Breakpoints
* Register View
* Disassembly View

Learning Outcome:

Understanding the relationship between:

```text
C Code
↓
Assembly Instructions
↓
Machine Code
↓
Processor Execution
```

---

# Tools Used

* STM32CubeIDE
* GNU ARM Compiler
* GNU Objdump
* Memory Browser
* Disassembly Viewer
* Debugger

---

# Key Concepts Learned

* Embedded build process
* Preprocessing
* Compilation
* Assembly generation
* Object files
* Linking
* ELF file structure
* Flash memory organization
* SRAM memory organization
* Startup code operation
* Reset Handler functionality
* Instruction-level debugging
* ARM Cortex-M4 execution flow

---

# Learning Outcome

Successfully analyzed the complete Embedded C software build process for STM32F446RE, from source code generation to machine-code execution.

Developed an understanding of how program code, variables, memory sections, startup files, and processor instructions interact within an ARM Cortex-M4 embedded system.
