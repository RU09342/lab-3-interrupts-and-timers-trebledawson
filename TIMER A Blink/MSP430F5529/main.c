#include <msp430.h>

unsigned int freqCalc(unsigned int number);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    P1DIR |= BIT0;   // Set P1.0 to output LED
    P4DIR |= BIT7;   // Set P4.7 to output LED
    P1OUT &= ~BIT0; // Initialize LEDs to off
    P4OUT &= !BIT7;  // Initialize LEDs to off
    TA0CTL = TASSEL_2 + MC_1 + ID_3 + TACLR; // Use SMCLK in up mode, /8 divider
    unsigned int desired = 16;          // Set desired frequency, f <= 16000
    unsigned int counts = freqCalc(desired); // counts = ACLK / (2*desired)

    TA0CCTL0 = CCIE;          // Enable capture/compare interrupt
    TA0CCTL0 &= 0x10;          // Set to compare mode
    TA0CCR0 = counts;          // Set TA0CCR0 to calculated counter number
    __enable_interrupt();     // Enable interrupt code
    while(1)
    {} // Run

}

#pragma vector=TIMER0_A0_VECTOR  // Define interrupt vector, in this case Port 1
__interrupt void Port_1(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1OUT ^= BIT0;   // Toggle LEDs
    P4OUT ^= BIT7;   // Toggle LEDs
}

unsigned int freqCalc(unsigned int desired)
{
    return 65536 / desired;
}
