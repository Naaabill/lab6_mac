#include "stm32f3xx.h"
#include "pinAccess.h"
#include "adc.h"
const unsigned char seq[]={8 ,0xc ,4,6,2,3,1,9};



void setup()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Msk | RCC_AHBENR_GPIOBEN_Msk; // enable GPIOA and GPIOB clock
    __asm("nop");

    
    // clock = 64MHz
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // active clock
    __asm("nop");
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST; // reset TIM6
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST; // clear reset TIM6
    __asm("nop");

    TIM6->PSC = 64000-1; // prescaler 64MHz/64000=1kHz
    TIM6->CNT = 0; // Remet à zéro le compteur
    TIM6->SR &= ~TIM_SR_UIF; // inverse le flag 
    TIM6->ARR = (100)-1; // 10hz valeur de remise max
    TIM6->CR1 |= TIM_CR1_CEN; // active le timer

    ADCInit(); //Initialisation de l'ADC

    pinMode(GPIOA, 8, OUTPUT);
    pinMode(GPIOA, 7, OUTPUT);
    pinMode(GPIOA, 6, OUTPUT);
    pinMode(GPIOA, 5, OUTPUT);

    
    pinMode(GPIOB, 1, INPUT_PULLUP);
    pinMode(GPIOB, 3, OUTPUT);
}



void stepCCW(int count){
    // on utilise static pour garder la valeur entre les appels
    static int count =0; 
    // Effacer PA5..PA8
    GPIOA->ODR &= ~(0xF << 5);
    // Appliquer la séquence correcte au GPIOA
    GPIOA->ODR |= (seq[count] & 0xF) << 5; 
    count++;
    // Si on dépasse la fin de la séquence, on revient au début
    if(count > 7) count = 0;


    static int count =0;
    // Même chose avec le registre BSRR
    GPIOA->BSRR = seq[count]<<5 | (~seq[count])<<(5+16); 
    count++;
    if(count > 7) count = 0;
}

void stepCW(){
    // on utilise static pour garder la valeur entre les appels
    static int count =7; 
    GPIOA->BSRR = seq[count]<<5 | (~seq[count])<<(5+16);
    count--;
    if(count < 0) count = 7;
}

/* main function */
int main(void)
{
    setup();
    /* Infinite loop */
    while (1)
    {
        static int running = 0;
        static int rotation = 0;
        uint16_t valeurperiode = 100-(98./4095)*ADCRead();
        TIM6->ARR = valeurperiode-1;
        
        int D6 = digitalRead(GPIOB,1); // lire l'état du bouton
        if(D6==1){ // bouton relâché
                static int count =0;
                stepCCW(count); // tourner dans le sens anti-horaire
                count++;
                if(count > 7) count = 0;
        } else if (D6==0) { // bouton appuyé
                static int count = 7;
                stepCW(count); // tourner dans le sens horaire
                count--;
                if(count < 0) count = 7;
        }
        
        if(D6==0){
            running = 1; // si bouton appuyé, démarrer la rotation automatique
        }
        if (running == 1){ 
            static int r=0;
            // modulo pour que quand on appuie une fois, ça tourne dans un sens, et la fois suivante dans l'autre
            if(r%2==0){ 
                if(rotation<4096){ // nombre de pas pour faire une rotation complète
                    static int count = 7;
                    stepCW(count); // j'ai modifié la fonction pour qu'elle prenne en argument le count
                    count--;
                    rotation++; 
                    if(count < 0) {
                        count = 7;   
                    } 
                } else {
                    rotation=0;
                    running =0;
                    r++;
                }
            } else if (r%2!=0){ // tourne dans l'autre sens quand réappuyé
                if(rotation<4096){
                    static int count =0;
                    stepCCW(count); 
                    count++;
                    if(count > 7) {
                        count = 0;  
                    }
                    rotation++;
                }   else {
                    rotation=0;
                    running =0;
                    r++;
                }
            }
        } 
  
        TIM6->SR &= ~TIM_SR_UIF; // clear le flag
        
        while (!(TIM6->SR & TIM_SR_UIF)); // attendre le flag
        
    }
}
