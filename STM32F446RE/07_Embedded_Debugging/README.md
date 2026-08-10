# Embedded Firmware Debugging Techniques

## Overview

Debugging is one of the most important skills in embedded firmware development. Unlike application software, embedded firmware directly interacts with processors, memory, peripherals, interrupts, communication interfaces, and physical hardware. Therefore, finding a problem often requires observing the firmware at different levels.

In this section, I studied different debugging techniques available in STM32CubeIDE for understanding program execution, runtime data, memory, function calls, and processor-level instructions.

The debugging techniques covered are:

- Serial Wire Viewer (SWV)
- ITM and SWV Data Tracing
- Single Stepping
- Step Into
- Step Over
- Step Out
- Breakpoints
- Breakpoint Management
- Disassembly
- Call Stack
- Expression Window
- Variable Window
- Memory Browser
- Data Watchpoints

The main objective is to understand how to move systematically from a firmware problem to its root cause instead of debugging by trial and error.

---

# 1. Serial Wire Viewer (SWV)

## 1.1 What is SWV?

Serial Wire Viewer (SWV) is a debugging and trace mechanism available on supported ARM Cortex-M processors.

It allows runtime information to be observed while the processor continues executing the firmware.

A simplified debugging path is:

    STM32 MCU
       |
       | SWD / SWO
       |
    ST-LINK
       |
       v
    STM32CubeIDE
       |
       v
    SWV Trace Views

Traditional breakpoint debugging stops the processor at a particular location.

SWV can instead provide runtime information without repeatedly stopping normal execution.

This makes SWV useful for:

- Runtime monitoring
- Timing-sensitive firmware
- Observing variable changes
- Event tracing
- Print-style debugging
- Performance investigation

SWV support depends on the MCU core, debug probe, SWD connection, and availability of the SWO signal. ST's documentation specifically notes that SWV/ITM are not available on Cortex-M0/M0+ cores. Therefore, SWV capability must always be checked for the target MCU. 

---

# 2. ITM and SWV

## 2.1 Instrumentation Trace Macrocell (ITM)

The Instrumentation Trace Macrocell (ITM) is a Cortex-M debug component that can be used to send instrumentation information from the processor to the debugger.

SWV can transport this information to STM32CubeIDE.

A common application is printf-style debugging.

For example:

```c
printf("Counter = %lu\n", counter);
```

Instead of using a normal UART only for debugging messages, ITM/SWV can provide a debug output mechanism when supported by the MCU and debug configuration.

Conceptually:

    Firmware
       |
       v
      ITM
       |
       v
      SWV
       |
       v
    ST-LINK
       |
       v
    STM32CubeIDE

---

# 3. SWV Data Tracing

SWV can also be used to observe runtime data.

For example:

```c
uint32_t counter = 0;

while(1)
{
    counter++;
}
```

If I want to understand how `counter` changes during execution, a trace-based method can be more useful than repeatedly stopping the processor.

Conceptually:

    counter
       |
       | Runtime changes
       v
    SWV Trace
       |
       v
    STM32CubeIDE

This is particularly useful for:

- Variables that change continuously
- Timing-related behavior
- Communication activity
- State-machine transitions
- Runtime events
- Performance analysis

---

# 4. SWV ITM Data Console

The SWV ITM Data Console can be used to observe text or instrumentation data sent through ITM.

For example:

```c
printf("System Started\n");
printf("ADC Value = %d\n", adc_value);
printf("State = %d\n", state);
```

The output can be observed through the debugger.

This provides a convenient way to understand what the firmware is doing during execution.

However, trace output itself can have configuration and timing limitations, so it should not automatically be treated as a completely invisible debugging mechanism.

---

# 5. Single Stepping

Single stepping allows firmware execution to be controlled one source-level step at a time.

For example:

```c
int a = 10;
int b = 20;
int c = a + b;
```

Instead of allowing the program to execute continuously, the debugger can execute one statement and then pause.

The basic process is:

    Execute one statement
            |
            v
        CPU stops
            |
            v
    Inspect program state
            |
            v
    Execute next statement
            |
            v
        CPU stops again

Single stepping is useful for understanding:

- Program flow
- Variable changes
- Conditional statements
- Function execution
- Loops
- Unexpected behavior

It is especially useful while learning unfamiliar firmware.

---

# 6. Step Into

Step Into is used when I want to enter a function and debug its internal execution.

