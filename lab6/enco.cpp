#include "stm32f3xx.h"
#include "pinAccess.h"
#include "exti.h"
#include "adc.h"
#include "tft.h"

volatile int valueEncode = 0;

/* init encoder hardware. Set value to 0.*/


void encoderInit (){
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //enable TIM7 clock
	// reset peripheral
	__asm("nop");
	RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;
	__asm("nop");

	TIM6->PSC = 64-1; //1Mhz
	TIM6->CNT = 0;
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6->ARR = 0xFFFF; // resolution 1us
	TIM6->CR1 |= TIM_CR1_CEN;

	attachInterrupt(GPIOA,0,RISING);
    pinMode(GPIOA,1,INPUT_PULLUP);
    valueEncode = 0;
}
/* return the encoder value , in the range [0,45] */
int encoderValue (){
    return valueEncode;
}

extern "C" void EXTI0_IRQHandler(){
    if(digitalRead(GPIOA,1) == 0){
        valueEncode++;
    } else if (digitalRead(GPIOA,1) == 1){ 
        valueEncode--;
    } 

    if (valueEncode < 0){
        valueEncode = 0;
    } else if (valueEncode > 45){
        valueEncode = 45;
    }
    EXTI->PR |= EXTI_PR_PR0;
}