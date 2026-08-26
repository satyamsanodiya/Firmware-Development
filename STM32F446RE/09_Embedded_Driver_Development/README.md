# Embedded Driver Development Using Embedded C and STM32F446RE

## Overview

In this section, I studied how to develop **low-level embedded drivers for the STM32F446RE using Embedded C and direct register-level programming**.

The objective was not only to make GPIO, I2C, SPI, USART and other peripherals work, but to understand the complete relationship between:

- MCU hardware
- Peripheral registers
- Memory-mapped addresses
- Embedded C
- Bit manipulation
- Configuration structures
- Handle structures
- Driver functions
- Driver APIs
- Application code

The main idea of this section is:

    Hardware
        ↓
    Registers
        ↓
    Embedded C
        ↓
    Driver
        ↓
    API
        ↓
    Application

The purpose is to understand how embedded firmware communicates with MCU hardware at the register level and how those low-level operations are organized into reusable and maintainable drivers.

The topics covered are:

- What an embedded driver is
- Why drivers are required
- Driver architecture
- Hardware abstraction
- Register-level programming
- Memory-mapped peripheral access
- MCU-specific header files
- Peripheral register structures
- Peripheral base addresses
- Register offsets
- Peripheral pointers
- `volatile`
- Bit manipulation
- Read-modify-write operations
- RCC clock control
- Peripheral clock enable and disable
- Configuration structures
- Handle structures
- Driver APIs
- API prototypes
- Driver header files
- Driver source files
- GPIO driver architecture
- GPIO initialization
- GPIO input/output operations
- GPIO alternate functions
- GPIO interrupt architecture
- I2C driver architecture
- SPI driver architecture
- USART driver architecture
- Interrupt-driven drivers
- Driver testing
- Register debugging
- Hardware debugging
- Driver development workflow
- Driver layering
- Reusable driver design

---

# 1. What Is an Embedded Driver?

An embedded driver is a software layer that controls a specific hardware peripheral and provides a clean interface for the application.

Without a driver, application code may directly manipulate hardware registers:

    Application
         |
         v
    Direct Register Access
         |
         v
    Peripheral Hardware

With a driver:

    Application
         |
         v
      Driver API
         |
         v
    Driver Implementation
         |
         v
    Peripheral Registers
         |
         v
    Peripheral Hardware

The driver therefore acts as an interface between the application and the MCU hardware.

For example, instead of writing register operations directly inside `main.c`:

    GPIOA->ODR |= (1 << 5);

the application can use:

    GPIO_WriteToOutputPin(GPIOA, 5, GPIO_PIN_SET);

The application describes what it wants to do, while the driver handles how the hardware should be configured.

---

# 2. Why Driver Development Is Important

In small programs, directly accessing registers may appear simple.

However, as the firmware becomes larger, direct register access inside application code becomes difficult to maintain.

For example:

    main.c
       |
       +---- GPIO registers
       |
       +---- I2C registers
       |
       +---- SPI registers
       |
       +---- USART registers
       |
       +---- RCC registers
       |
       +---- Interrupt registers

This mixes application logic with hardware implementation.

A driver-based architecture separates these responsibilities:

    Application
        |
        | Application logic
        v
    Driver API
        |
        | Hardware control
        v
    Peripheral Driver
        |
        | Register access
        v
    MCU Hardware

This provides:

- Modularity
- Reusability
- Maintainability
- Easier debugging
- Hardware abstraction
- Cleaner application code
- Easier peripheral replacement
- Better understanding of hardware/software interaction

---

# 3. Overall Driver Architecture

The complete architecture studied in this section is:

                    APPLICATION
                         |
                         | API calls
                         v
                 DRIVER API LAYER
                         |
                         v
              DRIVER IMPLEMENTATION
                         |
                         v
                MCU-SPECIFIC LAYER
                         |
                         v
                  REGISTER ACCESS
                         |
                         v
                    MCU HARDWARE

For example:

    Application
         |
         v
    GPIO_WriteToOutputPin()
         |
         v
    GPIO Driver
         |
         v
    GPIOA->ODR / BSRR
         |
         v
    GPIO Hardware
         |
         v
    Physical GPIO Pin

The same architecture can later be applied to:

    GPIO
      ↓
    I2C
      ↓
    SPI
      ↓
    USART
      ↓
    Other STM32 peripherals

---

# 4. Hardware-to-Software Relationship

The most important mental model for driver development is:

    Hardware
        ↓
    Hardware Specification
        ↓
    Register Map
        ↓
    Register Addresses
        ↓
    C Register Structures
        ↓
    Driver Functions
        ↓
    Driver API
        ↓
    Application

The reference manual tells me what the hardware provides.

The register map tells me how the hardware is controlled.

Embedded C provides a way to access those registers.

The driver organizes those register accesses into reusable functions.

The API allows application code to use the driver without knowing the internal register implementation.

---

# 5. Register-Level Programming

A microcontroller peripheral is controlled through registers.

For example, a GPIO peripheral contains registers such as:

    GPIO
     |
     +---- MODER
     +---- OTYPER
     +---- OSPEEDR
     +---- PUPDR
     +---- IDR
     +---- ODR
     +---- BSRR
     +---- LCKR
     +---- AFRL
     +---- AFRH

Each register contains multiple bit fields.

The firmware modifies those bits to control the hardware.

For example:

    Register
    31                         0
    +--------------------------+
    |       Register Bits      |
    +--------------------------+

If a particular hardware configuration is controlled by bits 10:8, the driver must:

    1. Identify the bit field
    2. Clear the existing value
    3. Shift the required value
    4. Write the value
    5. Verify the resulting hardware state

This is the foundation of register-level driver development.

---

# 6. Memory-Mapped Peripheral Concept

