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
    *((volatile unsigned int *)0x40021018) |= (1 << 2);
    *((volatile unsigned int *)0x40021018) |= (1 << 9);
	
	//We start ADC1 Callibration to better read the analog voltage.
	*( (volatile unsigned int *)0x40012408) |= (1 << 2);

    // Configure GPIO A pin 9-10 as output. (pins 8-15 use CRH while 0-7 use CRL registers)
    *(( volatile unsigned int *)0x40010804) = ((0x44444444 // The reset value
        & ~(0xfU << 4)  // Clear out the bits for pin 9
		& ~(0xfU << 8) //same for pin 10
		& ~(0xfU << 12)) //same for pin 11 but we don't set it because input mode requires MODE bits to be set to 00 and CNF to be 00 for analog mode

        |  (0x3U << 4)
		| (0x3U << 8)
		); // Set both MODE bits, leave CNF at 0

//Set GPIO_BSRR of port A
        *((volatile unsigned int *)0x40010810) = (1U << 9);
        *((volatile unsigned int *)0x40010810) = (1U << 10);

		while (1) {
        // Set the output bit for pin 9 and reset it for 10.
        *((volatile unsigned int *)0x40010810) = (1U << 9);
        *((volatile unsigned int *)0x40010810) = (1U << 26);
        wait();
        // Switch
        *((volatile unsigned int *)0x40010810) = (1U << 25);
        *((volatile unsigned int *)0x40010810) = (1U << 10);
        wait();
    }



}
