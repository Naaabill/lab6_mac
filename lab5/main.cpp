#include "stm32f3xx.h"
#include <iostream>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include "tft.h"
#include "pinAccess.h"
#include "exti.h"

volatile float measured_value = 0;

void setup()
{
	Tft.setup();
	

	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //enable TIM6 clock
	// reset peripheral
	__asm("nop");
	RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;
	__asm("nop");

	TIM6->PSC = 64000-1; // resolution 10hz
	TIM6->CNT = 0;
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6->ARR = 100-1; // 
	TIM6->CR1 |= TIM_CR1_CEN;

	TIM6->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM6_DAC1_IRQn);

	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; //enable TIM7 clock
	// reset peripheral
	__asm("nop");
	RCC->APB1RSTR |= RCC_APB1RSTR_TIM7RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM7RST;
	__asm("nop");

	TIM7->PSC = 64-1; //1Mhz
	TIM7->CNT = 0;
	TIM7->SR &= ~TIM_SR_UIF;
	TIM7->ARR = 0xFFFF; // resolution 1us
	TIM7->CR1 |= TIM_CR1_CEN;

	attachInterrupt(GPIOA,10,CHANGE);
	


}

extern "C" void EXTI15_10_IRQHandler(){

	if(digitalRead(GPIOA,10)){
		//rising edge
		TIM7->SR &= ~TIM_SR_UIF;
		TIM7->CNT = 0;
		
	} else {
		measured_value = TIM7->CNT;
		
	}
	EXTI->PR |= EXTI_PR_PR10; //clear pending bit
}

extern "C" void TIM6_DAC1_IRQHandler(){
	EXTI->IMR &= ~EXTI_IMR_MR10; //disable exti line 10
	pinMode(GPIOA,10,OUTPUT);
	digitalWrite(GPIOA,10,1);
	for(volatile int i=0;i<40;i++); // on mesure que un signal de 10us i<40
	digitalWrite(GPIOA,10,0);
	
	
	EXTI->IMR |= EXTI_IMR_MR10; //enable exti line 10
	pinMode(GPIOA,10,INPUT_PULLUP); 
	TIM6->SR &= ~TIM_SR_UIF;
}

//this is a simple application that uses the TFT display.
// => it first write a string in the top of the screen (in setup)
// => it writes the ADC value of the potentiometer in green, and
//    refreshes it each time the value differs more than 5.
int main()
{
	
	
	setup();
	while(1)
	{
	 	float distance = measured_value / 58.0f;
		
		float distance_mm = distance * 10.0f;
		
		// if(distance_mm < 0) distance_mm = 0;
		// if(distance_mm > 1000) distance_mm = 1000;
		// Tft.setTextColor(ST7735_RED);
		// Tft.setTextCursor(10, 20);
	
		
		// Tft.eraseText(20);
		// Tft.print(distance_mm);
		// Tft.print(" mm");

		Tft.fillRect(10, int(distance_mm), 50, 20, ST7735_RED); 

		// 2. Write the new value
		Tft.setCursor(10, 20);
		
		
		
        

		
	}
}

