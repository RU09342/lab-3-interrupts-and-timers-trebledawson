# Introduction
This project consisted of writing a program in C to control an MSP430 microprocessor to allow the control of onboard LEDs using the MSP430 built in clocks and timer interrupt vectors. The program was primarily developed on the MSP430G2553, and was subsequently ported to the other development boards.

# Basic Functionality
The introduction to the main() initializes the pin inputs and outputs, as well as initializes the timer A control register and capture/compare control register. TA0CCR0 can be set variably, and the function freqCalc() was written to calculate the value of TA0CCR0 for a given desired blink frequency.

# Interrupt
The interrupt vector TIMER0_A0_VECTOR is the vector specifically assigned to the MSP430 Timer A. When the Timer A interrupt flag is set, the code inside this interrupt vector runs, toggling the LED output.

# Frequency Calculator
This function accepts an unsigned integer "desired", representing a desired blink frequency, and outputs an unsigned integer representing the value to be stored in TA0CCR0. The calculation is simply the frequency of the SMCLK (1,048,576 cycles/second) divided by 8 (as Timer A uses the /8 divider), divided by the desired frequency multiplied by 2 (to account for the toggling frequency of the desired blink frequency).