STM32 peripherals are memory mapped.

This means the CPU accesses peripheral registers through addresses in the MCU address space.

Conceptually:

                    Cortex-M4
                        |
                        | Address + Data
                        v
                   Memory System
                        |
          +-------------+-------------+
          |                           |
          v                           v
       Memory                    Peripheral Region
          |                           |
          v                           +---- GPIO
     Flash / SRAM                    +---- RCC
                                     +---- SPI
                                     +---- I2C
                                     +---- USART
                                     +---- TIM
                                     +---- Other peripherals

Therefore:

    Peripheral Register
          =
    Memory Address

The CPU can read or write the peripheral register using normal load/store operations.

---

# 7. Peripheral Base Address

Each peripheral has a base address.

For example, the GPIOA peripheral begins at a specific address in the STM32F446RE memory map.

Conceptually:

    Peripheral Base Address
              +
        Register Offset
              =
        Register Address

For example:

    GPIOA Base Address
          +
    MODER Offset
          =
    GPIOA MODER Address

Similarly:

    RCC Base Address
          +
    CR Register Offset
          =
    RCC_CR Address

This is the basic address-calculation concept used throughout register-level programming.

---

# 8. Register Offset

A peripheral normally contains several registers located at different offsets from the peripheral base address.

Conceptually:

    Peripheral Base Address
             |
             +---- Offset 0x00 --> Register 1
             |
             +---- Offset 0x04 --> Register 2
             |
             +---- Offset 0x08 --> Register 3
             |
             +---- Offset 0x0C --> Register 4

Therefore:

    Register Address =
    Peripheral Base Address + Register Offset

The exact offsets must always be obtained from the STM32F446RE reference manual.

---

# 9. Direct Register Pointer

Once the register address is known, it can be accessed using a pointer.

Example:

    uint32_t *pRccReg;

The pointer can be assigned to a register address:

    pRccReg = (uint32_t *)RCC_REGISTER_ADDRESS;

Then:

    *pRccReg

means:

    Access the 32-bit register located at that address.

For example:

    *pRccReg |= (1U << bit);

means:

    Read the register
        ↓
    Set the selected bit
        ↓
    Write the register back

This is a fundamental technique in register-level Embedded C.

---

# 10. Peripheral Register Structures

Instead of defining every register as an individual pointer, a peripheral register block can be represented using a C structure.

For example:

    typedef struct
    {
        volatile uint32_t MODER;
        volatile uint32_t OTYPER;
        volatile uint32_t OSPEEDR;
        volatile uint32_t PUPDR;
        volatile uint32_t IDR;
        volatile uint32_t ODR;
        volatile uint32_t BSRR;
        volatile uint32_t LCKR;
        volatile uint32_t AFRL;
        volatile uint32_t AFRH;

    } GPIO_RegDef_t;

This structure represents the GPIO register layout in C.

Conceptually:

    GPIO_RegDef_t
          |
          +---- MODER
          +---- OTYPER
          +---- OSPEEDR
          +---- PUPDR
          +---- IDR
          +---- ODR
          +---- BSRR
          +---- LCKR
          +---- AFRL
          +---- AFRH

The order of the structure members must match the actual hardware register layout.

---

# 11. Why `volatile` Is Important

Peripheral registers are normally declared as `volatile`.

Example:

    volatile uint32_t IDR;

The reason is that hardware can change register values independently of normal software execution.

For example:

    External Signal
          |
          v
      GPIO Pin
          |
          v
     GPIO Hardware
          |
          v
       IDR Register
          |
          v
          CPU

The compiler must not assume that the value remains unchanged.

The `volatile` qualifier tells the compiler that every access to the register is significant and must actually occur.

This is especially important for:

- Peripheral registers
- Status registers
- Interrupt flags
- Hardware-controlled memory locations

---

# 12. Peripheral Pointer Mapping

Once the register structure is defined, it can be mapped to the peripheral base address.

Conceptually:

    GPIOA_BASE_ADDRESS
             |
             v
       GPIO_RegDef_t *
             |
             v
            GPIOA

Then:

    GPIOA->MODER

means:

    Access GPIOA MODER register.

Similarly:

    GPIOA->IDR

means:

    Access GPIOA input-data register.

And:

    GPIOA->ODR

means:

    Access GPIOA output-data register.

This provides a much cleaner way of writing register-level code.

---

# 13. MCU-Specific Header File

A central MCU-specific header is required to define the hardware resources.

For example:

    stm32f446x.h

This file can contain:

- Peripheral base addresses
- Bus base addresses
- Register definitions
- Peripheral structures
- Peripheral pointers
- RCC definitions
- Clock-control macros
- GPIO definitions
- Interrupt-related definitions

Conceptually:

    stm32f446x.h
          |
          +---- Memory Map
          |
          +---- Base Addresses
          |
          +---- Register Structures
          |
          +---- Peripheral Pointers
          |
          +---- RCC Definitions
          |
          +---- Clock Macros
          |
          +---- MCU-specific Definitions

The purpose of this layer is to provide the driver with knowledge about the target MCU.

---

# 14. Peripheral Driver Files

A peripheral driver is generally divided into two files.

For GPIO:

    stm32f446x_gpio_driver.h
    stm32f446x_gpio_driver.c

The header file contains the public interface.

The source file contains the implementation.

Conceptually:

    .h file
       |
       | Defines WHAT driver provides
       v
    Driver API
       |
       v
    .c file
       |
       | Defines HOW driver works
       v
    Register-level implementation

This separation is an important part of modular Embedded C development.

---

# 15. Driver Header File

The driver header file contains definitions required by the application and driver interface.

It can contain:

- Configuration macros
- Enumerations
- Configuration structures
- Handle structures
- API prototypes

