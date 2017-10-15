# Introduction
This project consisted of writing a program in C to control an MSP430 microprocessor to allow the control of an onboard LED with an onboard button. This program differs from Lab 2: Simple Blink in that the button functionality is controlled using an interrupt vector instead of through constant polling of a delay variable. This allows for more precise control of the pin output. The program was primarily developed on the MSP430G2553, and was subsequently ported to the other development boards.

# Basic Functionality
The introduction to the main() initializes the pin inputs and output. A crucial addition to this code is the __enable_interrupts() line, which allows interrupt vectors to be addressed when the appropriate interrupt flag has been set. After initializing the pins, the main() simply idles in an empty while loop until the interrupt vector is called.

# Interrupt
The interrupt vector PORT1_VECTOR is the interrupt vector specifically assigned to the Port 1 pins on the MSP430. When the Port 1 interrupt flag is set, the code inside this interrupt function is run. The first line toggles the LED output pin. The second line resets the interrupt flag so that after the code exits the interrupt vector, a subsequent press of the button can set the interrupt flag again, allowing the interrupt code to be run again.
