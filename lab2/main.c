#include "stm32f3xx.h"
#include "pinAccess.h"

void wait() {
  volatile int i = 0;
  for (i = 0; i < 700000; i++)
    ;
}

void setup() {
  // PB3 (led) as output
  pinMode(GPIOB,0, OUTPUT);
  pinMode(GPIOB,1,INPUT_PULLUP);
  pinMode(GPIOB,6,INPUT_PULLUP);
 
  
}

void charlieplexing(uint8_t mask);

void setLed(int n){
    // D'abord mettre toutes les LEDs en haute impédance
    pinMode(GPIOB,7,INPUT);
    pinMode(GPIOF,0,INPUT);
    pinMode(GPIOF,1,INPUT);

    if(n==1){
        digitalWrite(GPIOB,7,1); pinMode(GPIOB,7,OUTPUT);
        digitalWrite(GPIOF,0,0); pinMode(GPIOF,0,OUTPUT);
        // GPIOF,1 reste en input (Z)
    } else if(n==2){
        digitalWrite(GPIOB,7,0); pinMode(GPIOB,7,OUTPUT);
        digitalWrite(GPIOF,0,1); pinMode(GPIOF,0,OUTPUT);
        
    } else if(n==3){
        digitalWrite(GPIOF,0,1); pinMode(GPIOF,0,OUTPUT);
        digitalWrite(GPIOF,1,0); pinMode(GPIOF,1,OUTPUT);
      
    } else if(n==4){
        digitalWrite(GPIOF,0,0); pinMode(GPIOF,0,OUTPUT);
        digitalWrite(GPIOF,1,1); pinMode(GPIOF,1,OUTPUT);
        
    } else if(n==5){
        pinMode(GPIOF,0,INPUT);
        digitalWrite(GPIOF,1,0); pinMode(GPIOF,1,OUTPUT);
        digitalWrite(GPIOB,7,1); pinMode(GPIOB,7,OUTPUT);
    } else if(n==6){
        pinMode(GPIOF,0,INPUT);
        digitalWrite(GPIOF,1,1); pinMode(GPIOF,1,OUTPUT);
        digitalWrite(GPIOB,7,0); pinMode(GPIOB,7,OUTPUT);
    }
}
void charlieplexing(uint8_t mask){
   if(mask==0b00011111){
    for(int i=2;i<7;i++){
      setLed(i);
    }
  } else if(mask==0b00001111){
    for(int i=3;i<7;i++){
      setLed(i);
    }
  } else if(mask==0b00000111){
    for(int i=4;i<7;i++){
      setLed(i);
    }
  } else if(mask==0b00000011){
    for(int i=5;i<7;i++){
      setLed(i);
    }
  } else if(mask==0b00000001){
    
      setLed(6);
    
  } else {

  }
  
    
  
}
  


/* main function */
int main(void) {
  setup();
  uint8_t cpt=0b00011111;
  /* Infinite loop */
  while (1) {
    //setLed(cpt);
    charlieplexing(cpt);
    int bp = digitalRead(GPIOB,1);
    int bp5 = digitalRead(GPIOB,6);
    if(cpt==0b0000000){
      cpt=0b01111111;
    } else if (cpt>=0b1111111){
      cpt=0b00000000;
    }
  
    if(bp==0){
      cpt=cpt>>1;
      wait();
    } 
    else if(bp5==0){
      cpt=cpt<<1;
      wait();
    } 
    
  }
}

