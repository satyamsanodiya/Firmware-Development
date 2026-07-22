# Structures, Bit-Fields and Memory Alignment

## Overview

Structures are one of the most important data types in Embedded C because they allow multiple related variables to be grouped into a single user-defined data type. They are widely used to represent hardware registers, communication packets, peripheral configurations, sensor data, and protocol frames.

In this learning exercise, I explored how structures are stored in memory, how the compiler aligns structure members, why padding is inserted, how packed structures remove padding, and how bit-fields can significantly reduce memory usage when storing data that occupies only a few bits.

All experiments were performed using STM32CubeIDE and Embedded C.

---

# Learning Objectives

During this lesson I explored the following concepts:

- Structure declaration and memory allocation
- Packet decoding using structures
- Structure pointers and the `->` operator
- Bit-field implementation
- Memory reduction using bit-fields
- Structure alignment
- Structure padding
- Packed structures using `__attribute__((packed))`
- Assembly analysis of aligned and unaligned memory accesses
- Performance impact of unaligned accesses

---

# What is a Structure?

A structure is a user-defined data type that groups multiple variables of different data types under a single name.

Example:

```c
struct Packet
{
    uint8_t  crc;
    uint8_t  status;
    uint16_t payload;
    uint8_t  bat;
    uint8_t  sensor;
    uint8_t  longAddr;
    uint8_t  shortAddr;
    uint8_t  addrMode;
};
```

Unlike arrays, structure members may have different data types.

---

# Important Observation

One important concept I learned is:

> **Declaring a structure does not allocate memory.**

Only the structure definition exists.

Memory is allocated only when an object of that structure is created.

Example:

```c
struct Packet packet;
```

At this point, memory is reserved for the object `packet`.

---

# Packet Extraction Using Structures

To understand practical usage, I implemented a packet decoder for a 32-bit communication packet.

Packet format:

```
---------------------------------------------------------
|ADDR|SHORT| LONG_ADDR |SENSOR|BAT|PAYLOAD|STATUS|CRC|
---------------------------------------------------------
```

Each field occupies a different number of bits.

The packet is first received as a single 32-bit integer.

Example:

```c
uint32_t packetValue;
scanf("%X",&packetValue);
```

Each field is then extracted using masking and bit shifting.

Example:

```c
packet.crc =
        (uint8_t)(packetValue & 0x3);

packet.status =
        (uint8_t)((packetValue >> 2) & 0x1);

packet.payload =
        (uint16_t)((packetValue >> 3) & 0xFFF);
```

This exercise demonstrated how communication protocols are decoded in embedded systems.

---

# Memory Usage Without Bit-Fields

Initially, the packet structure was implemented using normal variables.

```c
struct Packet
{
    uint8_t crc;
    uint8_t status;
    uint16_t payload;
    uint8_t bat;
    uint8_t sensor;
    uint8_t longAddr;
    uint8_t shortAddr;
    uint8_t addrMode;
};
```

Although the actual packet occupies only **32 bits (4 bytes)**, the compiler allocated additional memory because each member occupied at least one byte.

Result:

```
sizeof(struct Packet)

= 8 Bytes
```

This demonstrates that ordinary structures cannot efficiently represent data occupying only a few bits.

---

# Bit-Fields

To reduce memory consumption, I implemented the packet using bit-fields.

Example:

```c
struct Packet
{
    uint8_t crc       :2;
    uint8_t status    :1;
    uint16_t payload  :12;
    uint8_t bat       :3;
    uint8_t sensor    :3;
    uint8_t longAddr  :8;
    uint8_t shortAddr :2;
    uint8_t addrMode  :1;
};
```

Each member now occupies only the exact number of required bits.

Result:

```
sizeof(struct Packet)

= 4 Bytes
```

The packet now occupies exactly the same memory as the original 32-bit communication frame.

---

# Advantages of Bit-Fields

Using bit-fields provides several benefits:

- Significant memory reduction
- Natural representation of communication packets
- Cleaner code compared to repeated masking
- Easier access to individual packet fields
- Better readability for protocol implementations

Bit-fields are commonly used in:

- CAN Frames
- Ethernet Headers
- USB Descriptors
- Network Protocols
- Device Registers
- Embedded Communication Packets

---

# Structure Alignment

Modern processors access memory more efficiently when data is aligned to natural word boundaries.

