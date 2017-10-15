# Introduction
This project consisted of writing a program in C to control an MSP430 microprocessor that allowed the control of an onboard LED's blink frequency with an onboard button. The LED is initialized to blink at 10 kHz, and the button controls the LED's blink frequency by tracking how long the button is depressed.

# Basic Functionality
The introduction to the main() initializes the pin inputs and outputs, as well as initializes the timer A control register and capture/compare control register. TA0CCR0 can be set variably, and the function freqCalc() was written to calculate the value of TA0CCR0 for a given desired blink frequency. In this case, the desired initial frequency is 10 kHz. 

# Timer A0 Interrupt
The Timer A0 interrupt simply toggles the LED output whenever it is fired, i.e. when the Timer A counter register reaches the value stored in TA0CCR0.

# Button Interrupt
The Port 1 interrupt has two functionalities, one for when the button is depressed, and one for when the button is released. 
* When the button is depressed, the interrupt vector activates Timer A1, whose sole duty is to track how long the button has been depressed. After activating the timer, the interrupt then sets the interrupt condition for the Port 1 interrupt vector to be the release of the button.
* When the button is released, the interrupt vector halts both Timer A1 and Timer A0, then resets Timer A0. Then, the interrupt assigns TA0CCR0 a new value corresponding to the value held in the Timer A1 counter register, indicating the length that the button was depressed. Timer A0 is then restarted, and the LED output resumes with the new frequency. The Timer A1 counter register is reset, and the interrupt condition for the Port 1 interrupt vector is set to once again be when the button is depressed.
After either of these conditions, the interrupt flag for the Port 1 interrupt vector is reset.

# Frequency Calculator
This function accepts an unsigned integer "desired", representing a desired blink frequency, and outputs an unsigned integer representing the value to be stored in TA0CCR0. The calculation is simply the frequency of the SMCLK (1,048,576 cycles/second) divided by 8 (as Timer A uses the /8 divider), divided by the desired frequency multiplied by 2 (to account for the toggling frequency of the desired blink frequency).

