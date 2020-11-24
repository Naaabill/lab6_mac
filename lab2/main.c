#include "stm32f3xx.h"

void wait() {
  volatile int i = 0;
  for (i = 0; i < 2000000; i++)
    ;
}

void setup() {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN_Msk; // clock for GPIOB
  __asm("nop");                          // wait until GPIO clock is Ok.

  // PB0 output
  GPIOB->MODER &= ~GPIO_MODER_MODER0_Msk;
  GPIOB->MODER |=  GPIO_MODER_MODER0_0;
  GPIOB->BSRR = GPIO_BSRR_BR_0;
}

/* main function */
int main(void) {
  setup();
  /* Infinite loop */
  while (1) {
    /* Add application code here */
    GPIOB->ODR |= GPIO_ODR_0; // bit set
    wait();
    GPIOB->ODR &= ~GPIO_ODR_0; // bit reset
    wait();
  }
}

