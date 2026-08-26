#ifndef INC_STM32F446X_H_
#define INC_STM32F446X_H_
#include<stdint.h>


// base address of memories with macros

#define FLASH_BASEADDR          0x0800 0000U
#define SRAM1_BASEADDR          0x2000 0000U
#define SRAM2_BASEADDR          0x2001 C000U
#define ROM_BASEADDR            0x1FFF 0000U
#define SRAM_BASEADDR           SRAM1_BASEADDR

//base address of APBx and AHBx Peripherals

#define PERIPH_BASEADDR         0x4000 0000U
#define APB1PH_BASEADDR         PERIPH_BASEADDR
#define APB2PH_BASEADDR         0x4001 0000U
#define AHB1PH_BASEADDR         0x4002 0000U
#define AHB2PH_BASEADDR         0x5000 0000U

// base address of peripheral which are hanging on
// AHB1 bus

#define GPIOA_BASEADDR          (AHB1PH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR          (AHB1PH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR          (AHB1PH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR          (AHB1PH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR          (AHB1PH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR          (AHB1PH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR          (AHB1PH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR          (AHB1PH_BASEADDR + 0x1C00)
#define RCC_BASEADDR            (AHB1PH_BASEADDR + 0x3800)

// base address of peripheral hanging on APB1 bus

#define I2C1_BASEADDR          (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR          (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASEADDR          (APB1PERIPH_BASE + 0x5C00)

#define SPI2_BASEADDR          (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR          (APB1PERIPH_BASE + 0x3C00)

#define USART2_BASEADDR        (APB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDR        (APB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDR         (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR         (APB1PERIPH_BASE + 0x5000)

// base address pf peripheral hanging on APB2 bus

#define EXTI_BASEADDR    (APB2PERIPH_BASE + 0x3C00)
#define SPI1_BASEADDR    (APB2PERIPH_BASE + 0x3000)
#define SYSCFG_BASEADDR  (APB2PERIPH_BASE + 0x3800)
#define USART1_BASEADDR  (APB2PERIPH_BASE + 0x1000)
#define USART6_BASEADDR  (APB2PERIPH_BASE + 0x1400)

// Structuring and defining the peripheral GPIO registers

typedef struct {

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

}GPIO_RegDef_t;

//GPIO_RegDef_t *pGPIOA = ((GPIO_RegDef_t *) GPIOA_BASEADDR);
//GPIO peripheral typecated into GPIOx_RegDef_t type structure

#define GPIOA    ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB    ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC    ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD    ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE    ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF    ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG    ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH    ((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define RCC      ((RCC_RegDef_t*)RCC_BASEADDR)

/*
 * Peripheral register definition structure for RCC
 */

typedef struct
{
    volatile uint32_t CR;          /* Address offset: 0x00 */
    volatile uint32_t PLLCFGR;     /* Address offset: 0x04 */
    volatile uint32_t CFGR;        /* Address offset: 0x08 */
    volatile uint32_t CIR;         /* Address offset: 0x0C */
    volatile uint32_t AHB1RSTR;    /* Address offset: 0x10 */
    volatile uint32_t AHB2RSTR;    /* Address offset: 0x14 */
    volatile uint32_t AHB3RSTR;    /* Address offset: 0x18 */

    uint32_t RESERVED0;            /* Reserved, 0x1C */

    volatile uint32_t APB1RSTR;    /* Address offset: 0x20 */
    volatile uint32_t APB2RSTR;    /* Address offset: 0x24 */

    uint32_t RESERVED1[2];         /* Reserved, 0x28 - 0x2C */

    volatile uint32_t AHB1ENR;     /* Address offset: 0x30 */
    volatile uint32_t AHB2ENR;     /* Address offset: 0x34 */
    volatile uint32_t AHB3ENR;     /* Address offset: 0x38 */

    uint32_t RESERVED2;            /* Reserved, 0x3C */

    volatile uint32_t APB1ENR;     /* Address offset: 0x40 */
    volatile uint32_t APB2ENR;     /* Address offset: 0x44 */

    uint32_t RESERVED3[2];         /* Reserved, 0x48 - 0x4C */

    volatile uint32_t AHB1LPENR;   /* Address offset: 0x50 */
    volatile uint32_t AHB2LPENR;   /* Address offset: 0x54 */
    volatile uint32_t AHB3LPENR;   /* Address offset: 0x58 */

    uint32_t RESERVED4;            /* Reserved, 0x5C */

    volatile uint32_t APB1LPENR;   /* Address offset: 0x60 */
    volatile uint32_t APB2LPENR;   /* Address offset: 0x64 */

    uint32_t RESERVED5[2];         /* Reserved, 0x68 - 0x6C */

    volatile uint32_t BDCR;        /* Address offset: 0x70 */
    volatile uint32_t CSR;         /* Address offset: 0x74 */

    uint32_t RESERVED6[2];         /* Reserved, 0x78 - 0x7C */

    volatile uint32_t SSCGR;       /* Address offset: 0x80 */
    volatile uint32_t PLLI2SCFGR;  /* Address offset: 0x84 */
    volatile uint32_t PLLSAICFGR;  /* Address offset: 0x88 */
    volatile uint32_t DCKCFGR;     /* Address offset: 0x8C */
    volatile uint32_t CKGATENR;    /* Address offset: 0x90 */
    volatile uint32_t DCKCFGR2;    /* Address offset: 0x94 */

} RCC_RegDef_t;
/*
 * Clock Enable Macros for GPIOx peripherals
 */

#define GPIOA_PCLK_EN()      (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()      (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()      (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()      (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()      (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()      (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()      (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()      (RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()      (RCC->AHB1ENR |= (1 << 8))
/*
 * Clock Disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_DI()      (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()      (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()      (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()      (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()      (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()      (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()      (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()      (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()      (RCC->AHB1ENR &= ~(1 << 8))
/*
 * Clock Enable Macros for I2Cx peripherals
 */

#define I2C1_PCLK_EN()       (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()       (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()       (RCC->APB1ENR |= (1 << 23))
/*
 * Clock Disable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_DI()       (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()       (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()       (RCC->APB1ENR &= ~(1 << 23))
/*
 * Clock Enable Macros for SPIx peripherals
 */
#define SPI1_PCLK_EN()      (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()      (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()      (RCC->APB1ENR |= (1 << 15))
/*
 * Clock Disable Macros for SPIx peripherals
 */
#define SPI1_PCLK_DI()      (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()      (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()      (RCC->APB1ENR &= ~(1 << 15))

/*
 * Clock Enable Macros for USARTx / UARTx peripherals
 */
#define USART1_PCLK_EN()     (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()     (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()     (RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()      (RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()      (RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN()     (RCC->APB2ENR |= (1 << 5))
/*
 * Clock Disable Macros for USARTx / UARTx peripherals
 */
#define USART1_PCLK_DI()     (RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()     (RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()     (RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()      (RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()      (RCC->APB1ENR &= ~(1 << 20))
#define USART6_PCLK_DI()     (RCC->APB2ENR &= ~(1 << 5))
/*
 * Clock Enable Macro for SYSCFG peripheral
 */

#define SYSCFG_PCLK_EN()     (RCC->APB2ENR |= (1 << 14))
/*
 * Clock Disable Macro for SYSCFG peripheral
 */
#define SYSCFG_PCLK_DI()     (RCC->APB2ENR &= ~(1 << 14))

// Some generic macros
#define ENABLE           1
#define DISABLE          0
#define SET              ENABLE
#define RESET            DISABLE


#endif /* INC_STM32F446X_H_ */