For example:

    GPIO_PinConfig_t
    GPIO_Handle_t

and:

    GPIO_Init()
    GPIO_DeInit()
    GPIO_ReadFromInputPin()
    GPIO_WriteToOutputPin()
    GPIO_ToggleOutputPin()

The header therefore describes the interface available to the rest of the firmware.

---

# 16. Driver Source File

The driver source file contains the actual implementation.

For example:

    stm32f446x_gpio_driver.c

This file contains functions such as:

    void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
    {
        /* Register-level implementation */
    }

The implementation performs the actual operations on:

    RCC registers
    GPIO registers
    EXTI registers
    SYSCFG registers
    NVIC-related configuration

depending on the functionality being implemented.

---

# 17. Configuration Structure

A peripheral often has many configuration parameters.

Instead of passing every parameter individually to a function, a configuration structure can group them together.

For GPIO:

    typedef struct
    {
        uint8_t GPIO_PinNumber;
        uint8_t GPIO_PinMode;
        uint8_t GPIO_PinSpeed;
        uint8_t GPIO_PinPuPdControl;
        uint8_t GPIO_PinOPType;
        uint8_t GPIO_PinAltFunMode;

    } GPIO_PinConfig_t;

The structure describes how the GPIO pin should be configured.

Conceptually:

    GPIO_PinConfig_t
          |
          +---- Pin Number
          |
          +---- Pin Mode
          |
          +---- Pin Speed
          |
          +---- Pull-up / Pull-down
          |
          +---- Output Type
          |
          +---- Alternate Function

---

# 18. Why Configuration Structures Are Used

Without a configuration structure, a function could require many parameters:

    GPIO_Init(
        GPIOA,
        pin,
        mode,
        speed,
        pull,
        output_type,
        alternate_function
    );

This becomes difficult to manage.

With a configuration structure:

    GPIO_Handle_t
          |
          +---- GPIO Peripheral
          |
          +---- GPIO Configuration
                       |
                       +---- Pin
                       +---- Mode
                       +---- Speed
                       +---- Pull
                       +---- Output Type
                       +---- Alternate Function

The function can receive one handle containing everything required.

---

# 19. GPIO Handle Structure

A GPIO handle connects a GPIO peripheral with its configuration.

Example:

    typedef struct
    {
        GPIO_RegDef_t *pGPIOx;
        GPIO_PinConfig_t GPIO_PinConfig;

    } GPIO_Handle_t;

Conceptually:

    GPIO_Handle_t
          |
          +---- pGPIOx
          |       |
          |       +---- GPIOA
          |       +---- GPIOB
          |       +---- GPIOC
          |       +---- ...
          |
          +---- GPIO_PinConfig
                  |
                  +---- Pin
                  +---- Mode
                  +---- Speed
                  +---- Pull
                  +---- Output Type
                  +---- Alternate Function

The handle therefore identifies:

    Which peripheral?

and:

    How should it be configured?

---

# 20. Why Handles Are Useful

The same driver implementation should work for different instances of a peripheral.

For example:

    GPIOA
    GPIOB
    GPIOC
    GPIOD

Instead of creating separate driver functions:

    GPIOA_Init()
    GPIOB_Init()
    GPIOC_Init()
    GPIOD_Init()

the same function can be reused:

    GPIO_Init(&gpioHandle);

The handle specifies which GPIO peripheral is being configured.

This makes the driver reusable.

---

# 21. Driver API Concept

API stands for:

    Application Programming Interface

In a driver, the API is the set of functions that the application can call.

For example:

    GPIO_Init()
    GPIO_DeInit()
    GPIO_ReadFromInputPin()
    GPIO_ReadFromInputPort()
    GPIO_WriteToOutputPin()
    GPIO_WriteToOutputPort()
    GPIO_ToggleOutputPin()
    GPIO_IRQConfig()
    GPIO_IRQHandling()

The application uses these APIs without needing to know the internal register operations.

---

# 22. API Prototype

An API prototype tells the compiler and developer:

- Function name
- Return type
- Parameters
- Parameter types

For example:

    uint8_t GPIO_ReadFromInputPin(
        GPIO_RegDef_t *pGPIOx,
        uint8_t PinNumber
    );

This means:

    Function name:
        GPIO_ReadFromInputPin

    Input:
        GPIO peripheral pointer
        Pin number

    Return value:
        uint8_t

The prototype describes the interface.

The actual implementation is placed in the `.c` file.

---

# 23. API vs Implementation

This distinction is very important.

The API describes:

    WHAT the driver provides.

The implementation describes:

    HOW the driver performs the operation.

Conceptually:

    Application
         |
         | WHAT?
         v
    Driver API
         |
         | HOW?
         v
    Driver Implementation
         |
         v
    Registers
         |
         v
    Hardware

For example:

    GPIO_WriteToOutputPin()

is the API.

Inside that function, the driver may perform:

    Read register
        ↓
    Modify required bit
        ↓
    Write register
        ↓
    Hardware changes GPIO output

The application does not need to know these internal steps.

---

# 24. GPIO Driver Architecture

The GPIO driver can be represented as:

                       APPLICATION
                            |
                            v
                  GPIO DRIVER API
                            |
                            v
                    GPIO_Handle_t
                            |
                            v
                 GPIO Driver Functions
                            |
                            v
                     GPIO Registers
                            |
                            v
                     GPIO Hardware
                            |
                            v
                     Physical Pin

The GPIO driver is therefore the first major example of the driver architecture.

---

# 25. GPIO Clock Control

Before configuring a GPIO peripheral, its peripheral clock must be enabled.

The RCC controls the peripheral clock.

Conceptually:

                         RCC
                          |
          +---------------+---------------+
          |               |               |
          v               v               v
        GPIOA           GPIOB           GPIOC
          |               |               |
          v               v               v
       Hardware        Hardware        Hardware