Example:

```c
int main(void)
{
    initialize();
    process_data();
}
```

If execution is currently at:

```c
initialize();
```

using Step Into enters the `initialize()` function.

Conceptually:

    main()
       |
       +----> initialize()
                    |
                    +----> inspect internal statements
                    |
                    +----> continue execution

Step Into is useful when:

- I suspect the problem is inside a function.
- I want to understand how a function works.
- I want to follow function execution in detail.
- I need to inspect internal variables.

---

# 7. Step Over

Step Over executes a function call without entering the function internally.

Example:

```c
int main(void)
{
    initialize();
    process_data();
}
```

If I use Step Over on:

```c
initialize();
```

the debugger executes the function and stops at the next statement.

Conceptually:

    main()
       |
       +----> initialize()
       |          |
       |          +---- function executes
       |
       v
    process_data()

Step Over is useful when I already understand the called function and do not need to inspect its internal implementation.

---

# 8. Step Out

Step Out is used when I am currently inside a function and want to return to the calling function.

Example:

    main()
      |
      v
    function_A()
      |
      v
    function_B()
      |
      v
    Current execution

If I am debugging inside `function_B()` and use Step Out, the debugger completes the current function and returns to the caller.

Conceptually:

    function_B()
         |
         | Step Out
         v
    function_A()

Step Out is useful when:

- I entered a function accidentally.
- I have finished analyzing the current function.
- I want to return to the higher-level program flow.

---

# 9. Breakpoints

A breakpoint stops program execution when the processor reaches a particular location.

For example:

```c
if(button_pressed)
{
    LED_ON();
}
```

A breakpoint can be placed on:

```c
LED_ON();
```

The execution becomes:

    Firmware Running
          |
          v
    Breakpoint reached
          |
          v
       CPU stops
          |
          +----> Inspect variables
          |
          +----> Inspect memory
          |
          +----> Inspect registers
          |
          +----> Inspect call stack

Breakpoints are useful when I already have a suspected code location.

They help answer questions such as:

- Is this line actually being executed?
- What are the variable values here?
- Which function called this function?
- What is the current processor state?

---

# 10. Breakpoint Management

During debugging, multiple breakpoints can be created.

For example:

    Breakpoint 1
    Breakpoint 2
    Breakpoint 3
    Breakpoint 4

STM32CubeIDE provides a Breakpoints view where breakpoints can be managed, enabled, disabled, removed, and skipped.

This is useful in larger firmware projects where many execution locations need to be investigated.

A breakpoint does not have to remain permanently active. It can be disabled temporarily and enabled again when required.

---

# 11. Disassembly

The processor does not directly execute C source code.

The C source code is compiled into machine instructions that the processor can execute.

The overall process is:

    C Source Code
          |
          v
       Compiler
          |
          v
      Assembly
          |
          v
    Machine Code
          |
          v
       CPU Execution

The Disassembly view allows the generated assembly instructions to be inspected.

For example:

```c
counter++;
```

will eventually become processor instructions that load, modify, and store the required data.

The exact assembly depends on:

- CPU architecture
- Compiler
- Compiler version
- Optimization level
- Data types
- Register allocation
- Source code structure

---

# 12. Why Disassembly is Important

Disassembly becomes especially important when source-level debugging is not enough.

It helps me understand:

- What instructions the CPU is actually executing
- How variables are loaded
- How variables are stored
- How memory is accessed
- How function calls are implemented
- How branches and conditions are implemented
- How compiler optimization changes the generated code

The relationship can be remembered as:

    C Code
       |
       v
    Compiler
       |
       v
    Assembly
       |
       v
    Machine Instructions
       |
       v
    CPU
       |
       v
    Hardware

This is particularly important in embedded systems because firmware interacts directly with hardware registers and memory-mapped peripherals.

---

# 13. Call Stack

The Call Stack shows the sequence of function calls that led to the current execution point.

Consider:

```c
int main(void)
{
    task1();
}

void task1(void)
{
    process_data();
}

void process_data(void)
{
    calculate();
}

void calculate(void)
{
    // Current execution
}
```

The Call Stack can conceptually look like:

    calculate()
    process_data()
    task1()
    main()

This tells me how execution reached the current function.

---

# 14. Why Call Stack is Important

The Call Stack is particularly useful when:

