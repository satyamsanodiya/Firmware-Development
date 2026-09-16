#ifndef INC_STM32F446X_GPIO_DRIVER_H_
#define INC_STM32F446X_GPIO_DRIVER_H_

#include "stm32f446x.h"

// This is configuration structure for GPIO

typedef struct{

	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;    // possible modes go to @PIN_MODES to check all
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPTType;
	uint8_t GPIO_PinAltFunMode;

}GPIO_PinConfig_t;

// This is a handle structure for a GPIO pin

typedef struct {

	// pointer to struct handle the base address of the GPIO peripheral registers
	GPIO_RegDef_t *pGPIOx;

	//to hold GPIO pin configuration settings
	GPIO_PinConfig_t GPIO_PinConfig; //structure variable

	// To handle
	//GPIO_Handle_t *pGPIOHandle
}GPIO_Handle_t;

// GPIO pin possiable mode @PIN_MODES
#define GPIO_MODE_IN      0
#define GPIO_MODE_OUT     1
#define GPIO_MODE_ALTFN   2
#define GPIO_MODE_ANALOG  3
#define GPIO_MODE_IT_FT   4 //interrupt modes when pin is in input mode altenative
#define GPIO_MODE_IT_RT   5
#define GPIO_MODE_IT_RF   6

// GPIO pin output types
#define GPIO_OP_TYPE_PP   0
#define GPIO_OP_TYPE_OD   1

// GPIO pin possible output speed
#define GPIO_SPEED_LOW    0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_FAST   2
#define GPIO_SPEED_HIGH   3

// GPIO pin Pullup/pulldown config macros
#define GPIO_NO_PUPD      0
#define GPIO_PIN_PU       1
#define GPIO_PIN_PD       2

//GPIO pin number
#define GPIO_PIN_NO_0     0
#define GPIO_PIN_NO_1     1
#define GPIO_PIN_NO_2     2
#define GPIO_PIN_NO_3     3
#define GPIO_PIN_NO_4     4
#define GPIO_PIN_NO_5     5
#define GPIO_PIN_NO_6     6
#define GPIO_PIN_NO_7     7
#define GPIO_PIN_NO_8     8
#define GPIO_PIN_NO_9     9
#define GPIO_PIN_NO_10    10
#define GPIO_PIN_NO_11    11
#define GPIO_PIN_NO_12    12
#define GPIO_PIN_NO_13    13
#define GPIO_PIN_NO_14    14
#define GPIO_PIN_NO_15    15

// API's supported by this driver(fun defination for more info)

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi); // Peripheral clk setup

void GPIO_Init(GPIO_Handle_t *pGPIOHandle); //Init and De-init
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);// to releasr the peri

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);// Read from GPIO port and pin
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);// Write on GPIO port and pin
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);// toggle the pin

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);// Interrupt Handling and config
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F446X_GPIO_DRIVER_H_ */