If the GPIO clock is disabled, the peripheral cannot be configured or used normally.

Therefore:

    Application
         |
         v
    GPIO_Init()
         |
         v
    Enable GPIO Clock
         |
         v
    Configure GPIO
         |
         v
    Use GPIO

---

# 26. Clock Enable and Disable Macros

Clock-control operations can be abstracted using macros.

Example:

    GPIOA_PCLK_EN()
    GPIOB_PCLK_EN()
    GPIOC_PCLK_EN()

and:

    GPIOA_PCLK_DI()
    GPIOB_PCLK_DI()
    GPIOC_PCLK_DI()

The same concept can be applied to:

    GPIO
    I2C
    SPI
    USART
    SYSCFG
    Other peripherals

The purpose is to avoid repeatedly writing low-level RCC bit manipulation throughout the driver.

---

# 27. General Peripheral Clock Flow

For any peripheral:

    Peripheral Required
            |
            v
    Identify RCC Bus
            |
            v
    Find Clock Enable Register
            |
            v
    Find Peripheral Clock Bit
            |
            v
    Set Bit
            |
            v
    Peripheral Clock Enabled
            |
            v
    Configure Peripheral

The exact RCC register and bit position depend on the peripheral and MCU.

---

# 28. GPIO Initialization Process

The general GPIO initialization sequence is:

    Application
         |
         v
    GPIO_Init()
         |
         v
    Enable GPIO Peripheral Clock
         |
         v
    Read Configuration Structure
         |
         v
    Configure MODER
         |
         v
    Configure OTYPER
         |
         v
    Configure OSPEEDR
         |
         v
    Configure PUPDR
         |
         v
    Configure AFRL / AFRH if required
         |
         v
    GPIO Ready
         |
         v
    Application Uses GPIO

This is the general process followed by the driver.

---

# 29. GPIO Input Path

When an external signal is applied to a GPIO input:

    External Signal
          |
          v
    Physical GPIO Pin
          |
          v
    GPIO Input Logic
          |
          v
        IDR
          |
          v
    GPIO Driver
          |
          v
    GPIO_ReadFromInputPin()
          |
          v
    Application

The driver reads the appropriate bit from the input data register.

---

# 30. GPIO Output Path

When the application wants to control a GPIO output:

    Application
         |
         v
    GPIO_WriteToOutputPin()
         |
         v
    GPIO Driver
         |
         v
    ODR / BSRR
         |
         v
    GPIO Output Logic
         |
         v
    Physical GPIO Pin
         |
         v
    External Hardware

Example:

    MCU GPIO
       |
       v
      LED

or:

    MCU GPIO
       |
       v
    Chip Select

or:

    MCU GPIO
       |
       v
    Control Signal

---

# 31. GPIO Alternate Function

GPIO pins can also be connected internally to peripheral functions.

A physical pin can potentially provide functions such as:

    GPIO
    USART
    SPI
    I2C
    TIM
    MCO
    Other Alternate Functions

Conceptually:

                    Physical Pin
                         |
          +--------------+--------------+
          |              |              |
         GPIO           SPI           USART
          |              |              |
          +--------------+--------------+
                         |
                    Selected Function

Only the selected function controls the pin.

---

# 32. Alternate Function Configuration

To use a peripheral function on a GPIO pin, the firmware generally needs to:

    1. Configure the pin as Alternate Function mode.
    2. Select the correct Alternate Function number.
    3. Configure the required peripheral.

Conceptually:

    GPIO Pin
       |
       v
    MODER
       |
       v
    Alternate Function Mode
       |
       v
    AFRL / AFRH
       |
       v
    Peripheral Function
       |
       v
    Physical Signal

The exact alternate-function number must always be taken from the STM32F446RE datasheet alternate-function table.

---

# 33. GPIO Interrupt Architecture

GPIO interrupts involve multiple hardware blocks.

The general signal path is:

    External Signal
          |
          v
      GPIO Pin
          |
          v
         EXTI
          |
          v
       SYSCFG
          |
          v
         NVIC
          |
          v
      Cortex-M4
          |
          v
      IRQ Handler
          |
          v
    GPIO_IRQHandling()
          |
          v
      Application

This introduces several concepts:

- EXTI line
- SYSCFG routing
- Interrupt number
- NVIC configuration
- Interrupt priority
- Interrupt enable
- Pending flag
- IRQ handler
- Interrupt callback/application handling

---

# 34. Driver and Interrupt Relationship

A driver can expose an interrupt configuration API.

For example:

    GPIO_IRQConfig(
        IRQNumber,
        IRQPriority,
        EnorDi
    );

Then the interrupt handler can call:

    GPIO_IRQHandling(PinNumber);

Conceptually:

    Hardware Event
          |
          v
        EXTI
          |
          v
        NVIC
          |
          v
    CPU Interrupt
          |
          v
      IRQ Handler
          |
          v
    Driver IRQ Handler
          |
          v
      Application

This allows hardware events to reach application-level code through the driver.

---

# 35. Driver Layering

A clean firmware architecture can be represented as:

    +--------------------------------------+
    |              Application             |
    |                                      |
    |        main.c / application logic    |
    +--------------------------------------+
                     |
                     v
    +--------------------------------------+
    |               Driver API             |
    |                                      |
    | GPIO / I2C / SPI / USART APIs        |
    +--------------------------------------+
                     |
                     v
    +--------------------------------------+
    |          Driver Implementation       |
    |                                      |
    | Register configuration / operations  |
    +--------------------------------------+
                     |
                     v
    +--------------------------------------+
    |           MCU-Specific Layer         |
    |                                      |
    | Base addresses / register structures |
    +--------------------------------------+
                     |
                     v
    +--------------------------------------+
    |                Hardware              |
    +--------------------------------------+