- A function is called unexpectedly.
- A fault occurs.
- A program crashes.
- Functions are deeply nested.
- I need to understand the execution path.
- Investigating processor exceptions.

For example:

    main()
       |
       v
    communication_task()
       |
       v
    receive_packet()
       |
       v
    process_packet()
       |
       v
    calculate()
       |
       v
    Fault

The Call Stack helps reconstruct this execution path.

This becomes particularly valuable when investigating faults such as HardFaults.

---

# 15. Expression Window

The Expression Window allows specific variables or expressions to be monitored during debugging.

For example:

```c
uint32_t counter;
uint32_t status;
```

I can monitor:

```text
counter
status
buffer[0]
```

I can also inspect expressions involving peripheral registers, for example:

```c
GPIOA->IDR
GPIOA->ODR
```

This is useful when I want to continuously monitor selected information without modifying the source code simply to print debugging information.

---

# 16. Variable Window

The Variable Window displays variables and their current values during a debugging session.

For example:

```c
uint32_t counter = 100;
uint8_t status = 1;
```

The debugger may show:

    Variable       Value
    --------------------
    counter        100
    status         1

This helps investigate:

- Incorrect calculations
- Unexpected variable changes
- Function parameters
- Pointer values
- Buffer contents
- Control variables
- State-machine variables

The Variable Window is especially useful when stepping through code.

---

# 17. Memory Browser

The Memory Browser allows raw memory contents to be inspected at specific memory addresses.

For example:

    Address       Data
    -------------------------
    0x20000000    0x00000010
    0x20000004    0x00000001
    0x20000008    0x000000FF

This is extremely important in embedded systems because RAM, Flash, and peripheral registers are mapped into the processor's address space.

The Memory Browser can be used to inspect:

- RAM
- Flash
- Arrays
- Buffers
- Stack memory
- Variables
- Memory-mapped peripheral registers

---

# 18. Memory-Mapped Peripheral Debugging

STM32 peripherals are accessible through memory-mapped registers.

Conceptually:

    CPU
     |
     v
    System Bus
     |
     +--------------------+
     |                    |
     v                    v
    RAM             Peripheral Registers
                          |
                  +-------+-------+
                  |       |       |
                  v       v       v
                 GPIO    RCC    USART

For example:

```c
GPIOA->IDR
GPIOA->ODR
RCC->AHB1ENR
```

can be inspected during debugging.

This allows me to compare:

    C Source Code
          |
          v
    Register Access
          |
          v
    Register Address
          |
          v
    Actual Register Value
          |
          v
    Hardware Behavior

For example, if I configure a GPIO but the pin does not behave as expected, I can inspect the actual GPIO and RCC register values instead of relying only on the source code.

---

# 19. Data Watchpoints

A Data Watchpoint is different from a normal breakpoint.

A breakpoint is associated with a code location:

    "Stop when execution reaches this instruction."

A data watchpoint is associated with a data location:

    "Stop when this data is accessed or modified."

For example:

```c
uint32_t counter;
```

Suppose I expect:

```text
counter = 100
```

but during execution it unexpectedly becomes:

```text
counter = 500
```

If I do not know which instruction modified it, I can use a data watchpoint on `counter`.

The process becomes:

    Unexpected variable value
              |
              v
      Set data watchpoint
              |
              v
        Run firmware
              |
              v
      Variable is accessed
      or modified
              |
              v
          CPU stops
              |
              v
      Inspect current code
              |
              v
       Find source of change

This is useful for finding:

- Unexpected writes
- Memory corruption
- Pointer-related bugs
- Buffer corruption
- Variables modified by unexpected code

---

# 20. Breakpoint vs Data Watchpoint

The difference can be remembered very simply.

## Breakpoint

```text
Known CODE location
        |
        v
   CPU stops there
```

## Data Watchpoint

```text
Known DATA / MEMORY location
        |
        v
Detect access or modification
        |
        v
      CPU stops
```

For example, if I know this line is suspicious:

```c
counter++;
```

I can place a breakpoint on that source line.

But if I only know that `counter` is changing unexpectedly and I do not know which instruction is responsible, a data watchpoint is more appropriate.

---

# 21. Combining Debugging Techniques

Professional embedded debugging normally requires combining several techniques rather than relying on one method.

