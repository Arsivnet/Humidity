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

// Enable port A clock gate.
    *((volatile unsigned int *)0x40021018) |= (1 << 2);

    // Configure GPIO A pin 9-10 as output. (pins 8-15 use CRH while 0-7 use CRL registers)
    *((volatile unsigned int *)0x40010804) = ((0x44444444 // The reset value
        & ~(0xfU << 4))  // Clear out the bits for pin 9
        |  (0x3U << 4)); // Set both MODE bits, leave CNF at 0
//Set GPIO_BSRR
        *((volatile unsigned int *)0x40010810) = (1U << 9);
    /*
		while (1) {
        // Set the output bit.
        *((volatile unsigned int *)0x40010810) = (1U << 9);
        wait();
        // Reset it again.
        *((volatile unsigned int *)0x40011010) = (1U << 29);
        wait();
    }
*/


}