This separation is one of the most important concepts in embedded driver development.

---

# 36. I2C Driver

The next major driver after GPIO is I2C.

The I2C driver introduces concepts such as:

- Peripheral clock
- GPIO alternate function
- SCL
- SDA
- Timing configuration
- START condition
- STOP condition
- Addressing
- ACK
- NACK
- Transmit
- Receive
- Status flags
- Error handling

The communication architecture is:

    Application
         |
         v
      I2C API
         |
         v
     I2C Driver
         |
         v
    I2C Registers
         |
         v
    I2C Peripheral
         |
         +---------- SCL
         |
         +---------- SDA
                      |
                      v
                  I2C Device

Possible I2C devices include:

    EEPROM
    IMU
    RTC
    Temperature Sensor
    ADC
    GPIO Expander

---

# 37. I2C Driver Flow

A simplified I2C transmit operation can be understood as:

    Application
         |
         v
    I2C_SendData()
         |
         v
    Configure peripheral
         |
         v
    Generate START
         |
         v
    Send address
         |
         v
    Check status
         |
         v
    Send data
         |
         v
    Check status
         |
         v
    Generate STOP
         |
         v
    Return to application

The actual implementation will depend on the STM32F446 I2C peripheral registers and status flags.

---

# 38. SPI Driver

SPI introduces synchronous serial communication.

Important concepts include:

- Master
- Slave
- SCK
- MOSI
- MISO
- NSS
- Clock polarity
- Clock phase
- SPI mode
- Baud-rate prescaler
- Data frame
- Transmit
- Receive

Conceptually:

                         SPI MASTER
                              |
             +----------------+----------------+
             |                |                |
             v                v                v
            SCK              MOSI             MISO
             |                |                |
             |                |                |
             +----------------+----------------+
                              |
                              v
                         SPI SLAVE

The driver controls the SPI peripheral through its registers.

---

# 39. SPI Clock Phase and Polarity

SPI communication depends heavily on clock configuration.

Two important parameters are:

    CPOL
    CPHA

Together they determine the SPI mode.

Conceptually:

    CPOL + CPHA
         |
         v
    SPI Clock Behaviour
         |
         v
    Sampling / Data Timing
         |
         v
    Correct Communication

The SPI driver must configure these parameters according to the requirements of the connected device.

---

# 40. USART Driver

USART introduces asynchronous serial communication.

Important configuration parameters include:

- Baud rate
- Word length
- Stop bits
- Parity
- Oversampling
- TX
- RX
- Interrupts

Communication path:

    Application
         |
         v
      USART API
         |
         v
    USART Driver
         |
         v
    USART Registers
         |
         v
    USART Peripheral
         |
         +--------- TX ---------> External Device
         |
         +--------- RX <--------- External Device

USART is also useful for:

- Debug messages
- Sensor communication
- Bootloaders
- Command interfaces
- PC communication

---

# 41. General Communication Driver Architecture

Although GPIO, I2C, SPI and USART have different hardware protocols, their software architecture follows the same basic model.

    Application
         |
         v
    Peripheral API
         |
         v
    Configuration
         |
         v
    Handle
         |
         v
    Driver Implementation
         |
         v
    Peripheral Registers
         |
         v
    Peripheral Hardware
         |
         v
    External Device

The major difference is the peripheral-specific register operations and protocol state machine.

---

# 42. Driver Development Process

The same development process will be followed for each peripheral.

    1. Study the MCU datasheet
             ↓
    2. Study the reference manual
             ↓
    3. Understand peripheral architecture
             ↓
    4. Identify the peripheral bus
             ↓
    5. Identify peripheral base address
             ↓
    6. Study register map
             ↓
    7. Identify register bit fields
             ↓
    8. Create register structure
             ↓
    9. Create peripheral pointer
             ↓
    10. Implement clock control
             ↓
    11. Create configuration structure
             ↓
    12. Create handle structure
             ↓
    13. Design API prototypes
             ↓
    14. Implement APIs
             ↓
    15. Build firmware
             ↓
    16. Program MCU
             ↓
    17. Debug registers
             ↓
    18. Verify peripheral operation
             ↓
    19. Verify physical hardware
             ↓
    20. Document the driver

This process converts information from the hardware documentation into working firmware.

---

# 43. Reference Manual to Driver Flow

The most important workflow is:

    Requirement
         |
         v
    Reference Manual
         |
         v
    Peripheral Chapter
         |
         v
    Register Description
         |
         v
    Register Offset
         |
         v
    Bit Field
         |
         v
    Embedded C
         |
         v
    Driver Function
         |
         v
    API
         |
         v
    Application

For example:

    Requirement:
    Configure GPIO as output

         ↓

    GPIO section of reference manual

         ↓

    MODER register

         ↓

    Correct MODER bit field

         ↓

    Register-level C code

         ↓

    GPIO_Init()

         ↓

    Application

This is the core workflow of register-level driver development.

---

# 44. Bit Manipulation

Register-level driver development requires extensive use of bitwise operators.

## Set a Bit

    REG |= (1U << bit);

Meaning:

    Read register
        ↓
    Set selected bit
        ↓
    Write register

## Clear a Bit

    REG &= ~(1U << bit);

Meaning:

    Read register
        ↓
    Clear selected bit
        ↓
    Write register

## Toggle a Bit

    REG ^= (1U << bit);

Meaning:

    If bit = 0 → make it 1
    If bit = 1 → make it 0

## Modify a Bit Field

    REG &= ~(MASK << POSITION);
    REG |=  (VALUE << POSITION);

This is particularly important for multi-bit peripheral configuration fields.

---

# 45. Read-Modify-Write

