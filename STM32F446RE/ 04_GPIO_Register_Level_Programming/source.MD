#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint32_t *pClkCtrlreg  = (uint32_t *)0x40023830;
    uint32_t *pPortAModereg = (uint32_t *)0x40020000;
    uint32_t *pPortAOutreg  = (uint32_t *)0x40020014;
    uint32_t *pPortAInreg  = (uint32_t *)0x40020010; // that is read only register

    // To set clock for GPIO A using RCC clock enable register( set 1st bit position)
    uint32_t temp = *pClkCtrlreg;
    temp = temp | (1<<0);
    *pClkCtrlreg = temp;
    // or we can just do *pClkCtrlreg |= 0x08;

    // To set the mode of GPIO PA5 as an Output
    // First clear the value of both 10th and 11th bit using bitwise AND
    *pPortAModereg &= (~(3<<10));
    *pPortAModereg &= (~(3<<0));  // here PA0 is set as input pin

    // Then set the value of 10th bit to 1 using bitwise OR
    *pPortAModereg |= (1<<10); // now PA5(user led) pin set as an output
    while(1)
    { // Now to read GPIO PA0 using input data register
    uint8_t Pin_status = (uint8_t)*pPortAInreg & (1<<0);

    // Now to write on pin 5 of PORT A high or low
    if( Pin_status){
    	//true then turn on the led (GPIO 5)
    	  *pPortAOutreg |= (1<<5);
    }
    else {
    	// false then turn of (low GPIO 5)
    	 *pPortAOutreg &= (~(1<<5));
    }
    }

}
