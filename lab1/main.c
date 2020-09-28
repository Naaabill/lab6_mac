#include "stm32f3xx.h"

void wait() {
  volatile int i = 0;
  for (i = 0; i < 200; i++)
    ;
}

void setup() {
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN_Msk; // clock for GPIOB
  __asm("nop");                          // wait until GPIOB clock is Ok.
  GPIOC->MODER = 1 << (3 * 2);           // PB3 output
}

/* main function */
int main(void) {
  setup();
  /* Infinite loop */
  while (1) {
    /* Add application code here */
    GPIOC->ODR |= 1 << 3;
    // GPIOC->BSRR = 1 << 3; // bit set
    // wait();
    // GPIOC->BSRR = 1 << (3 + 16); // bit reset
    GPIOC->ODR &= ~(1 << 3); // bit reset
    // wait();
  }
}