Many peripheral registers contain multiple independent fields.

Therefore, directly writing the complete register can accidentally modify unrelated fields.

For example:

    REG = VALUE;

may overwrite other configuration fields.

A safer approach for individual fields is:

    REG &= ~(MASK << POSITION);
    REG |=  (VALUE << POSITION);

This is called a read-modify-write operation.

The general process is:

    Read register
         ↓
    Clear required field
         ↓
    Insert new value
         ↓
    Write register

This technique is used throughout register-level driver development.

---

# 46. Driver Testing Strategy

A driver should not be considered complete merely because the code compiles.

Testing should occur at multiple levels.

## Level 1 — Compilation

Check:

    Syntax
       ↓
    Types
       ↓
    Declarations
       ↓
    Function prototypes
       ↓
    Linking

## Level 2 — Register Verification

Check actual register values using the debugger.

For GPIO:

    GPIOA->MODER
    GPIOA->OTYPER
    GPIOA->OSPEEDR
    GPIOA->PUPDR
    GPIOA->IDR
    GPIOA->ODR

For RCC:

    RCC->CR
    RCC->CFGR
    RCC->PLLCFGR
    RCC peripheral clock registers

## Level 3 — Functional Hardware Test

Example:

    GPIO Output
         |
         v
        LED

or:

    GPIO Input
         |
         v
      Push Button

## Level 4 — Physical Signal Verification

For communication peripherals:

    I2C
      |
      +---- SCL
      +---- SDA

    SPI
      |
      +---- SCK
      +---- MOSI
      +---- MISO
      +---- NSS

    USART
      |
      +---- TX
      +---- RX

These signals can be inspected using suitable laboratory equipment.

---

# 47. Register-Level Debugging

A useful debugging method is to compare:

    Expected Register Value
             |
             v
          Debugger
             |
             v
    Actual Register Value
             |
             v
          Compare
             |
             v
    Find Configuration Error

For example, if GPIO output does not work:

    LED not turning ON
           |
           v
    Was GPIO clock enabled?
           |
           v
    Is correct GPIO port selected?
           |
           v
    Is correct pin selected?
           |
           v
    Is pin configured as output?
           |
           v
    Is output type correct?
           |
           v
    Is pull configuration correct?
           |
           v
    Is ODR / BSRR correct?
           |
           v
    Is physical pin changing?
           |
           v
    Is external circuit correct?

This creates a systematic debugging process instead of randomly changing code.

---

# 48. Driver Debugging Philosophy

When something does not work, debugging should move from software toward hardware.

    Application
         ↓
    API
         ↓
    Driver Function
         ↓
    Register Value
         ↓
    Peripheral State
         ↓
    Physical Signal
         ↓
    External Hardware

Each layer should be verified before moving to the next.

This helps isolate whether the problem is:

    Application logic
    API usage
    Driver implementation
    Register configuration
    Clock configuration
    Peripheral configuration
    GPIO configuration
    Physical hardware

---

# 49. Driver Reusability

A good driver should not be written specifically for one application.

For example, the GPIO driver should be usable for:

    LED
    Push Button
    Chip Select
    Control Signal
    Interrupt Input
    Alternate Function

The same driver API can then be reused across different projects.

The application changes, but the driver interface can remain the same.

This is one of the major advantages of driver-based firmware architecture.

---

# 50. Hardware Abstraction

The driver provides a level of abstraction between the application and hardware.

Without abstraction:

    Application
        |
        +---- RCC registers
        +---- GPIO registers
        +---- I2C registers
        +---- SPI registers
        +---- USART registers

With abstraction:

    Application
        |
        +---- GPIO API
        +---- I2C API
        +---- SPI API
        +---- USART API
                 |
                 v
             Drivers
                 |
                 v
              Registers

The application becomes easier to understand because it works with meaningful operations rather than raw register manipulation.

---

# 51. Abstraction Does Not Mean Hiding Everything

The purpose of the driver is not to completely hide the hardware from the firmware developer.

During learning and low-level development, understanding the hardware remains important.

The correct relationship is:

    Application
         |
         | Uses clean API
         v
    Driver
         |
         | Understands hardware
         v
    Registers
         |
         v
    Hardware

The application is abstracted from register details, but the driver developer must understand those details.

This is especially important for debugging.

---

# 52. GPIO Driver Learning Progression

GPIO is the first major driver because it introduces the fundamental concepts.

The progression is:

    GPIO Register Map
          ↓
    GPIO Register Structure
          ↓
    GPIO Peripheral Pointer
          ↓
    RCC Clock Control
          ↓
    GPIO Configuration Structure
          ↓
    GPIO Handle
          ↓
    GPIO_Init()
          ↓
    GPIO Read APIs
          ↓
    GPIO Write APIs
          ↓
    GPIO Toggle API
          ↓
    GPIO Interrupt APIs
          ↓
    Hardware Testing

Once these concepts are understood, the same architecture can be applied to more complex peripherals.

---

# 53. I2C Learning Progression

The I2C driver will extend the architecture with protocol-level concepts.

    I2C Register Map
          ↓
    GPIO Alternate Function
          ↓
    I2C Clock Control
          ↓
    Timing Configuration
          ↓
    I2C Handle
          ↓
    START Condition
          ↓
    Address Transmission
          ↓
    ACK / NACK
          ↓
    Data Transmission
          ↓
    Data Reception
          ↓
    STOP Condition
          ↓
    Error Handling
          ↓
    Hardware Testing

---

# 54. SPI Learning Progression

The SPI driver will introduce:

    SPI Register Map
          ↓
    GPIO Alternate Function
          ↓
    SPI Clock Control
          ↓
    Master / Slave Configuration
          ↓
    Clock Polarity
          ↓
    Clock Phase
          ↓
    Baud Rate
          ↓
    Data Frame
          ↓
    NSS Handling
          ↓
    TX / RX
          ↓
    Hardware Testing

