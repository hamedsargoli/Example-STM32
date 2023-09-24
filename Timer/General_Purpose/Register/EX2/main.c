/*****************************************************************
- Example 2: 
					* General purpose Timer - interrupt Down counter
						Freg(CK_CNT) = 250 Hz  
					 	Time updtae  = 10 sec 
					  
- Date: 2023-09-24
- Directed by: Hamed Sargoli

*****************************************************************/
//Header file
#include "stm32f407xx.h"

//Global Variable
volatile int timercounter = 0;

//function's
void config_port(void);
void set_pin(void);
void reset_pin(void);
void clockON_PORT(void);

//main function
int main(void)
{
	//GPIO Config(LED)
	clockON_PORT();
	config_port();
	
	//----------------------------------------------------------------------
	//Timer Config
	//----------------------------------------------------------------------
	//**********************************************************************
	// Initialize the interrupt (constant import from stm32f4xx.h)
	__NVIC_EnableIRQ(TIM4_IRQn);
	//**********************************************************************
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;					//clock Timer 4 ON
  
	//**********************************************************************
	TIM4->DIER |= TIM_DIER_UIE;									//Enable Intrupt
	//**********************************************************************
	
	TIM4->ARR = 1249;  												  //Auto Reload Register = wanted number - 1  ||  5s = 4ms * (1249+1)
	TIM4->PSC = 63999;											  	//Prescaler register = wanted number - 1    ||  250Hz = 16MHz/(63999+1)
	
	
	TIM4->CR1 |= TIM_CR1_CEN;	  								//Enable Timer
	//TIM6->CR1 |= TIM_CR1_CEN | TIM_CR1_UDIS;	  //Enable Timer and Disable Update for test this bit(UDIS=update disable)(See in Simulation SR Bit)
	//----------------------------------------------------------------------
	 	
	while(1)
	{
				
		if(timercounter > 1)
		{			
			timercounter=0;
			reset_pin();														 //OFF LED
		}
		else if(timercounter > 0)
			set_pin();															 //on LED
		
	}//end while
}//end main

//Config GPIO PIN9 from GPIOB OUTPUT
void config_port(void){
	GPIOB->MODER |= GPIO_MODER_MODE9_0;					 //output pin9 from port B
}

//PIN9 From GPIOB = 1
void set_pin(void){
	GPIOB->ODR |= GPIO_ODR_OD9;									 //set_pin pin9 = high
}

//PIN9 From GPIOB = 0
void reset_pin(void){
	GPIOB->ODR &= ~GPIO_ODR_OD9;								 //reset pin9 = low
}

//Run Clock GPIOB
void clockON_PORT(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;				 //clock GPIOB ON
}




//**********************************************************************
/*------------------------------------------------------------------------------
  Timer6 Update Interrupt Handler(in handler from Startup_STM32f407xx.s
 *------------------------------------------------------------------------------*/
void TIM4_IRQHandler() 
{
	TIM4->SR &= ~TIM_SR_UIF;								     //Reset Flag update for run intrupt next update
	timercounter++;															 //counter added one for on|off LED
} // end TIM6_UP_IRQHandler
//**********************************************************************
