#include <msp430f5529.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    P4DIR |= BIT7;            // Set P1.6 to output
    P1DIR &= ~BIT1;            // Set P1.3 to input
    P1REN |= BIT1;            // Set P1.3 resistor enable...
    P1OUT |= BIT1;            // ...to pullup resistor
    P1IE |= BIT1;             // Set P1.3 as interrupt pin

    __enable_interrupt();     // Enable interrupt code
    while(1)
    {}              // By default, do nothing continuously
}

#pragma vector=PORT1_VECTOR   // Define interrupt vector, in this case Port 1
__interrupt void Port_1(void) // Define interrupt function that runs whenever interrupt is detected
{
    P4OUT ^= BIT7;            // Toggle P1.6 (LED)
    P1IFG = ~BIT1;            // Disable interrupt flag to re-enable interrupt
}
