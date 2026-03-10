#include <msp430.h>
#include "gpio.h"
#include "button.h"

// Test LED pins (P1.0 and P4.0)
#define LED1 BIT0  // P1.0 - toggles with START_STOP button
#define LED2 BIT0  // P4.0 - toggles with RESET button

// Test action functions
void test_action_button1(void)
{
    // Toggle LED1 (P1.0) when START_STOP button is pressed
    P1OUT ^= LED1;
}

void test_action_button2(void)
{
    // Toggle LED2 (P4.0) when RESET button is pressed
    P4OUT ^= LED2;
}

// Main entry point - GPIO and Button test
int main(void)
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    
    // Initialize GPIO - sets all ports to output
    GPIO_Init();
    
    // Configure test LEDs on P1.0 and P4.0
    P1DIR |= LED1;  // Set P1.0 as output
    P4DIR |= LED2;  // Set P4.0 as output
    P1OUT &= ~LED1; // Turn off P1.0 LED
    P4OUT &= ~LED2; // Turn off P4.0 LED
    
    // Initialize button input detection with Timer_A0 for debouncing
    Button_Init();
    
    // Enable Timer_A0 interrupt for button debouncing
    TA0CCTL0 = CCIE;
    TA0CCR0 = 1000 - 1;  // 1ms timer
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;
    
    // Enable global interrupts
    __enable_interrupt();
    
    // Main loop - process button presses
    unsigned char btnLast[2] = {0, 0};
    
    while(1)
    {
        // Read button 1 (START_STOP on P1.2) and execute test_action_button1 on press
        Button_Handler(!(P1IN & START_STOP_BUTTON), &btnLast[0], 0, test_action_button1);
        
        // Read button 2 (RESET on P2.6) and execute test_action_button2 on press
        Button_Handler(!(P2IN & RESET_BUTTON), &btnLast[1], 1, test_action_button2);
        
        __no_operation();
    }
}
