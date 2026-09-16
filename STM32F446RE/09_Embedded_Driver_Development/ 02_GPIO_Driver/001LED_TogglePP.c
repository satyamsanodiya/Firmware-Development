#include "stm32f446x.h"
#include "stm32f446x_gpio_driver.h"

void delay(void){
	// software delay
	for(uint32_t i=0; i < 500000; i++);

}
int main(void){

  GPIO_Handle_t GpioLed;
  GpioLed.pGPIOx = GPIOA; // Select GPIO A for config
  GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
  GpioLed.GPIO_PinConfig.GPIO_PinMode   = GPIO_MODE_OUT;
  GpioLed.GPIO_PinConfig.GPIO_PinSpeed  = GPIO_SPEED_FAST;
  GpioLed.GPIO_PinConfig.GPIO_PinOPTType     = GPIO_OP_TYPE_PP;//Push pull output type
  GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

  //Before initilization Clock must be On for that peripheral
  GPIO_PeriClockControl(GPIOA, ENABLE);
  //Now to actual implementation for the register
  GPIO_Init(&GpioLed);
  //Now toggle Pin for infinite time
  while(1){

	  GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
	  delay();

  }

}
