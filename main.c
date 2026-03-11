#include <msp430.h>
#include "gpio.h"
#include "lcd.h"
#include "rtc.h"
#include "button.h"
#include "time.h"

// Button action functions
void toggleRunning(void)
{
    extern volatile int running;
    running ^= 1;
}

void resetTimer(void)
{
    Time_Reset();
}

// Main entry point - Complete Clock System
int main(void)
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    
    // Initialize all subsystems
    GPIO_Init();
    Button_Init();
    LCD_Init();
    Time_Init();
    RTC_Init();
    
    // Unlock GPIO pins from Low Power Mode 5
    PM5CTL0 &= ~LOCKLPM5;
    
    // Update display with initial time
    extern volatile unsigned char *Seconds;
    extern volatile unsigned char *Minutes;
    extern volatile unsigned char *Hours;
    
    LCDMEM[LCD_POS1] = LCD_GetDigit((*Hours)/10);
    LCDMEM[LCD_POS2] = LCD_GetDigit((*Hours)%10);
    LCDMEM[LCD_POS3] = LCD_GetDigit((*Minutes)/10);
    LCDMEM[LCD_POS4] = LCD_GetDigit((*Minutes)%10);
    LCDMEM[LCD_POS5] = LCD_GetDigit((*Seconds)/10);
    LCDMEM[LCD_POS6] = LCD_GetDigit((*Seconds)%10);
    LCDMEM[7]  = 0x04;  // Colon separator
    LCDMEM[11] = 0x04;  // Colon separator
    
    // Enable global interrupts
    __enable_interrupt();
    
    // Start the timer
    extern volatile int running;
    running = 1;
    
    // Main loop - handle buttons and wait for interrupts
    unsigned char btnLast[2] = {0, 0};
    
    while(1)
    {
        Button_Handler(!(P1IN & START_STOP_BUTTON), &btnLast[0], 0, toggleRunning);
        Button_Handler(!(P2IN & RESET_BUTTON), &btnLast[1], 1, resetTimer);
        __no_operation();
    }
}

// Timer_A0 ISR for button debounce timing
extern volatile unsigned int btnTime[2];
extern volatile unsigned char btnPressed[2];

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void)
{
    if(btnPressed[0]) btnTime[0]++;
    if(btnPressed[1]) btnTime[1]++;
}
