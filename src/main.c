extern unsigned long _end_of_ram;


void main();

__attribute__ ((section(".interrupt_vectors"), used))

void (* const interrupt_vectors[])(void) = {



(void (*)(void))((unsigned long)&_end_of_ram),

main

};

void wait(){

	for (unsigned int i = 0; i < 2000000; ++i) __asm__ volatile ("nop");

}

void main() {

// Enable port A clock gate and ADC1
    *((volatile unsigned int *)0x40021018) |= (1U << 2);
    *((volatile unsigned int *)0x40021018) |= (1U << 9);
	
	//Turn on ADC1
	*( (volatile unsigned int *)0x40012408) |= (1U << 0);
	
	//We start ADC1 Callibration to better read the analog voltage.
	*( (volatile unsigned int *)0x40012408) |= (1U << 2);
    //wait until calibration is complete
    while ( *( (volatile unsigned int *)0x40012408) == 5U )
    {
        /* code */
    }

/*Set SWstart as the external trigger, enable external triggers 
*(We're using single conversion mode so alternatively, we can just use ADON bit to start conversion and skip this.)
*/
    *( (volatile unsigned int *)0x40012408) =( 0x00000000  
		| (7U << 17)
		| (1U << 20)
		
		);
//Enable discontinous mode, specify the number of channels to be converted (1)
	*( (volatile unsigned int *)0x40012404) = ((0x00000000
		& ~(7U << 13))
		| (1U << 11)
		);

//Set channel 0 as the first conversion in regular sequence
	*( (volatile unsigned int *)0x40012434) |= (0U << 0);
    

    // Configure GPIO A pin 9-10 as output. (pins 8-15 use CRH while 0-7 use CRL registers)
    *(( volatile unsigned int *)0x40010804) = ((0x44444444 // The reset value
        & ~(0xfU << 4)  // Clear out the bits for pin 9
		& ~(0xfU << 8) //same for pin 10
		)
        |  (0x3U << 4)
		| (0x3U << 8)
		); // Set both MODE bits, leave CNF at 0
		
	/*Configure GPIO A pin 0 to be an analog (CNF=00) input ( Mode=00)
	 * Detail: In total we need 4 bits to be zero. So we take 4 bits as 1111 (Which is 15 in decimal and F in hexadecimal),
	 * reverse it's value so we have 0000, and we
	 * logical shift this value to the starting position of our desired pin in the register. In this case it's 0.
	 */
	*( (volatile unsigned int *)0x40010800) = (0x44444444 & ~(0xfU << 0);

//Set GPIO_BSRR of port A
        *((volatile unsigned int *)0x40010810) = (1U << 9);
        *((volatile unsigned int *)0x40010810) = (1U << 10);

		while (1) {
		//reset conversion start bit
	*( (volatile unsigned int *)0x40012408) |= (0U << 22);
        // Set the output bit for pin 9 and reset it for 10.
        *((volatile unsigned int *)0x40010810) = (1U << 9);
        *((volatile unsigned int *)0x40010810) = (1U << 26);
		// Start ADC conversion with SWstart
	*( (volatile unsigned int *)0x40012408) |= (1U << 22);
        wait();
		//reset conversion start bit
	*( (volatile unsigned int *)0x40012408) |= (0U << 22);
        // Switch
        *((volatile unsigned int *)0x40010810) = (1U << 25);
        *((volatile unsigned int *)0x40010810) = (1U << 10);
		//Start conversion
		*( (volatile unsigned int *)0x40012408) |= (1U << 22);

        wait();
    }



}
