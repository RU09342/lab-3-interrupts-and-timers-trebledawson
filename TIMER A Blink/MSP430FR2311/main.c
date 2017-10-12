#include <msp430.h>
#define LED1 BIT0
#define LED2 BIT0

unsigned int freqCalc(unsigned int number);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= LED1;   // Set P1.0 to output LED
    P2DIR |= LED2;   // Set P4.7 to output LED
    P1OUT &= ~LED1; // Initialize LEDs to off
    P2OUT &= !LED2;  // Initialize LEDs to off
    TB0CTL = TBSSEL_2 + MC_1 + ID_3 + TBCLR; // Use SMCLK in up mode, /8 divider
    unsigned int desired = 16;          // Set desired frequency, f <= 16000
    TB0CCR0 = freqCalc(desired); // counts = ACLK / (2*desired)

    TB0CCTL0 = CCIE;          // Enable capture/compare interrupt
    TB0CCTL0 &= 0x10;          // Set to compare mode
    __enable_interrupt();     // Enable interrupt code
    while(1)
    {} // Run

}

#pragma vector=TIMER0_B0_VECTOR  // Define interrupt vector, in this case Port 1
__interrupt void Port_1(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1OUT ^= LED1;   // Toggle LEDs
    P2OUT ^= LED2;   // Toggle LEDs
}

unsigned int freqCalc(unsigned int desired)
{
    return 65536 / desired;
}
