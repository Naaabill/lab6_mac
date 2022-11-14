#include "stm32f3xx.h"
#include "pinAccess.h"

void wait() {
  volatile int i = 0;
  for (i = 0; i < 2000000; i++)
    ;
}

void setup() {
  // PB3 (led) as output
  pinMode(GPIOB,3, OUTPUT);
}

/* main function */
int main(void) {
  setup();
  /* Infinite loop */
  while (1) {
    /* Add application code here */
    digitalWrite(GPIOB,3,1);
    wait();
    digitalWrite(GPIOB,3,0);
    wait();
  }
}

