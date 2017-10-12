#include <msp430g2553.h>

unsigned int freqCalc(unsigned int number);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    P1DIR |= (BIT0 + BIT6);   // Set P1.0, P1.6 to output
    P1OUT &= ~(BIT0 + BIT6);  // Initialize P1.0 and P1.6 to off
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
    P1OUT ^= (BIT0 + BIT6);   // Toggle P1.6 (LED)
}

unsigned int freqCalc(unsigned int desired)
{
    return 65536 / desired;
}
