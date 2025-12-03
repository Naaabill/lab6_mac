#include "stm32f3xx.h"
#include "pinAccess.h"

void servoInit(){
    // 1. Activer l'horloge pour le Timer 0
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    __asm("nop");
    // 2. Activer l'horloge pour le GPIOB (IMPORTANT si pinMode ne le fait pas tout seul)
    RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;
    __asm("nop");
    // Reset du timer pour être propre
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;
    __asm("nop");
    // 3. Configuration de la vitesse (20ms)
    // Horloge système = 64 MHz
    TIM3->PSC = 64 - 1;    // 1 tick = 1 µs
    TIM3->ARR = 20000 - 1; // 20000 ticks = 20 ms
    TIM3->CNT = 0;
    
    pinAlt(GPIOB, 0, 2);
    TIM3->CCMR2 &= ~TIM_CCMR2_CC3S; // Reset
    TIM3->CCMR2 |= (6UL << 4);      // PWM Mode 1 sur Channel 3 (OC3M)
    TIM3->CCMR2 |= TIM_CCMR2_OC3PE; // Preload

    TIM3->CCER |= TIM_CCER_CC3E;    // Activer la sortie 3
    TIM3->CCR3 = 1500-1; // Mettre la valeur dans CCR3

     TIM3->CR1 |= TIM_CR1_CEN;   
}

void servoSet(int deg){
    float value = 1000+(1000./90.)*deg;
    TIM3->CCR3 = int(value)-1;
}

// photo toggle 20ms car on devait faire une interruption pour tester que c'etait toutes les 20 ms
//deuxieme photo pwm 1500-1, sur 20ms donc 1,5ms sur 20ms