# GPIO Register-Level Programming on STM32F446RE

## Objective

The objective of this exercise was to understand how a GPIO peripheral is controlled at the register level without using HAL libraries.

The implementation was performed on the STM32F446RE Nucleo board by directly accessing peripheral registers through their memory-mapped addresses and controlling the onboard LED connected to Port D Pin 12.

This exercise helped me understand how ARM Cortex-M microcontrollers interact with hardware peripherals through memory-mapped registers.

---

# Concepts Explored

## 1. STM32 Memory Map

Before controlling any peripheral, I studied the STM32F446RE memory map from the Reference Manual.

The memory map defines the address space allocated to:

* Flash Memory
* SRAM
* Peripheral Registers
* System Memory
* Cortex-M Core Peripherals

Understanding the memory map is essential because all peripherals are accessed through fixed memory addresses.

---

## 2. Memory-Mapped Peripheral Registers

STM32 peripherals are mapped into the processor address space.

Each peripheral contains a group of registers that can be accessed through specific addresses.

Examples:

* RCC Register Block
* GPIO Register Block
* USART Register Block
* SPI Register Block
* ADC Register Block

This allows the CPU to configure hardware simply by reading or writing memory locations.

---

## 3. ARM Cortex-M Bus Architecture

I studied how the Cortex-M4 communicates with peripherals using:

* 32-bit Address Bus
* 32-bit Data Bus

The processor places an address on the address bus and exchanges data through the data bus.

This mechanism enables direct access to peripheral registers.

---

## 4. RCC Clock Control Mechanism

STM32 peripherals are disabled after reset to reduce power consumption.

Before using any peripheral, its clock must be enabled through the Reset and Clock Control (RCC) module.

For GPIO Port D:

* RCC Base Address was identified from the Reference Manual.
* AHB1ENR register was used.
* Bit 3 (GPIODEN) was set to enable the clock for Port D.

Without enabling the clock, all GPIO registers remain inaccessible.

Key Learning:

A peripheral is effectively inactive until its clock source is enabled.

---

## 5. GPIO Configuration Flow

The following sequence was used to configure Port D Pin 12:

### Step 1: Enable GPIO Clock

Enable Port D clock using RCC->AHB1ENR.

### Step 2: Configure Pin Mode

Modify GPIOx_MODER register.

Pin 12 mode bits:

MODER[25:24]

Configuration:

01 = General Purpose Output

### Step 3: Write Output Data

Use GPIOx_ODR register.

ODR12 = 1

This drives the output pin HIGH.

---

## 6. Register-Level Bit Manipulation

All register modifications were performed using bitwise operations.

Operations used:

* AND (&)
* OR (|)
* Left Shift (<<)
* Bit Masking

Example:

Clear existing mode bits:

```c
GPIO_MODER &= ~(3 << 24);
```

Set output mode:

```c
GPIO_MODER |= (1 << 24);
```

These techniques are fundamental for low-level firmware development.

---

## 7. Register Debugging Using STM32CubeIDE

The Register View in STM32CubeIDE was used to observe hardware register changes in real time.

The following registers were monitored:

* RCC_AHB1ENR
* GPIOD_MODER
* GPIOD_ODR

This helped verify that:

* Clock enable bit was correctly set.
* GPIO mode bits were updated.
* Output register reflected the expected state.

---

## Results

Successfully controlled the onboard LED using direct register access without HAL libraries.

Verified:

* RCC clock enable sequence
* GPIO configuration process
* Memory-mapped register access
* Bit-level manipulation
* Register debugging techniques

---

## Key Learning Outcomes

* Understanding of STM32 memory architecture
* Practical use of Reference Manual and Datasheet
* Peripheral clock management
* Register-level GPIO programming
* ARM Cortex-M memory-mapped I/O concept
* Debugging hardware registers using STM32CubeIDE
* Efficient use of bitwise operations in embedded firmware

This exercise established the foundation required for advanced peripheral development such as UART, SPI, I2C, Timers, ADC, CAN, and DMA at the register level.
