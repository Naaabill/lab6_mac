#include "stm32f3xx.h"
#include <iostream>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include "tft.h"
#include "pinAccess.h"
#include "exti.h"

volatile float measured_value = 0; // valeur mesurée par le timer (durée de l'echo)

void setup()
{
	Tft.setup(); // initialise l'écran TFT
	
	//--------------------------------------------------------------
	// CONFIGURATION DU TIMER TIM6 (sert à envoyer le Trigger à 10 Hz)
	//--------------------------------------------------------------
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // active l'horloge pour TIM6

	RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST; // reset du timer
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;

	TIM6->PSC = 64000-1; // on divise la fréquence -> 1 kHz
	TIM6->CNT = 0;
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6->ARR = 100-1; // 1 kHz / 100 = 10 Hz => interruption toutes les 100ms
	TIM6->CR1 |= TIM_CR1_CEN; // démarre le timer

	TIM6->DIER |= TIM_DIER_UIE; // autorise l'interruption update
	NVIC_EnableIRQ(TIM6_DAC1_IRQn); // active dans le NVIC

	//--------------------------------------------------------------
	// CONFIGURATION DU TIMER TIM7 (sert à mesurer le temps Echo)
	//--------------------------------------------------------------
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; // active l'horloge TIM7

	RCC->APB1RSTR |= RCC_APB1RSTR_TIM7RST; // reset du timer
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM7RST;

	TIM7->PSC = 64-1; // fréquence 1 MHz -> 1 tick = 1 microseconde
	TIM7->CNT = 0;
	TIM7->SR &= ~TIM_SR_UIF;
	TIM7->ARR = 0xFFFF; // compteur sur 16 bits
	TIM7->CR1 |= TIM_CR1_CEN; // démarre TIM7

	//--------------------------------------------------------------
	// ACTIVATION DE L'INTERRUPTION SUR PA10 (Echo + Trigger)
	//--------------------------------------------------------------
	attachInterrupt(GPIOA,10,CHANGE); // interruption sur front montant et descendant
}
	

//--------------------------------------------------------------
// INTERRUPTIONS EXTI (gestion du signal Echo)
//--------------------------------------------------------------
extern "C" void EXTI15_10_IRQHandler(){

	if(digitalRead(GPIOA,10)){
		// FRONT MONTANT = début du signal Echo
		TIM7->SR &= ~TIM_SR_UIF; // clear du flag
		TIM7->CNT = 0; // on remet à zéro le timer (début du chrono)
		
	} else {
		// FRONT DESCENDANT = fin du signal Echo
		measured_value = TIM7->CNT; // on récupère la durée du pulse echo
	}
	EXTI->PR |= EXTI_PR_PR10; // on efface le pending bit
}


//--------------------------------------------------------------
// INTERRUPTIONS TIM6 (envoi du Trigger ultrason)
//--------------------------------------------------------------
extern "C" void TIM6_DAC1_IRQHandler(){

	EXTI->IMR &= ~EXTI_IMR_MR10; // désactive l'interruption echo pendant le trigger

	pinMode(GPIOA,10,OUTPUT);   // PA10 en sortie pour envoyer le trigger
	digitalWrite(GPIOA,10,1);   // met PA10 à 1 = début Trigger
	for(volatile int i=0;i<40;i++); // petite pause ~10us pour respecter le capteur
	digitalWrite(GPIOA,10,0);   // met PA10 à 0 = fin Trigger
	
	EXTI->IMR |= EXTI_IMR_MR10;  // réactive l'interruption Echo
	pinMode(GPIOA,10,INPUT_PULLUP); // PA10 redevient une entrée

	TIM6->SR &= ~TIM_SR_UIF; // efface le flag d'interruption TIM6
}


//--------------------------------------------------------------
// PROGRAMME PRINCIPAL
//--------------------------------------------------------------
int main()
{
	setup();

	while(1)
	{
		// --- Positionnement des zones d'affichage ---
		const int Y_BAS = 100;    
		const int Y_HAUT = 20;    
		const int MAX_PIXELS = Y_BAS - Y_HAUT;
		const int BAR_X = 53;

		// --- Conversion du temps mesuré en distance ---
		float distance = measured_value / 58.0f; // 58us/cm
		float distance_mm = distance * 10.0f;   // mm

		// Sécurité : on borne la distance
		if (distance_mm < 0) distance_mm = 0;
		if (distance_mm > 1000) distance_mm = 1000;

		// Hauteur de la barre rouge
		int bar_height = (int)((distance_mm / 1000.0f) * MAX_PIXELS);

		// Point de départ de la barre (en haut)
		int y_depart = Y_BAS - bar_height;

		// --- Efface la zone au-dessus de la barre ---
		Tft.fillRect(BAR_X, Y_HAUT, 50, y_depart - Y_HAUT, ST7735_BLACK);

		Tft.setCursor(33, 120);
		Tft.eraseText(12+20);

		// --- Choix de la couleur de la barre selon la distance ---
		if(distance_mm <= 100){
			Tft.setTextColor(ST7735_RED, ST7735_BLACK);
			Tft.fillRect(BAR_X, y_depart, 50, bar_height, ST7735_RED);

		} else if (distance_mm > 100 && distance_mm <= 500){
			Tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
			Tft.fillRect(BAR_X, y_depart, 50, bar_height, ST7735_YELLOW);

		} else {
			Tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
			Tft.fillRect(BAR_X, y_depart, 50, bar_height, ST7735_GREEN);
		}

		// --- Affichage de la distance ---
		Tft.print("distance: ");

		if(distance_mm > 100){
			Tft.print((int)distance);
			Tft.print(" cm");
		} else {
			Tft.print((int)distance_mm);
			Tft.print(" mm");
		}
	}
}
