#include <msp430.h>
#define LED1 BIT0;
#define LED2 BIT0;
#define BUTTON BIT1;
unsigned int freqCalc(unsigned int number);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    P1DIR |= LED1;   // Set P1.0 and P4.7 to output
    P2DIR |= LED2;
    P1OUT &= ~LED1;  // Initialize LEDs to off
    P2OUT &= ~LED2;
    P1DIR &= ~BUTTON;            // Set P1.3 to input
    P1REN |= BUTTON;            // Set P1.3 resistor enable...
    P1OUT |= BUTTON;            // ...to enable pullup resistor
    P1IE |= BUTTON;             // Set P1.3 to be interrupt
    P1IES |= BUTTON;            // Set P1.3 to falling edge interrupt
    TB0CTL = TBSSEL_2 + MC_1 + ID_3 + TBCLR; // Use SMCLK in up mode

    static const unsigned int desired = 10000;          // Set desired frequency, f <= 16000
    TB0CCR0 = freqCalc(desired); // counts = SMCLK / (2*desired)
    TB0CCTL0 = CCIE;          // Enable capture/compare interrupt
    TB0CCTL0 &= 0x10;          // Set to compare mode

    TB1CTL = TBSSEL_2 + MC_0 + ID_3 + TBCLR; // Use SMCLK in continuous mode, divide 8
    __enable_interrupt();     // Enable interrupt code
    while(1)
    {}
}

#pragma vector=TIMER0_B0_VECTOR  // Define interrupt vector, in this case Timer0_A0
__interrupt void led_blinker(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1OUT ^= LED1;   // Toggle LEDs
    P2OUT ^= LED2;
}

#pragma vector=PORT1_VECTOR   // Define interrupt vector, in this case Port 1
__interrupt void button_setter(void) // Define interrupt function that runs whenever interrupt is detected
{

    if (!(P1IN & BIT1))
    {

        TB1CTL = TBSSEL_2 + MC_2 + ID_3 + TBCLR; // Starts timer in continuous mode
        P1IES &= ~BIT1; // Set P1.3 to rising edge interrupt
    }
    else
    {
        TB1CTL = MC_0; // Halt timer 1
        TB0CTL = MC_0; // Halt timer 0
        TB0R = 0;      // Set TA0R to 0
        TB0CCR0 = TB1R; // Pass timer 1 value to TA0CCR0 for timer 0 CCR0
        TB0CTL = TBSSEL_2 + MC_1 + ID_3 + TBCLR; // Restart timer 0 in up mode
        TB1R = 0; // Set TA1R to 0

        P1IES |= BIT1; // Set P1.3 to falling edge interrupt
    }
    P1IFG &= ~BIT1; // Reset interrupt flag
}

unsigned int freqCalc(unsigned int desired)
{
    return 65536 / desired;
}