For example:

    Problem
      |
      v
    Breakpoint
      |
      v
    Inspect Variables
      |
      v
    Inspect Memory
      |
      v
    Inspect Peripheral Registers
      |
      v
    Check Call Stack
      |
      v
    Inspect Disassembly
      |
      v
    Use Data Watchpoint if data changes unexpectedly
      |
      v
    Use SWV if runtime behavior or timing must be observed
      |
      v
    Root Cause

Each debugging method provides a different level of visibility.

---

# 22. Example: Debugging an STM32 GPIO Problem

Suppose the firmware contains:

```c
while(1)
{
    uint8_t pin_status = GPIOA->IDR & (1 << 0);

    if(pin_status)
    {
        GPIOA->ODR |= (1 << 5);
    }
    else
    {
        GPIOA->ODR &= ~(1 << 5);
    }
}
```

Suppose the LED does not respond correctly.

I can debug the problem systematically.

## Step 1 - Breakpoint

Place a breakpoint inside the `if` statement.

Question:

```text
Is this code executing?
```

---

## Step 2 - Variable Inspection

Inspect:

```text
pin_status
```

Question:

```text
Is the firmware reading the expected input state?
```

---

## Step 3 - Peripheral Register Inspection

Inspect:

```text
GPIOA->IDR
GPIOA->ODR
```

Question:

```text
Are the actual GPIO registers containing the expected values?
```

---

## Step 4 - RCC Register Inspection

Check whether the required peripheral clock is enabled.

For example:

```text
RCC->AHB1ENR
```

Question:

```text
Is the GPIO peripheral clock actually enabled?
```

---

## Step 5 - Call Stack

If execution unexpectedly enters another function or an exception handler, inspect the Call Stack.

Question:

```text
How did execution reach this point?
```

---

## Step 6 - Disassembly

If the source-level behavior does not explain the problem, inspect the generated assembly.

Question:

```text
What instructions is the CPU actually executing?
```

---

## Step 7 - Data Watchpoint

If a variable unexpectedly changes, place a watchpoint on the variable.

Question:

```text
Which instruction is modifying this data?
```

---

## Step 8 - SWV

If the problem appears to be related to timing or continuously changing runtime behavior, use SWV/Data Trace.

Question:

```text
What is happening while the firmware is continuously running?
```

This creates a structured debugging process instead of guessing.

---

# 23. Choosing the Correct Debugging Technique

| Problem | Useful Technique |
|---|---|
| Observe runtime behavior | SWV / Data Trace |
| Print runtime debug information | ITM / SWV |
| Execute firmware one statement at a time | Single Stepping |
| Enter a function | Step Into |
| Execute a function without entering it | Step Over |
| Return from the current function | Step Out |
| Check whether a line executes | Breakpoint |
| Manage multiple breakpoints | Breakpoints View |
| Understand CPU instructions | Disassembly |
| Understand function execution history | Call Stack |
| Monitor selected expressions | Expression Window |
| Inspect variables | Variable Window |
| Inspect raw memory | Memory Browser |
| Inspect peripheral register state | Memory / Peripheral Register View |
| Find unexpected data modification | Data Watchpoint |
| Investigate timing-sensitive runtime behavior | SWV / Data Trace |

---

# 24. Complete Embedded Debugging Workflow

A practical debugging workflow can be organized as:

```text
                    PROBLEM
                       |
                       v
                Reproduce Problem
                       |
                       v
                Observe Behavior
                       |
             +---------+---------+
             |                   |
             v                   v
       Runtime Problem      Code Flow Problem
             |                   |
             v                   v
          SWV/Trace          Breakpoint
                                 |
                                 v
                         Single Stepping
                                 |
                +----------------+----------------+
                |                |                |
                v                v                v
            Step Into        Step Over        Step Out
                |
                v
        Variables / Expressions
                |
                v
            Call Stack
                |
                v
         Memory Browser
                |
                v
       Peripheral Registers
                |
                v
       Data Watchpoint
                |
                v
           Disassembly
                |
                v
            Root Cause
```

The important point is that these techniques are complementary.

---

# 25. Debugging at Different Abstraction Levels

One of the most important concepts I learned is that embedded debugging can be performed at different abstraction levels.