---

# 55. USART Learning Progression

The USART driver will introduce:

    USART Register Map
          ↓
    GPIO Alternate Function
          ↓
    USART Clock Control
          ↓
    Baud Rate
          ↓
    Word Length
          ↓
    Stop Bits
          ↓
    Parity
          ↓
    TX
          ↓
    RX
          ↓
    Interrupts
          ↓
    Hardware Testing

---

# 56. Interrupt-Driven Drivers

The initial driver implementation can use polling.

Conceptually:

    CPU
     |
     v
    Check Flag
     |
     +---- Not Ready ----+
     |                   |
     |                   |
     +<------------------+
     |
     v
    Ready
     |
     v
    Process Data

Later, the driver can be extended to use interrupts:

    Peripheral Event
          |
          v
       Interrupt
          |
          v
         NVIC
          |
          v
         CPU
          |
          v
      IRQ Handler
          |
          v
    Driver Interrupt Handler
          |
          v
       Application

This improves CPU efficiency and allows asynchronous event handling.

---

# 57. Future Driver Improvements

After the basic register-level drivers are working, they can be extended with:

- Interrupt-driven communication
- Non-blocking APIs
- DMA support
- Error handling
- Timeout mechanisms
- Callback functions
- State machines
- Transaction management
- Buffer management
- Concurrency handling

The development progression is therefore:

    Basic Register-Level Driver
              ↓
    Functional Driver
              ↓
    Interrupt-Driven Driver
              ↓
    Non-Blocking Driver
              ↓
    DMA-Based Driver
              ↓
    More Robust Embedded Driver

---

# 58. Current Learning Stage

The current driver-development work is progressing through the following stages:

    [✓] MCU memory map
    [✓] Peripheral memory mapping
    [✓] Peripheral base addresses
    [✓] Register offsets
    [✓] Register structures
    [✓] Peripheral pointers
    [✓] volatile
    [✓] Bit manipulation
    [✓] RCC structure
    [✓] GPIO register structure
    [✓] GPIO peripheral definitions
    [✓] GPIO clock enable macros
    [✓] GPIO clock disable macros
    [✓] I2C clock enable concepts
    [✓] SPI clock-control concepts
    [✓] USART clock-control concepts
    [✓] SYSCFG clock-control concepts
    [✓] Configuration structures
    [✓] Handle structures
    [✓] Driver header files
    [✓] Driver source files
    [✓] API prototypes
    [✓] Understanding API parameters
    [→] Register-level API implementation
    [ ] GPIO driver testing
    [ ] GPIO interrupt implementation
    [ ] I2C driver implementation
    [ ] SPI driver implementation
    [ ] USART driver implementation
    [ ] Interrupt-driven communication
    [ ] DMA-based communication

The current focus is therefore:

    Driver Architecture
          ↓
    API Design
          ↓
    API Prototypes
          ↓
    Register-Level Implementation
          ↓
    Hardware Testing

---

# 59. Planned Repository Structure

The driver-development section can eventually be organized as:

    09_Embedded_Driver_Development/
    |
    +---- README.md
    |
    +---- 01_MCU_Specific_Header/
    |       |
    |       +---- README.md
    |       +---- stm32f446x.h
    |
    +---- 02_GPIO_Driver/
    |       |
    |       +---- README.md
    |       |
    |       +---- Inc/
    |       |       |
    |       |       +---- stm32f446x_gpio_driver.h
    |       |
    |       +---- Src/
    |               |
    |               +---- stm32f446x_gpio_driver.c
    |
    +---- 03_I2C_Driver/
    |       |
    |       +---- README.md
    |       +---- Inc/
    |       +---- Src/
    |
    +---- 04_SPI_Driver/
    |       |
    |       +---- README.md
    |       +---- Inc/
    |       +---- Src/
    |
    +---- 05_USART_Driver/
    |       |
    |       +---- README.md
    |       +---- Inc/
    |       +---- Src/
    |
    +---- 06_Test_Applications/
            |
            +---- GPIO/
            +---- I2C/
            +---- SPI/
            +---- USART/

The main `README.md` explains the complete driver architecture and learning process.

Individual peripheral folders can later contain their own detailed documentation and implementation.

---

# 60. Relationship Between Application, Driver and Hardware

The complete relationship can be remembered as:

    APPLICATION
         |
         | "I want to perform an operation."
         v
       API
         |
         | "I will translate that operation."
         v
      DRIVER
         |
         | "I know which registers control it."
         v
     REGISTERS
         |
         | "I control the hardware."
         v
      PERIPHERAL
         |
         v
      PHYSICAL
      HARDWARE

For example:

    Application
         |
         v
    GPIO_WriteToOutputPin()
         |
         v
    GPIO Driver
         |
         v
    GPIOA->BSRR
         |
         v
    GPIO Hardware
         |
         v
    PA5
         |
         v
       LED

This is the central idea of the entire driver-development section.

---

# 61. Most Important Mental Model

The entire driver-development concept can be remembered as:

    Hardware
       ↓
    Register Map
       ↓
    Register Structure
       ↓
    Peripheral Pointer
       ↓
    Configuration Structure
       ↓
    Handle
       ↓
    Driver Function
       ↓
    API
       ↓
    Application

For GPIO:

    GPIO Hardware
          ↓
    GPIO Registers
          ↓
    GPIO_RegDef_t
          ↓
    GPIOA pointer
          ↓
    GPIO_PinConfig_t
          ↓
    GPIO_Handle_t
          ↓
    GPIO_Init()
          ↓
    Application

This sequence should become the mental model for every peripheral driver developed later.

