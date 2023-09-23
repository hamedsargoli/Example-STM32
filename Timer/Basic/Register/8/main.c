/*****************************************************************
- Example 8: Increase the time using the OPM feature in timer 7 ( 5 secend for blinking led)
					  Intrupt Enable Timer 6 and 7
					* One Pulse Mode ACtive for Timer 7
						Freg(CK_CNT) = 250 Hz  for Timer 6 and 7 
					  
- Date: 2023-09-19
- Directed by: Hamed Sargoli

*****************************************************************/
//Header file
#include "stm32f407xx.h"

//Global Variable
volatile int timercounter = 0 , OPMCounter = 0;

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
	// Initialize the interrupt (constant import from stm32f4xx.h)
	__NVIC_EnableIRQ(TIM6_DAC_IRQn);
	__NVIC_EnableIRQ(TIM7_IRQn);

	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN | RCC_APB1ENR_TIM7EN;					//clock Timer6  AND Timer7 ON
  
	TIM6->DIER |= TIM_DIER_UIE;									//Enable Intrupt Timer6
	TIM7->DIER |= TIM_DIER_UIE;									//Enable Intrupt Timer7
	
	TIM6->ARR = 249;												    //Auto Reload Register = wanted number - 1  ||  1 s     = 4ms   * (249+1)
	TIM6->PSC = 63999;											  	//Prescaler register = wanted number - 1    ||  250 Hz  = 16MHz / (63999+1)
	TIM7->ARR = 249;												    //Auto Reload Register = wanted number - 1  ||  1 s     = 4ms   * (249+1)
	TIM7->PSC = 63999;											  	//Prescaler register = wanted number - 1    ||  250 Hz  = 16MHz / (63999+1)
	
	//***********************************************************************
	TIM7->CR1 |= TIM_CR1_OPM;
	//***********************************************************************
	
	TIM6->CR1 |= TIM_CR1_CEN;	  								//Enable Timer
	TIM7->CR1 |= TIM_CR1_CEN;	  								//Enable Timer
	//----------------------------------------------------------------------
	 	
	while(1)
	{
				
		if(timercounter >= 2)
		{			
			timercounter=0;
			reset_pin();														//OFF LED
		}
		else if(timercounter >= 1)
			set_pin();															//on LED
		
	}//end while
}//end main


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


/*------------------------------------------------------------------------------
  Timer6 Update Interrupt Handler(in handler from Startup_STM32f407xx.s
 *------------------------------------------------------------------------------*/
void TIM6_DAC_IRQHandler() 
{
	TIM6->SR &= ~TIM_SR_UIF;										//Reset Flag update for run intrupt next update
	OPMCounter++;													 		  //for extra time
	if(OPMCounter > 3)
	{
		OPMCounter=0;													
		TIM7->CR1 |= TIM_CR1_CEN;									//enable timer 7(OPM bit = 1)
	}
} // end TIM6

/*------------------------------------------------------------------------------
  Timer7 Update Interrupt Handler(in handler from Startup_STM32f407xx.s
 *------------------------------------------------------------------------------*/

void TIM7_IRQHandler() 
{
	TIM7->SR &= ~TIM_SR_UIF;										//Reset Flag update for run intrupt next update
	timercounter++;															//counter added one for on|off LED
} // end TIM7