Typical alignment:

| Data Type | Preferred Alignment |
|------------|--------------------|
| uint8_t | 1 Byte |
| uint16_t | 2 Bytes |
| uint32_t | 4 Bytes |
| uint64_t | 8 Bytes |

The compiler automatically aligns structure members to improve execution speed.

---

# Structure Padding

During alignment, the compiler may insert unused bytes between members.

Example:

```c
struct Example
{
    uint8_t  a;
    uint32_t b;
};
```

Memory Layout:

```
Address

0x00   a

0x01   Padding

0x02   Padding

0x03   Padding

0x04   b
0x05
0x06
0x07
```

Although only **5 bytes** of actual data exist,

the compiler allocates **8 bytes**.

The unused bytes are called **padding bytes**.

Padding improves processor performance because aligned memory accesses require fewer machine instructions.

---

# Packed Structures

Sometimes memory usage is more important than execution speed.

The compiler padding can be removed using:

```c
__attribute__((packed))
```

Example:

```c
struct __attribute__((packed)) Example
{
    uint8_t a;
    uint32_t b;
};
```

Memory Layout:

```
Address

0x00   a

0x01   b

0x02

0x03

0x04
```

Now no padding exists.

The structure occupies only **5 bytes**.

---

# Why Packed Structures Can Reduce Performance

Although packed structures reduce memory usage, they introduce unaligned memory accesses.

For example,

reading a 32-bit variable from address:

```
0x20000001
```

is not naturally aligned.

The processor cannot fetch the entire word in a single aligned access.

The compiler therefore generates additional load, shift, and combine instructions.

During assembly analysis, I observed that packed structures required more instructions than naturally aligned structures.

Therefore:

- Less memory
- More instructions
- Slightly slower execution

This illustrates the trade-off between memory efficiency and execution speed.

---

# Structure Pointer

Instead of accessing structure members directly,

```c
packet.payload
```

a pointer can be used.

Example:

```c
struct Packet packet;

struct Packet *ptr = &packet;
```

Members are accessed using the arrow operator.

```c
ptr->payload

ptr->sensor

ptr->crc
```

The `->` operator is simply shorthand for:

```c
(*ptr).payload
```

Structure pointers are widely used in:

- Peripheral Drivers
- Device Drivers
- RTOS Objects
- Communication Protocol Stacks
- Linked Lists
- Dynamic Memory Allocation

---

# Practical Learning Outcome

During this exercise I implemented a complete 32-bit packet decoder using Embedded C structures and bit-fields.

I compared the memory usage of ordinary structures and bit-field structures, observed the reduction from **8 bytes to 4 bytes**, and verified the results using `sizeof()`.

I also explored compiler-generated structure padding, analyzed aligned versus unaligned memory accesses, used packed structures to remove padding, and studied the resulting assembly code to understand why packed structures may reduce execution performance despite improving memory utilization.

Finally, I learned how structure pointers simplify access to structure members using the `->` operator, which is widely used in embedded firmware and driver development.

---

# Key Takeaways

- Structure definitions do not allocate memory.
- Memory is allocated only when an object is created.
- Bit-fields efficiently represent communication packets.
- Bit-fields reduced packet memory from **8 Bytes to 4 Bytes**.
- Compiler alignment improves execution efficiency.
- Padding increases memory consumption but improves performance.
- Packed structures remove padding but may decrease execution speed.
- Structure pointers simplify access to complex data structures.
- Understanding memory layout is essential for embedded firmware development.

---

# Interview Questions

### Why do we use structures in embedded systems?

Structures group related data into a single object, making firmware easier to maintain and ideal for representing hardware registers, communication packets, and peripheral configurations.

---

### What is a bit-field?

A bit-field allows a structure member to occupy only the required number of bits instead of an entire byte or word, reducing memory consumption.

---

### Why does the compiler insert padding?

Padding aligns data members to their natural boundaries, enabling faster and more efficient memory accesses by the processor.

---

### What is a packed structure?

A packed structure removes compiler-inserted padding using `__attribute__((packed))`, reducing memory usage but potentially causing slower execution due to unaligned memory accesses.

---

### When should packed structures be used?

Packed structures are useful when matching communication protocol formats, hardware-defined memory layouts, or storage formats where exact byte representation is required.
