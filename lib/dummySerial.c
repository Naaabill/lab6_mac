#include "adc.h"
#include "stm32f3xx.h"


void serialInit()
{

}

void serialPutChar(char c)
{
    USART2->TDR = c;
}

void serialPutString(char *c)
{
    while(*c) serialPutChar(*c++);
}

