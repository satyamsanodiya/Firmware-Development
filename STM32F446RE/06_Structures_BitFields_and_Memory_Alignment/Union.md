---

# Union

## What is a Union?

A **union** is a user-defined data type similar to a structure, but unlike a structure, **all members share the same memory location**.

Only one member can hold a meaningful value at a time because writing to one member overwrites the data of the others.

The size of a union is always equal to the size of its **largest member**.

This makes unions extremely useful in embedded systems where memory is limited.

---

# Structure vs Union

| Structure | Union |
|-----------|--------|
| Every member has its own memory location | All members share the same memory location |
| Size is the sum of all members (plus padding) | Size equals the largest member |
| Multiple members can store valid data simultaneously | Only one member should be used at a time |
| Used to represent grouped information | Used to represent the same memory in different formats |

---

# Why Use Union in Embedded Systems?

Unions are commonly used in firmware for:

- Memory optimization
- Packet decoding
- Hardware register access
- Protocol implementation
- Byte-level manipulation
- Viewing the same data in multiple formats

Since all members occupy the same memory, no additional memory is consumed.

---

# Packet Extraction Using Union and Bit-Fields

Instead of manually shifting and masking every field, I implemented a **union** containing a **32-bit packet** and a **bit-field structure**.

```c
union Packet
{
    uint32_t packetValue;

    struct
    {
        uint32_t crc       :2;
        uint32_t status    :1;
        uint32_t payload   :12;
        uint32_t bat       :3;
        uint32_t sensor    :3;
        uint32_t longAddr  :8;
        uint32_t shortAddr :2;
        uint32_t addrMode  :1;
    } packetFields;
};
```

Here,

- `packetValue` stores the complete 32-bit communication packet.
- `packetFields` provides direct access to every individual field.

Both members occupy the **same 4-byte memory location**.

---

# Reading Packet Fields

The complete packet is received as a 32-bit hexadecimal value.

```c
union Packet packet;

scanf("%X",&packet.packetValue);
```

Since both members share the same memory, every field becomes immediately available.

```c
printf("crc      : %X\n", packet.packetFields.crc);
printf("status   : %X\n", packet.packetFields.status);
printf("payload  : %X\n", packet.packetFields.payload);
printf("bat      : %X\n", packet.packetFields.bat);
printf("sensor   : %X\n", packet.packetFields.sensor);
printf("longAddr : %X\n", packet.packetFields.longAddr);
printf("shortAddr: %X\n", packet.packetFields.shortAddr);
printf("addrMode : %X\n", packet.packetFields.addrMode);
```

No manual bit masking or shifting is required because the compiler automatically maps every bit-field to the correct position.

---

# Memory Efficiency

Since both members overlap in memory,

```c
sizeof(packet)
```

returns

```
4 Bytes
```

Even though the union contains

- one 32-bit integer
- one complete bit-field structure

only **4 bytes** are required because both members share the same storage.

This is significantly more memory-efficient than storing separate variables.

---

# Another Union Example

I also implemented a simple union to understand how different data types share memory.

```c
union Address
{
    uint16_t shortAddr;
    uint32_t longAddr;
};
```

Example:

```c
union Address addr;

addr.shortAddr = 0xABCD;
addr.longAddr  = 0xEEEECCCC;

printf("short addr = %X\n", addr.shortAddr);
printf("long addr  = %X\n", addr.longAddr);
```

Since both members occupy the same memory,

writing to `longAddr` overwrites the value previously stored in `shortAddr`.

This demonstrates that union members do **not** have independent storage.

---

# Advantages of Union

- Saves memory by sharing storage between members.
- Eliminates unnecessary data duplication.
- Provides multiple views of the same memory.
- Simplifies packet decoding.
- Frequently used with bit-fields for protocol implementation.
- Commonly used in embedded firmware and device drivers.

---

# Practical Learning Outcome

During this exercise, I learned how **unions** provide multiple representations of the same memory location without increasing memory consumption.

I combined **unions** with **bit-fields** to decode a 32-bit communication packet directly into individual fields such as CRC, Status, Payload, Battery, Sensor, and Address information. This approach eliminated the need for repeated masking and bit-shifting operations while keeping the packet size fixed at **4 bytes**.

I also verified that all union members share the same memory location by creating a simple address example, where writing to one member immediately affected the other. This helped me understand why unions are widely used in embedded firmware for communication protocols, hardware register mapping, and memory-constrained applications.

---

# Interview Questions

### What is the difference between a structure and a union?

A structure allocates separate memory for every member, while a union shares the same memory among all its members. Therefore, the size of a structure is generally the sum of its members (plus padding), whereas the size of a union equals the size of its largest member.

---

### Why are unions used in embedded systems?

Unions are used to save memory and to represent the same data in multiple formats, such as viewing a communication packet as both a 32-bit integer and individual bit-fields.

---

### Why is a union combined with bit-fields?

Combining a union with bit-fields allows an entire packet to be received as a single integer while simultaneously providing direct access to each field without performing manual masking and shifting operations.

---

### What happens if one member of a union is modified?

Since all members share the same memory location, writing to one member overwrites the data represented by the other members.
