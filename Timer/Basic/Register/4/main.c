/*****************************************************************
- Project Example 4: 
						test basic timer with flag(SR)
						Freg(CK_CNT) = 250 Hz  
					 	Time updtae = 4 msec 
					* UG bit = 1 | Software Update (Line53)(SEE Simulator)
- Date: 2023-09-18
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
	
	TIM6->ARR = 1999;  													//Auto Reload Register = wanted number - 1  ||  8s = 4ms * (1999+1)
	TIM6->PSC = 63999;													//Prescaler register = wanted number - 1    ||  250Hz = 16MHz/(63999+1)
	
	TIM6->CR1 |= TIM_CR1_CEN;	  								//Enable Timer
	 	
	while(1)
	{
		if((TIM6->SR & TIM_SR_UIF) == TIM_SR_UIF)	//flag Check
		{
			set_pin();															//on LED
			TIM6->SR &= ~TIM_SR_UIF;								//Reset Flag update
			timercounter++;													//counter added one
		}
		
		if(timercounter > 2)										
			timercounter=0;
		else if(timercounter > 1)
			reset_pin();														//OFF LED
		//*******************************************************************************
		if(TIM6->CNT == 999)
			TIM6->EGR |= TIM_EGR_UG;
		//*******************************************************************************
	}

}

void config_port(void){
	GPIOB->MODER |= GPIO_MODER_MODE9_0;					//output pin9 from port B
}
void set_pin(void){
	GPIOB->ODR |= GPIO_ODR_OD9;									//set_pin pin9 = high
}
void reset_pin(void){
	GPIOB->ODR &= ~GPIO_ODR_OD9;								//reset pin9 = low
}
void clockON_PORT(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;				//clock GPIOB ON
}
