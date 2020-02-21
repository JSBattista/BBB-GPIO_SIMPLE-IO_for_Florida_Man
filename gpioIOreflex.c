// GPIO Reflex Demo
// Demo to show the use of one GPIO pin for input and another for output as a result of that input.
// Output wave does not imply data, just a means.
// 
// compile and link:
// gcc -o reflex gpioIOreflex.c
// run
// sudo ./reflex
#define GPIO_BASE__P8   0x44e10800
#define GPIO_BASE__P9   0x44e10000
#define GPIO_BASE       0x44e10800
#define BASEADDR_GPIO0 0x44E07000
#define BASEADDR_GPIO1 0x4804C000
#define BASEADDR_GPIO2 0x481AC000
#define BASEADDR_GPIO3 0x481AE000  
#define GPIO_SIZE  0x00000FFF      
#define GPIO_OE 0x14d
#define GPIO_IN 0x14e
#define GPIO_OUT 0x14f
#define USR__IN (1<<12)  // Actual GPIO Name and function in Mode 7. BBB Pin 8_12
#define USR__OUT (1<<13) // BBB pin 8_13
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
int mem_fd;
char *gpio_mem, *gpio_map;
// I/O access
volatile unsigned *gpio;
static void io_setup(void) {
    /* open /dev/mem */
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
            printf("can't open /dev/mem \n");
            exit (-1);
    }  // if
    // mmap GPIO 
    gpio_map = (char *)mmap(
            0,
            GPIO_SIZE,
            PROT_READ|PROT_WRITE,
            MAP_SHARED,
            mem_fd,
            BASEADDR_GPIO1     //  don't drop the base - needs to change depending on what GPIO pin is in use
    );
    close(mem_fd);		// No need for this any more
    if (gpio_map == MAP_FAILED) {
            printf("mmap error %d\n", (int)gpio_map);
            exit (-1);
    }  // if
    gpio = (volatile unsigned *)gpio_map;  // Assign to GPIO handle
    *(gpio + GPIO_OE) |= (USR__IN);  // Register specific input pin in the bank. Note: input may still work without this command. 
    *(gpio + GPIO_OE) &= ~(USR__OUT); // Register output. Entire bank can be "set" this way, but that ruins input. 
}
int main(int argc, char **argv) {
    io_setup();
    while (1) {  // *** infinite loop will have to CTRL-C out of this
	if(((*(gpio + GPIO_IN) & USR__IN) &  (1 << 12)) >> 12) {     // Note the shifting out of registers here for input and the relation to this being gpio 1_12
		for (int incr = 0; incr < 100; incr++) {  // Do a square wave for a while
	        	*(gpio + GPIO_OUT) = *(gpio + GPIO_OUT) | USR__OUT;  // Set output of GPIO high
			usleep(10000);
        		*(gpio + GPIO_OUT) = *(gpio + GPIO_OUT) & (~USR__OUT); // Set output of GPIO low
			usleep(10000);
		}  // for 
	} // if
    }  // while
}