---

# 62. Driver Development vs Application Development

Application code answers:

    What should the system do?

Driver code answers:

    How do I control the hardware to make it happen?

For example:

    Application requirement:
    Turn ON LED

The application calls:

    GPIO_WriteToOutputPin()

The driver performs:

    Identify GPIO register
          ↓
    Identify pin bit
          ↓
    Modify register
          ↓
    Write register
          ↓
    GPIO hardware changes state

Therefore:

    Application = System behaviour

    Driver = Hardware control

This separation is fundamental to embedded software architecture.

---

# 63. Why Register-Level Understanding Matters

High-level libraries can make peripherals easy to configure.

However, if something goes wrong, the firmware developer eventually needs to understand:

    Which register?
    Which bit?
    Which clock?
    Which bus?
    Which peripheral?
    Which GPIO?
    Which alternate function?
    Which interrupt?
    Which status flag?

Therefore, register-level knowledge is valuable even when using higher-level libraries.

The driver-development section is intended to build this understanding from the hardware upward.

---

# 64. Complete Driver Development Workflow

The complete workflow for this project is:

    Hardware Documentation
             |
             v
       Understand MCU
             |
             v
       Understand Bus
             |
             v
      Understand Peripheral
             |
             v
        Study Registers
             |
             v
      Calculate Addresses
             |
             v
      Create C Structures
             |
             v
       Create Handles
             |
             v
       Design APIs
             |
             v
    Implement Register Access
             |
             v
       Build Firmware
             |
             v
        Program MCU
             |
             v
      Debug Registers
             |
             v
       Test Hardware
             |
             v
      Verify Behaviour
             |
             v
        Document Driver

This is the complete process from hardware documentation to working firmware.

---

# 65. Final Learning Objective

The final objective of this section is not simply:

    "I can write a GPIO driver."

The real objective is:

    "I understand how software controls MCU hardware."

That means being able to move through the following chain:

    Requirement
        ↓
    Hardware Block
        ↓
    Peripheral
        ↓
    Bus
        ↓
    Base Address
        ↓
    Register
        ↓
    Bit Field
        ↓
    Embedded C
        ↓
    Driver Function
        ↓
    API
        ↓
    Application
        ↓
    Physical Hardware

Once this thought process becomes natural, developing drivers for new STM32 peripherals becomes a structured engineering process rather than memorizing code.

---

# 66. Key Takeaways

The most important concepts from this section are:

- An embedded driver is the software layer between application code and hardware.
- STM32 peripherals are controlled through memory-mapped registers.
- Peripheral registers are accessed using addresses.
- Register addresses are determined from peripheral base addresses and register offsets.
- C structures can represent peripheral register layouts.
- Peripheral structures can be mapped to hardware addresses using pointers.
- `volatile` is important when accessing hardware-controlled registers.
- Bitwise operations are fundamental to register-level programming.
- Read-modify-write operations allow individual register fields to be modified safely.
- RCC controls peripheral clocks.
- Peripheral clocks must generally be enabled before peripheral configuration.
- Configuration structures organize peripheral settings.
- Handle structures associate a peripheral instance with its configuration.
- A driver API defines what operations are available to the application.
- The driver implementation defines how those operations are performed.
- Header files define the driver interface.
- Source files contain the implementation.
- GPIO provides the foundation for understanding peripheral driver architecture.
- Alternate functions connect GPIO pins to internal peripheral functions.
- GPIO interrupts involve GPIO, EXTI, SYSCFG, NVIC and the Cortex-M4 interrupt system.
- I2C introduces bus protocol and transaction management.
- SPI introduces synchronous serial communication and clock-phase/polarity configuration.
- USART introduces asynchronous serial communication and baud-rate configuration.
- Interrupt-driven and DMA-based drivers are later extensions of the same architecture.
- Register debugging and hardware verification are essential parts of driver development.
- The reference manual and datasheet are the primary sources for register and peripheral configuration.
- Driver development connects Embedded C knowledge directly with MCU hardware.

---

# 67. Engineering Mindset

The most important habit developed in this section is to avoid asking only:

    "What code should I write?"

Instead, ask:

    What hardware block am I controlling?

    Which peripheral controls it?

    Which bus is the peripheral connected to?

    What is the peripheral base address?

    Which register controls the required operation?

    What is the register offset?

    Which bit field must be modified?

    Does the peripheral clock need to be enabled?

    What GPIO configuration is required?

    Is an alternate function required?

    Is an interrupt involved?

    How can I verify the register configuration?

    How can I verify the physical hardware?

This produces a systematic hardware-to-software debugging approach.

---

# 68. Final Concept

The complete concept of this section can be summarized as:

    HARDWARE
        ↓
    Understand Hardware
        ↓
    Study Datasheet / Reference Manual
        ↓
    Understand Memory Map
        ↓
    Understand Bus Architecture
        ↓
    Identify Peripheral
        ↓
    Identify Registers
        ↓
    Identify Bit Fields
        ↓
    Represent Hardware in Embedded C
        ↓
    Create Driver
        ↓
    Create API
        ↓
    Use Driver from Application
        ↓
    Debug Registers
        ↓
    Verify Peripheral
        ↓
    Verify Physical Hardware

The purpose of this section is to build the foundation required for developing **register-level STM32 firmware and reusable embedded peripheral drivers using Embedded C**.

The final goal is to move from:

    "I know how to use a peripheral library"

to:

    "I understand what happens inside the MCU when my firmware controls a peripheral."

This understanding forms the foundation for advanced embedded firmware development, including:

    Interrupts
        ↓
    DMA
        ↓
    RTOS
        ↓
    Communication stacks
        ↓
    Device drivers
        ↓
    Board support packages
        ↓
    Complex embedded systems
