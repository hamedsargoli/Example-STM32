/*****************************************************************
- Project Example 1: 
						test basic timer with flag(SR)
						Freg = 1Mhz & Time updtae = 1msec
- Date: 2023-09-17
- Directed by: Hamed Sargoli

*****************************************************************/
//Header file
#include "stm32f407xx.h"

//function's
void config_port(void);
void set_pin(void);
void reset_pin(void);
void clockON_PORT(void);

//main function
int main(void)
{
	//Variable
	int timercounter = 0;
	//GPIO Config(LED)
	clockON_PORT();
	config_port();
	//Timer Config
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;					//clock Timer6 ON
	TIM6->ARR = 999;  													//Auto Reload Register = wanted number - 1  ||  1ms = 1us * (999+1)
	TIM6->PSC = 15;															//Prescaler register = wanted number - 1    ||  16MHz/(15+1)=1MHz
	TIM6->CR1 |= TIM_CR1_CEN;										//Enable Timer
	 	
	while(1)
	{
		if((TIM6->SR & TIM_SR_UIF) == TIM_SR_UIF)	//flag Check
		{
			set_pin();															//on LED
			TIM6->SR &= ~TIM_SR_UIF;								//Reset Flag update
			timercounter++;													//counter added one
		}
		
		if(timercounter >= 2000)										
			timercounter=0;
		else if(timercounter >= 1000)
			reset_pin();														//OFF LED
	}
	
}

void config_port(void){
	GPIOB->MODER |= GPIO_MODER_MODE9_0;			//output pin9 from port B
}
void set_pin(void){
	GPIOB->ODR |= GPIO_ODR_OD9;							//set_pin pin9 = high
}
void reset_pin(void){
	GPIOB->ODR &= ~GPIO_ODR_OD9;						//reset pin9 = low
}
void clockON_PORT(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;		//clock GPIOB ON
}