```text
+----------------------------------+
|          Application Logic      |
+----------------------------------+
                |
                v
+----------------------------------+
|          C Source Code           |
|   Breakpoints / Stepping         |
+----------------------------------+
                |
                v
+----------------------------------+
|       Variables / Expressions    |
+----------------------------------+
                |
                v
+----------------------------------+
|       Memory / Registers         |
|       Memory Browser             |
+----------------------------------+
                |
                v
+----------------------------------+
|       Assembly Instructions      |
|          Disassembly             |
+----------------------------------+
                |
                v
+----------------------------------+
|        CPU Execution             |
+----------------------------------+
                |
                v
+----------------------------------+
|        Physical Hardware         |
+----------------------------------+
```

The closer I move toward the CPU and hardware level, the more detailed the debugging information becomes.

---

# 26. Important Debugging Mindset

A professional debugging process should be based on evidence rather than assumptions.

Instead of thinking:

```text
"The GPIO is not working, so I should change the code."
```

I should ask:

```text
1. Is the code executing?
2. Are the variables correct?
3. Are the peripheral registers correct?
4. Is the peripheral clock enabled?
5. Is the execution path correct?
6. Is any unexpected code modifying the data?
7. What instructions is the CPU executing?
8. Is the problem timing-related?
```

The corresponding debugging tools are:

```text
Code execution
    -> Breakpoint

Program flow
    -> Step Into / Step Over / Step Out

Variables
    -> Variable Window

Expressions
    -> Expression Window

Function history
    -> Call Stack

Memory
    -> Memory Browser

Peripheral state
    -> Register / Memory View

Unexpected data modification
    -> Data Watchpoint

CPU-level behavior
    -> Disassembly

Runtime / timing behavior
    -> SWV / Data Trace
```

---

# 27. Key Learning

The main lesson from this section is that embedded debugging is not simply about placing a breakpoint.

A firmware developer needs to understand multiple views of the same execution:

```text
                Firmware
                   |
       +-----------+-----------+
       |           |           |
       v           v           v
   Program       Runtime     Hardware
    Flow          Data        State
       |           |           |
       v           v           v
 Breakpoints      SWV       Registers
 Stepping        Trace      Memory
       |                       |
       v                       v
 Call Stack              Memory Browser
       |
       v
 Variables / Expressions
       |
       v
 Disassembly
       |
       v
 Data Watchpoints
```

Each technique answers a different question.

### SWV / Data Trace

Answers:

> What is happening while the firmware is running?

### Breakpoint

Answers:

> Does execution reach this code location?

### Step Into

Answers:

> What happens inside this function?

### Step Over

Answers:

> Can I execute this function without inspecting its internal code?

### Step Out

Answers:

> How do I return from this function to its caller?

### Call Stack

Answers:

> How did execution reach the current function?

### Variable Window

Answers:

> What values do my variables currently contain?

### Expression Window

Answers:

> What is the current value of this selected expression?

### Memory Browser

Answers:

> What data is actually stored at this memory address?

### Data Watchpoint

Answers:

> Which execution causes this data to change or be accessed?

### Disassembly

Answers:

> What instructions is the processor actually executing?

---

# 28. Final Summary

In this lesson, I studied the major debugging techniques used during embedded firmware development with STM32CubeIDE.

The topics covered were:

- Serial Wire Viewer (SWV)
- ITM
- SWV Data Tracing
- SWV ITM Data Console
- Single Stepping
- Step Into
- Step Over
- Step Out
- Breakpoints
- Breakpoint Management
- Disassembly
- Call Stack
- Expression Window
- Variable Window
- Memory Browser
- Memory-mapped Peripheral Debugging
- Data Watchpoints
- Combining multiple debugging techniques
- Systematic debugging workflow

The most important concept I learned is:

```text
DO NOT GUESS THE BUG.
OBSERVE THE SYSTEM.
```

A professional debugging process should move from:

```text
Problem
   ↓
Reproduce
   ↓
Observe
   ↓
Locate
   ↓
Inspect
   ↓
Trace
   ↓
Verify
   ↓
Root Cause
```

By using breakpoints, stepping, variables, expressions, memory inspection, call-stack analysis, data watchpoints, disassembly, and SWV tracing together, firmware behavior can be investigated from the high-level C code down to the actual processor instructions and hardware registers.

This forms the foundation for systematic embedded firmware debugging.

---

# References

- STMicroelectronics - STM32CubeIDE User Guide
- STM32CubeIDE Debug Perspective
- STM32CubeIDE SWV and ITM Debugging Documentation
- STM32CubeIDE Memory and Disassembly Views
- STM32CubeIDE Breakpoint and Debugging Documentation
