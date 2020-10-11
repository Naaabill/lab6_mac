#include "stm32f3xx.h"
#include "sw.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include "tft.h"

void setup() {
    SWInit();
    Tft.setup();
}

int main(void) {

    volatile int temps;
    setup();

    Tft.setTextCursor(5,0);

    SWReset();
    Tft.print("ITII ");
    temps = SWGet();
    Tft.print(temps);

    /* Infinite loop */
    while (1) {

    }
}

