#include "gpio.h"

void GPIO_Init(void)
{
    // Initialize all port output registers to 0 and direction to output
    P1OUT = P2OUT = P3OUT = P4OUT = P5OUT = P6OUT = P7OUT = P8OUT = 0;
    P1DIR = P2DIR = P3DIR = P4DIR = P5DIR = P6DIR = P7DIR = P8DIR = 0xFF;
}
