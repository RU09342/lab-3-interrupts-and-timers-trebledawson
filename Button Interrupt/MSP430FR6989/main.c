#include <msp430.h>
#define LED1 BIT0
#define BUTTON BIT2

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= LED1;            // Set P1.6 to output
    P1DIR &= ~BUTTON;            // Set P1.3 to input
    P1REN |= BUTTON;            // Set P1.3 resistor enable...
    P1OUT |= BUTTON;            // ...to pullup resistor
    P1IE |= BUTTON;             // Set P1.3 as interrupt pin

    __bis_SR_register(LPM3_bits | GIE);     // Enable interrupt code
    while(1)
    {}              // By default, do nothing continuously
}

#pragma vector=PORT1_VECTOR   // Define interrupt vector, in this case Port 1
__interrupt void Port_1(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1OUT ^= LED1;            // Toggle P1.6 (LED)
    P1IFG = ~BUTTON;            // Disable interrupt flag to re-enable interrupt
}
