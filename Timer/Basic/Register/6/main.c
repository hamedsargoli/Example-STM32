/*****************************************************************
- Example 6: 
					  Intrupt Enable
					* Test ARPE BIT in Simulation(check CNT Register and ARR Register ...)(Line 44 , 59~64 , 93)
						Freg(CK_CNT) = 250 Hz  
					  
- Date: 2023-09-19
- Directed by: Hamed Sargoli
*****************************************************************/
//Header file
#include "stm32f407xx.h"
#include "stddef.h"

//Global Variable
volatile int timercounter = 0 , CounterARPE = 1;

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
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;					//clock Timer6 ON
  
	TIM6->DIER |= TIM_DIER_UIE;									//Enable Intrupt
	
	TIM6->ARR = 0x4000;												  //Auto Reload Register = wanted number - 1  ||  65.54 s = 4ms * (16384+1)
	TIM6->PSC = 63999;											  	//Prescaler register = wanted number - 1    ||  250 Hz  = 16MHz / (63999+1)
	//***********************************************************************
	TIM6->CR1 |= TIM_CR1_ARPE;									//ARR is buffered
	//***********************************************************************
	TIM6->CR1 |= TIM_CR1_CEN;	  								//Enable Timer
	//----------------------------------------------------------------------
	 	
	while(1)
	{
				
		if(timercounter > 1)
		{			
			timercounter=0;
			reset_pin();														//OFF LED
		}
		else if(timercounter > 0)
			set_pin();															//on LED
		//***********************************************************************
		if(CounterARPE > 4)
			TIM6->CR1 &= ~TIM_CR1_ARPE;							//ARR is not buffered
		
		TIM6->ARR = (int)(0x4000 / CounterARPE);	//change ARR
		//***********************************************************************
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


//**********************************************************************
/*------------------------------------------------------------------------------
  Timer6 Update Interrupt Handler(in handler from Startup_STM32f407xx.s
 *------------------------------------------------------------------------------*/
void TIM6_DAC_IRQHandler() 
{
	TIM6->SR &= ~TIM_SR_UIF;								//Reset Flag update for run intrupt next update
	timercounter++;													//counter added one for on|off LED
	//***********************************************************************
	CounterARPE *=2;	
	//***********************************************************************
} // end TIM6_UP_IRQHandler
//**********************************************************************
