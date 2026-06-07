# STM32F446RE - ITM Debug Print using SWD

## Objective

Demonstrate real-time debugging output using the Instrumentation Trace Macrocell (ITM) and Single Wire Debug (SWD) interface on the STM32F446RE Nucleo development board.

---

## Hardware Used

* STM32F446RE Nucleo Board
* ST-LINK Debugger (On-board)
* USB Cable
* STM32CubeIDE

---

## Software Environment

* STM32CubeIDE
* ARM GCC Toolchain
* ST-LINK GDB Server

---

## Background

Traditional embedded debugging often relies on UART peripherals for diagnostic messages. However, UART requires dedicated pins and additional hardware configuration.

ARM Cortex-M processors provide an Instrumentation Trace Macrocell (ITM), which enables lightweight debugging and real-time data tracing through the SWD interface without consuming UART resources.

---

## Debug Interfaces

### JTAG

JTAG is a standard debugging interface that uses multiple signal lines:

* TCK
* TMS
* TDI
* TDO

Advantages:

* Advanced debugging capabilities
* Boundary scan testing

Disadvantages:

* Requires multiple pins

---

### SWD (Single Wire Debug)

SWD is an ARM-specific debugging interface.

Signals:

* SWCLK
* SWDIO

Advantages:

* Uses fewer pins
* Faster setup
* Widely supported by STM32 devices

The STM32F446RE Nucleo board uses the on-board ST-LINK debugger connected through the SWD interface.

---

### ITM (Instrumentation Trace Macrocell)

ITM is a hardware debugging feature available inside ARM Cortex-M processors.

Features:

* Real-time trace output
* Low CPU overhead
* No UART hardware required
* Debug messages transmitted through SWD

ITM is commonly used for:

* Firmware debugging
* Runtime variable monitoring
* Performance analysis
* Software trace logging

---

## Firmware Implementation

Example code:

```c
#include <stdio.h>

int main(void)
{
    printf("hello world\r\n");

    for(;;);
}
```

The printf output is redirected to the SWV ITM Data Console within STM32CubeIDE.

---

## Debug Configuration

### Debug Interface

```text
SWD
```

### Trace Configuration

Enable:

```text
Serial Wire Viewer (SWV)
```

Configure:

```text
Core Clock Frequency
```

Open:

```text
SWV ITM Data Console
```

Enable:

```text
ITM Stimulus Port 0
```

---

## Results

The message:

```text
hello world
```

was successfully displayed in the SWV ITM Data Console through the SWD interface.

This confirms:

* Successful STM32 program execution
* Working SWD connection
* Proper ITM configuration
* Functional printf redirection

---

## Key Concepts Learned

* ARM Cortex-M4 debugging architecture
* JTAG interface basics
* SWD interface operation
* ITM trace functionality
* SWV ITM Data Console usage
* printf redirection techniques
* STM32CubeIDE debugging workflow


---

## Learning Outcome

Successfully implemented real-time firmware debugging using the ARM Cortex-M4 Instrumentation Trace Macrocell (ITM) over the SWD interface on the STM32F446RE Nucleo board.

This project established a professional debugging workflow that will be used throughout future STM32 peripheral-driver and embedded-system development projects.

