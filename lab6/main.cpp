#include "stm32f3xx.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include "adc.h"
#include "tft.h"
#include "servo.h"
#include "enco.h"
#include "pinAccess.h"


enum etat {
	manuel,
	automa,
	transition_manuel_auto,
	transition_auto_manuel
};

enum btn {
	BAS,
	HAUT
};

void setup()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; // enable TIM7 clock
	// reset peripheral
	__asm("nop");
	RCC->APB1RSTR |= RCC_APB1RSTR_TIM7RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM7RST;
	__asm("nop");

	TIM7->PSC = 64000 - 1; // 1Khz 1ms
	TIM7->CNT = 0;
	TIM7->SR &= ~TIM_SR_UIF;
	TIM7->ARR = 50 - 1;
	TIM7->CR1 |= TIM_CR1_CEN;
	ADCInit();

	pinMode(GPIOB, 1, INPUT_PULLUP);
}

// this is a simple application that uses the TFT display.
//  => it first write a string in the top of the screen (in setup)
//  => it writes the ADC value of the potentiometer in green, and
//     refreshes it each time the value differs more than 5.
int main()
{
	static int move = 0;
	bool v=false;
	bool state = false;
	etat fsm = manuel;
	btn fsm_2 = BAS; 
	encoderInit();
	servoInit();
	setup();
	while (1)
	{

		int pb1 = digitalRead(GPIOB, 1);
		// float angle = 0+(90./4095)*ADCRead();
		// servoSet(angle);

		// int valeur_encoder = encoderValue();
		// servoSet(valeur_encoder*2);

		// if (state == false ){
		// 	if(TIM7->SR & TIM_SR_UIF){
		// 		TIM7->SR &= ~TIM_SR_UIF;
		// 		servoSet(move*2);
		// 		move++;
		// 	}
		// 	if (move > 45){
		// 		state = true;
		// 		move = 45;
		// 	}
		// }
		// if(state==true) {
		// 	if(TIM7->SR & TIM_SR_UIF){
		// 		TIM7->SR &= ~TIM_SR_UIF;
		// 		servoSet(move*2);
		// 		move--;
		// 	}
		// 	if (move < 0){
		// 		state = false;
		// 		move = 0;
		// 	}
		// }
		switch(fsm_2)
		{
			case BAS:
				if(pb1 == 0) {          // front descendant détecté
					fsm_2 = HAUT;       // passe à l’état appuyé
					v = true;           // action sur appui
				}
				break;

			case HAUT:
				if(pb1 == 1) {          // bouton relâché
					fsm_2 = BAS;        // retourne à l’état relâché
					v = false;          // action sur relâche
				}
				break;
		}

		int valeur_encoder = encoderValue();

		
		switch (fsm)
		{
		case manuel:
			if(v) fsm = transition_manuel_auto;
			servoSet(valeur_encoder * 2);
			break;

		case automa:
			if(v) fsm = transition_auto_manuel;
			if (state == false ){
				if(TIM7->SR & TIM_SR_UIF){
					TIM7->SR &= ~TIM_SR_UIF;
					servoSet(move*2);
					move++;
				}
				if (move > 45){
					state = true;
					move = 45;
				}
			}
			if(state==true) {
				if(TIM7->SR & TIM_SR_UIF){
					TIM7->SR &= ~TIM_SR_UIF;
					servoSet(move*2);
					move--;
				}
				if (move < 0){
					state = false;
					move = 0;
				}
			}
			break;

		case transition_auto_manuel:
			while(move!=valeur_encoder){
				if (state == false ){
					if(TIM7->SR & TIM_SR_UIF){
						TIM7->SR &= ~TIM_SR_UIF;
						servoSet(move*2);
						move++;
					}
					if (move > 45){
						state = true;
						move = 45;
					}
				}
				if(state==true) {
					if(TIM7->SR & TIM_SR_UIF){
						TIM7->SR &= ~TIM_SR_UIF;
						servoSet(move*2);
						move--;
					}
					if (move < 0){
						state = false;
						move = 0;
					}
				}
			}
			if(!v)fsm = manuel;
			break;
		case transition_manuel_auto:
			if(!v){
				move = valeur_encoder;
				fsm = automa;
			}
			
			break;
		default:
			break;
		}


	}
}
