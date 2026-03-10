#include <msp430.h>
#include "lcd.h"

// Test time values for LCD display
volatile unsigned char testSeconds = 0;
volatile unsigned char testMinutes = 0;
volatile unsigned char testHours = 0;

// Forward declaration
void update_test_display(void);

// Test action - increment test hours (0-23)
void increment_hours(void)
{
    testHours++;
    if(testHours >= 24) testHours = 0;
    update_test_display();
}

// Test action - increment test minutes (0-59)
void increment_minutes(void)
{
    testMinutes++;
    if(testMinutes >= 60) testMinutes = 0;
    update_test_display();
}

// Update the LCD display with test values
void update_test_display(void)
{
    LCDMEM[LCD_POS1] = LCD_GetDigit(testHours / 10);
    LCDMEM[LCD_POS2] = LCD_GetDigit(testHours % 10);
    LCDMEM[LCD_POS3] = LCD_GetDigit(testMinutes / 10);
    LCDMEM[LCD_POS4] = LCD_GetDigit(testMinutes % 10);
    LCDMEM[LCD_POS5] = LCD_GetDigit(testSeconds / 10);
    LCDMEM[LCD_POS6] = LCD_GetDigit(testSeconds % 10);
    LCDMEM[7]  = 0x04;  // Colon separator
    LCDMEM[11] = 0x04;  // Colon separator
}

// Main entry point - LCD test
int main(void)
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    
    // Initialize LCD controller
    LCD_Init();
    
    // Set initial test time: 12:34:56
    testHours = 12;
    testMinutes = 34;
    testSeconds = 56;
    
    // Display the initial time on LCD
    update_test_display();
    
    // Enable global interrupts
    __enable_interrupt();
    
    // Main loop - simple test cycling through times
    volatile unsigned int testCounter = 0;
    
    while(1)
    {
        // Test: increment seconds every ~1 second (just for visual testing)
        testCounter++;
        if(testCounter > 20000)
        {
            testCounter = 0;
            testSeconds++;
            if(testSeconds >= 60)
            {
                testSeconds = 0;
                testMinutes++;
                if(testMinutes >= 60)
                {
                    testMinutes = 0;
                    testHours++;
                    if(testHours >= 24) testHours = 0;
                }
            }
            update_test_display();
        }
        
        __no_operation();
    }
}
