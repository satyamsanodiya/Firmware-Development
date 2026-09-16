#include "stm32f446x_gpio_driver.h"

// API's supported by this driver implementation

/*********************************************************************
 * @fn      - GPIO_PeriClockControl
 *
 * @brief   - Enables or disables the peripheral clock for the given GPIO port
 *
 * @param[in]- pGPIOx: Base address of the GPIO peripheral
 * @param[in]- EnorDi: ENABLE or DISABLE macro
 *
 * @return  - none
 *
 * @Note    - none
 */

// Peripheral clk setup
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi) {
	    if(EnorDi == ENABLE)
	    {
	        if(pGPIOx == GPIOA)
	        {
	            GPIOA_PCLK_EN();
	        }
	        else if(pGPIOx == GPIOB)
	        {
	            GPIOB_PCLK_EN();
	        }
	        else if(pGPIOx == GPIOC)
	        {
	            GPIOC_PCLK_EN();
	        }
	        else if(pGPIOx == GPIOD)
	        {
	            GPIOD_PCLK_EN();
	        }
	        else if(pGPIOx == GPIOE)
	        {
	            GPIOE_PCLK_EN();
	        }
	        else if(pGPIOx == GPIOF)
	        {
	            GPIOF_PCLK_EN();
	        }
	        else if(pGPIOx == GPIOG)
	        {
	            GPIOG_PCLK_EN();
	        }
	        else if(pGPIOx == GPIOH)
	        {
	            GPIOH_PCLK_EN();
	        }
	    }
	    else
	    {
	        // Disable GPIO peripheral clock

	        if(pGPIOx == GPIOA)
	        {
	            GPIOA_PCLK_DI();
	        }
	        else if(pGPIOx == GPIOB)
	        {
	            GPIOB_PCLK_DI();
	        }
	        else if(pGPIOx == GPIOC)
	        {
	            GPIOC_PCLK_DI();
	        }
	        else if(pGPIOx == GPIOD)
	        {
	            GPIOD_PCLK_DI();
	        }
	        else if(pGPIOx == GPIOE)
	        {
	            GPIOE_PCLK_DI();
	        }
	        else if(pGPIOx == GPIOF)
	        {
	            GPIOF_PCLK_DI();
	        }
	        else if(pGPIOx == GPIOG)
	        {
	            GPIOG_PCLK_DI();
	        }
	        else if(pGPIOx == GPIOH)
	        {
	            GPIOH_PCLK_DI();
	        }
	    }

}
//Init and De-init
/*********************************************************************
 * @fn      - GPIO_Init
 *
 * @brief   - Initializes the GPIO port and pin according to the specified
 *            parameters in the GPIO handle structure.
 *
 * @param[in]- pGPIOHandle: Pointer to the GPIO_Handle_t structure that
 *             contains the configuration information for the given GPIO pin.
 *
 * @return  - none
 *
 * @Note    - none
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;

	//1. configure the mode of gpio pin
      if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
    	  // NON interrupt mode
    	  temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    	  pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) );// clear
    	  pGPIOHandle->pGPIOx->MODER |= temp;

      }else{
    	  // interrupt mode
      }
      temp=0;
	//2. configure the speed
      temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
      pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));//clear
      pGPIOHandle->pGPIOx->OSPEEDR |= temp;

      temp = 0;
	//3. config the pupd setting
      temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
       pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));//clear
       pGPIOHandle->pGPIOx->PUPDR |= temp;

       temp = 0;
	//4. config the output type
       temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinOPTType << (1* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
       pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << (1* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));//clear
       pGPIOHandle->pGPIOx->OTYPER |= temp;

       temp = 0;
	//5. config Alternate functionality
       if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
       {
    	   // Configure the alt fuction reg
    	   // first check which alt fn reg haev to use if 0 ALF_L andif 1 ALF_H
    	   uint32_t temp1, temp2;
    	   temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
    	   // To find the bit postion for pin which required 4 bit to slecet ALF
    	   temp2 =  pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
    	   pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF <<(4*temp2));// clear bitfield before modify
           pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4*temp2));

       }

}

// to releasr the peri
/*********************************************************************
 * @fn      - GPIO_DeInit
 *
 * @brief   - De-initializes the given GPIO port, forcing its registers
 *            back to their default reset values.
 *
 * @param[in]- pGPIOx: Base address of the GPIO peripheral
 *
 * @return  - none
 *
 * @Note    - none
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	           if(pGPIOx == GPIOA)
		        {
		            GPIOA_REG_RESET();
		        }
		        else if(pGPIOx == GPIOB)
		        {
		            GPIOB_REG_RESET();
		        }
		        else if(pGPIOx == GPIOC)
		        {
		        	GPIOC_REG_RESET();
		        }
		        else if(pGPIOx == GPIOD)
		        {
		        	GPIOD_REG_RESET();
		        }
		        else if(pGPIOx == GPIOE)
		        {
		        	GPIOE_REG_RESET();
		        }
		        else if(pGPIOx == GPIOF)
		        {
		        	GPIOF_REG_RESET();
		        }
		        else if(pGPIOx == GPIOG)
		        {
		        	GPIOG_REG_RESET();
		        }
		        else if(pGPIOx == GPIOH)
		        {
		        	GPIOH_REG_RESET();
		        }
}

// Read from GPIO port and pin
/*********************************************************************
 * @fn      - GPIO_ReadFromInputPin
 *
 * @brief   - Reads the digital state of a specific GPIO pin.
 *
 * @param[in]- pGPIOx: Base address of the GPIO peripheral
 * @param[in]- PinNumber: The specific pin number to read from
 *
 * @return  - 0 or 1 (the current state of the pin)
 *
 * @Note    - none
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;

	value = (uint8_t)((pGPIOx->IDR >>PinNumber) & 0x00000001);

	return value;
}
/*********************************************************************
 * @fn      - GPIO_ReadFromInputPort
 *
 * @brief   - Reads the digital state of an entire GPIO port.
 *
 * @param[in]- pGPIOx: Base address of the GPIO peripheral
 *
 * @return  - 16-bit value representing the state of all 16 pins on the port
 *
 * @Note    - none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;

	value = (uint16_t) (pGPIOx->IDR);

	return value;
}
// Write on GPIO port and pin
/*********************************************************************
 * @fn      - GPIO_WriteToOutputPin
 *
 * @brief   - Writes a digital value (HIGH or LOW) to a specific GPIO pin.
 *
 * @param[in]- pGPIOx: Base address of the GPIO peripheral
 * @param[in]- PinNumber: The specific pin number to write to
 * @param[in]- Value: The value to write (e.g., SET or RESET)
 *
 * @return  - none
 *
 * @Note    - none
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET){
          //write 1 on correspoding Pin number in GPIO port output data reg
		pGPIOx->ODR |= (1<<PinNumber);
	}else{
		//write 0
		pGPIOx->ODR &= ~(1<<PinNumber);
	}
}
/*********************************************************************
 * @fn      - GPIO_WriteToOutputPort
 *
 * @brief   - Writes a 16-bit value to an entire GPIO port simultaneously.
 *
 * @param[in]- pGPIOx: Base address of the GPIO peripheral
 * @param[in]- Value: The 16-bit value to write to the port
 *
 * @return  - none
 *
 * @Note    - none
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;

}

// toggle the pin
/*********************************************************************
 * @fn      - GPIO_ToggleOutputPin
 *
 * @brief   - Toggles the digital state of a specific GPIO output pin
 *            (changes HIGH to LOW, or LOW to HIGH).
 *
 * @param[in]- pGPIOx: Base address of the GPIO peripheral
 * @param[in]- PinNumber: The specific pin number to toggle
 *
 * @return  - none
 *
 * @Note    - none
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1<<PinNumber);

}

/*********************************************************************
 * @fn      - GPIO_IRQConfig
 *
 * @brief   - Configures the interrupt in the NVIC for a specific IRQ
 *            number, including enabling/disabling it and setting its priority.
 *
 * @param[in]- IRQNumber: The IRQ number associated with the interrupt
 * @param[in]- IRQPriority: The priority level to assign to the interrupt
 * @param[in]- EnorDi: ENABLE or DISABLE macro
 *
 * @return  - none
 *
 * @Note    - none
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi)
{
}

/*********************************************************************
 * @fn      - GPIO_IRQHandling
 *
 * @brief   - Processes the interrupt by clearing the corresponding pending
 *            bit in the EXTI PR (Pending Register) so the ISR doesn't
 *            trigger endlessly.
 *
 * @param[in]- PinNumber: The pin number that triggered the interrupt
 *
 * @return  - none
 *
 * @Note    - none
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
}

