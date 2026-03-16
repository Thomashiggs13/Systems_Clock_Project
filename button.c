#include "button.h"

// Button state tracking
volatile unsigned int btnTime[2] = {0, 0};
volatile unsigned char btnPressed[2] = {0, 0};

void Button_Init(void)
{
    // Configure START/STOP button (P1.2)
    P1DIR &= ~START_STOP_BUTTON;
    P1REN |= START_STOP_BUTTON;
    P1OUT |= START_STOP_BUTTON;
    
    // Configure RESET button (P2.6)
    P2DIR &= ~RESET_BUTTON;
    P2REN |= RESET_BUTTON;
    P2OUT |= RESET_BUTTON;
}

void Button_Handler(unsigned char curr, unsigned char *last, int index, void (*action)(void))   //Action is a function pointer to the action to execute on button press
{
    if(curr && !(*last)) {      
        btnTime[index] = 0;
        btnPressed[index] = 1;
    }
    if(!curr && *last) {
        btnPressed[index] = 0;
        if(btnTime[index] >= 50)
            action();
        btnTime[index] = 0;
    }
    *last = curr;
}
