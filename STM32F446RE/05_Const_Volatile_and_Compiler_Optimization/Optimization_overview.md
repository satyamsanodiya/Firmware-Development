# Compiler Optimization

## Overview

Modern embedded systems often operate with limited Flash memory, SRAM, processing power, and battery capacity. Every unnecessary machine instruction increases execution time, memory usage, and power consumption.

Compiler optimization is the process in which the compiler analyzes the source code and generates a more efficient machine code without changing the intended functionality of the program.

Instead of executing exactly what is written in C, the compiler attempts to produce faster, smaller, and more efficient executable code.

In this learning exercise, I explored different GCC optimization levels and observed how enabling optimization changes the generated binary size and instruction sequence.

---

# Why Compiler Optimization is Important

Optimization plays a significant role in embedded firmware because most microcontrollers have limited hardware resources.

A well-optimized program provides several advantages:

- Reduces Flash memory usage
- Reduces program execution time
- Decreases CPU instruction count
- Minimizes memory accesses
- Reduces power consumption
- Improves real-time performance
- Generates cleaner machine instructions

These improvements are especially important in battery-powered embedded systems where execution efficiency directly affects battery life.

---

# GCC Optimization Levels

The GNU Compiler Collection (GCC) provides multiple optimization levels.

| Optimization Level | Description |
|--------------------|-------------|
| **-O0** | No optimization (default during debugging) |
| **-O1** | Basic optimization |
| **-O2** | Recommended optimization for most firmware projects |
| **-O3** | Aggressive optimization for maximum performance |
| **-Os** | Optimizes specifically for minimum code size |

---

# Why -O0 is Used During Debugging

When debugging firmware, optimization is usually disabled.

```
-O0
```

At this level:

- Every C statement closely matches the generated assembly.
- Variables remain available inside the debugger.
- Breakpoints behave exactly as expected.
- Code execution is easy to follow line-by-line.

Although the executable becomes larger and slower, debugging becomes much easier.

---

# Why -O2 is Commonly Used in Production

After firmware has been verified, optimization is typically enabled.

```
-O2
```

The compiler now performs several optimizations, including:

- Removing redundant instructions
- Eliminating unnecessary memory accesses
- Constant propagation
- Constant folding
- Dead code elimination
- Register allocation optimization
- Loop optimization
- Common sub-expression elimination

The result is a smaller and faster executable.

---

# Practical Experiment

To understand compiler optimization, I compiled the same STM32 register-level GPIO program using two different optimization levels.

## Build using -O0

```
text    data    bss     dec     hex
784       0    1568    2352    930
```

Characteristics:

- Larger executable
- More assembly instructions
- Easier debugging
- Every C statement visible in generated code

---

## Build using -O2

```
text    data    bss     dec     hex
708       0    1568    2276    8E4
```

Characteristics:

- Smaller executable
- Fewer machine instructions
- Faster execution
- Better Flash utilization
- Reduced instruction fetch cycles

---

# Result Analysis

Comparing both builds shows that only changing the compiler optimization level reduced the program size.

| Parameter | -O0 | -O2 |
|-----------|------|------|
| Text Section | 784 Bytes | 708 Bytes |
| Data Section | 0 Bytes | 0 Bytes |
| BSS Section | 1568 Bytes | 1568 Bytes |
| Total Size | 2352 Bytes | 2276 Bytes |

The reduction occurred in the **Text section**, which contains executable machine instructions.

This demonstrates that optimization mainly improves the generated instruction sequence rather than changing RAM usage.

---

# How the Compiler Reduces Code Size

The compiler performs multiple internal optimizations before generating machine code.

Some common examples include:

### Dead Code Elimination

Unused variables or instructions are removed.

Example:

```c
int a = 5;
```

If `a` is never used, the compiler removes it completely.

---

### Constant Folding

Compile-time calculations are performed during compilation instead of runtime.

Example:

```c
int x = 5 * 10;
```

Instead of generating multiplication instructions, the compiler simply stores:

```c
int x = 50;
```

---

### Constant Propagation

Known constant values are substituted directly into expressions.

Example:

```c
const int x = 10;
int y = x + 5;
```

Compiler generates:

```c
int y = 15;
```

without reading `x` from memory.

---

### Register Allocation

Frequently used variables are kept inside CPU registers instead of repeatedly reading from SRAM.

This significantly reduces memory access time.

---

### Loop Optimization

Small loops may be optimized or partially unrolled to reduce branch instructions and improve execution speed.

---

# Practical Learning Outcome

Through this experiment, I understood that compiler optimization is not simply about reducing program size.

It directly affects:

- Flash memory utilization
- CPU execution speed
- Instruction count
- Memory access frequency
- Power consumption
- Overall firmware performance

I also learned that while optimization improves execution efficiency, it makes debugging more difficult because variables may be optimized out, instructions may be reordered, and multiple C statements may be combined into fewer machine instructions.

For this reason, firmware developers typically use **-O0 during development** and **-O2 (or sometimes -Os)** for production firmware.
