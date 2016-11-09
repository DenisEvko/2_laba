#include "stm32f10x.h"

#define LED2_ON()		GPIOC->BSRR |= GPIO_BSRR_BS8
#define LED2_OFF()	GPIOC->BSRR |= GPIO_BSRR_BR8

#define LED1_ON()		GPIOC->BSRR |= GPIO_BSRR_BS9
#define LED1_OFF()	GPIOC->BSRR |= GPIO_BSRR_BR9


#define LED1_IS_ON()	(GPIOC->ODR & GPIO_ODR_ODR9) > 0
#define BUTTON_IS_ON() (GPIOA->IDR & GPIO_IDR_IDR0) > 0

void init_diod();
void init_but();
void temp_delay(uint32_t delayTime);
//void EXTI0_IRQHandler();
 
int main() {
	
	init_diod();
	init_but();
	
	LED1_ON();
	LED2_ON();
	
	while(1) {
		
	}
	
}


void init_diod() {
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH = 0;
	
	GPIOC->CRH |= GPIO_CRH_MODE8_1;	
	GPIOC->CRH |= GPIO_CRH_MODE9_1;//Output mode, max speed 2 MHz.
	//GPIOC->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF8_1);	
	
}

void init_but() {
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0); //сброс
	GPIOA->CRL  |= GPIO_CRL_CNF0_1;
	
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PA; //External interrupt configuration 1 || EXTI0_PA - register 0pin portA
	
	//	EXTI->FTSR |= EXTI_FTSR_TR9;			//Прерывание по спаду импульса (при нажатии на кнопку)
	// EXTI->IMR |= EXTI_IMR_MR9;				//Выставляем маску - EXTI9
	
	EXTI->RTSR |= EXTI_RTSR_TR0; //Rising trigger enabled (for Event and Interrupt) for input line.
	EXTI->IMR |= EXTI_IMR_MR0;	//Interrupt request from Line x is not masked
	
	//NVIC_EnableIRQ(EXTI9_5_IRQn);			//Разрешаем прерывание
	//NVIC_SetPriority(EXTI9_5_IRQn, 0);		//Выставляем приоритет
	
	NVIC_EnableIRQ(EXTI0_IRQn); //EXTI Line0 Interrupt 
	NVIC_SetPriority(EXTI0_IRQn, 0);// set priority
	
}

void EXTI0_IRQHandler() {
	
	temp_delay(10000);
	
	if (BUTTON_IS_ON()) {
		if(LED1_IS_ON()) {
			LED1_OFF();
			LED2_ON();
		}
		else {
			LED2_OFF();
			LED1_ON();
		}
	}
	
	EXTI->PR |= EXTI_PR_PR0;
			
}

void temp_delay(uint32_t delayTime)
{
	for(; 0 < delayTime; delayTime--)
	{
		
	}
}


