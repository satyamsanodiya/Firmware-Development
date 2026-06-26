# C Runtime I/O and Buffer Management

---

# Overview

This module explores the internal working of the **C Runtime Library (CRT)** Input/Output subsystem using host-side applications developed in **STM32CubeIDE (MinGW GCC)**.

Although firmware executing on STM32 microcontrollers typically communicates through peripherals such as **UART**, **USB**, **CAN**, or **SWO**, these communication interfaces frequently rely on the same standard C library functions (`printf()`, `scanf()`, `putchar()`, and `getchar()`) after being redirected to the desired peripheral.

Therefore, understanding how the C Runtime Library manages **input buffering**, **output buffering**, and **formatted I/O** provides an essential foundation before implementing embedded communication drivers.

Rather than simply learning how to use library functions, this module focuses on understanding the internal execution flow between the application, runtime library, input/output buffers, and the console.

---

# Learning Objectives

After completing this module, I gained a practical understanding of the following concepts:

- Standard Input (`stdin`)
- Standard Output (`stdout`)
- Standard Error (`stderr`)
- Input Buffering
- Output Buffering
- Line Buffering
- Buffer Flushing
- Formatted Input using `scanf()`
- Formatted Output using `printf()`
- Character Input using `getchar()`
- Character Output using `putchar()`
- Scientific Notation Formatting
- ASCII Character Representation
- Runtime Buffer Management
- Console Synchronization

These concepts form the basis for implementing UART communication on STM32 microcontrollers where `printf()` and `scanf()` are commonly redirected to UART peripherals.

---

# Standard Streams

The C Runtime Library provides three standard streams that are automatically available to every C application.

| Stream | Description |
|---------|-------------|
| stdin | Standard Input Stream (Keyboard) |
| stdout | Standard Output Stream (Console Display) |
| stderr | Standard Error Stream |

The runtime library internally manages these streams using software buffers before communicating with the operating system.

---

# C Runtime I/O Architecture

```

```
                    Keyboard
                        │
                        ▼
                 +---------------+
                 |    stdin      |
                 | Input Buffer  |
                 +---------------+
                        │
                        ▼
              scanf() / getchar()
                        │
                        ▼
                  Application Code
                        │
                        ▼
             printf() / putchar()
                        │
                        ▼
                 +---------------+
                 |    stdout     |
                 | Output Buffer |
                 +---------------+
                        │
                        ▼
                  Console Window
```

The application never communicates directly with the keyboard or display.

Instead, all input and output operations are managed through the C Runtime Library.

---

# Understanding Output Buffering

Whenever a program executes

```c
printf("Enter Number : ");
```

the output is **not immediately displayed** on the console.

Instead, the runtime library first copies the characters into the **stdout output buffer**.

This improves execution efficiency because multiple characters can be transferred together instead of performing a system call for every individual character.

---

# Output Buffer Flow

```

```
              printf()

                  │

                  ▼

      +------------------------+
      |     stdout Buffer      |
      | "Enter Number :"       |
      +------------------------+

                  │

      (Still inside buffer)

                  │

                  ▼

        Console shows nothing
```

At this stage the prompt has been generated, but it has not yet been transferred to the console.

---

# When Does stdout Flush?

The output buffer is transferred to the console whenever one of the following conditions occurs.

- A newline character (`\n`) is printed.
- The output buffer becomes full.
- `fflush(stdout)` is called.
- The application terminates normally.

Only after one of these events does the console display the buffered text.

---

# Using fflush(stdout)

Consider the following example.

```c
printf("Enter Number : ");
scanf("%d",&number);
```

Some runtime environments use buffered output.

Therefore the prompt may still remain inside the output buffer while `scanf()` immediately starts waiting for keyboard input.

As a result, the user is asked for input before seeing the prompt.

To solve this problem,

```c
printf("Enter Number : ");
fflush(stdout);
scanf("%d",&number);
```

is used.

The call to

```c
fflush(stdout);
```

forces the runtime library to immediately transfer all pending output to the console.

---

# Output Buffer Execution Flow

```

```
printf()

        │

        ▼

stdout Buffer

        │

fflush(stdout)

        │

        ▼

Operating System

        │

        ▼

Console Display

        │

        ▼

User reads prompt
```

---

# Why is Output Buffering Used?

Without buffering,

every call to `printf()` would require the operating system to immediately access the display device.

Frequent system calls significantly reduce performance.

Instead,

```

```
Application

↓

stdout Buffer

↓

Operating System

↓

Console
```

allows multiple characters to be grouped together before transmission.

This reduces overhead and improves execution efficiency.

---

# Practical Example

Program

```c
printf("Temperature = ");
printf("%d",temp);
printf(" C");
```

Without buffering,

three separate console operations may occur.

With buffering,

the runtime combines them internally as

```

```
Temperature = 25 C
```

before sending the entire string to the operating system.

This significantly improves performance.

---

# Key Observations

- `printf()` does **not always display text immediately.**
- Output first enters the stdout buffer.
- The runtime decides when the buffer should be flushed.
- `fflush(stdout)` forces immediate transmission.
- Buffering reduces unnecessary operating system calls.
- This mechanism improves execution efficiency.

---
