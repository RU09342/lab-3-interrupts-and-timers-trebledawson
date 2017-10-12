#include <msp430.h>
#define LED1 BIT0;
#define LED2 BIT1;
#define BUTTON BIT5;
unsigned int freqCalc(unsigned int number);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                               // to activate previously configured port settings
    P1DIR |= LED1;   // Set P1.0 and P4.7 to output
    P1DIR |= LED2;
    P1OUT &= ~LED1;  // Initialize LEDs to off
    P1OUT &= ~LED2;
    P5DIR &= ~BUTTON;            // Set P1.3 to input
    P5REN |= BUTTON;            // Set P1.3 resistor enable...
    P5OUT |= BUTTON;            // ...to enable pullup resistor
    P5IE |= BUTTON;             // Set P1.3 to be interrupt
    P5IES |= BUTTON;            // Set P1.3 to falling edge interrupt
    TA0CTL = TASSEL_2 + MC_1 + ID_3 + TACLR; // Use SMCLK in up mode

    static const unsigned int desired = 10000;          // Set desired frequency, f <= 16000
    TA0CCR0 = freqCalc(desired); // counts = SMCLK / (2*desired)
    TA0CCTL0 = CCIE;          // Enable capture/compare interrupt
    TA0CCTL0 &= 0x10;          // Set to compare mode

    TA1CTL = TASSEL_2 + MC_0 + ID_3 + TACLR; // Use SMCLK in continuous mode, divide 8
    __enable_interrupt();     // Enable interrupt code
    while(1)
    {}
}

#pragma vector=TIMER0_A0_VECTOR  // Define interrupt vector, in this case Timer0_A0
__interrupt void led_blinker(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1OUT ^= LED1;   // Toggle LEDs
    P1OUT ^= LED2;
}

#pragma vector=PORT5_VECTOR   // Define interrupt vector, in this case Port 1
__interrupt void button_setter(void) // Define interrupt function that runs whenever interrupt is detected
{

    if (!(P5IN & BIT5))
    {

        TA1CTL = TASSEL_2 + MC_2 + ID_3 + TACLR; // Starts timer in continuous mode
        P5IES &= ~BIT5; // Set P1.3 to rising edge interrupt
    }
    else
    {
        TA1CTL = MC_0; // Halt timer 1
        TA0CTL = MC_0; // Halt timer 0
        TA0R = 0;      // Set TA0R to 0
        TA0CCR0 = TA1R; // Pass timer 1 value to TA0CCR0 for timer 0 CCR0
        TA0CTL = TASSEL_2 + MC_1 + ID_3 + TACLR; // Restart timer 0 in up mode
        TA1R = 0; // Set TA1R to 0

        P5IES |= BIT5; // Set P1.3 to falling edge interrupt
    }
    P5IFG &= ~BIT5; // Reset interrupt flag
}

unsigned int freqCalc(unsigned int desired)
{
    return 65536 / desired;
}
