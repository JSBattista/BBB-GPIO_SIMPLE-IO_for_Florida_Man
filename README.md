# BBB-GPIO_SIMPLE-IO_for_Florida_Man
GPIO in and out sample so simple even Florida Man can do it. 
There are times when you need a simple solution and don't have time for anything else. And if this is one of them, then you don't even need to be from Florida. 
Generally, the situation of "I have a GPIO pin I want to put something into, and another I want to get something out of" is often muddled with the usual inclusion of DTS and DTB and overlays and flat files and modes and pinmux and all that other sort of stuff that gets between us and the in-out solution like a level boss gets in the way of the treasure. 
And while it's great to see other fellows write these wonderful C++ libraries and such that do the job, sometimes we don't need that. I like my program like my synth music: minimal. 
So the goal here is to clear that up, plain in simple, with plain and simple C code. 
First, credit where credit is due. The core of this solution comes from a poster madscientist159 at Stack Overflow. See the link: https://stackoverflow.com/questions/13124271/driving-beaglebone-gpio-through-dev-mem#20446907
Note the output sample with memory map. That's nice for flashing the LEDs. But I wanted input too. So after some shift and register warfare I got that to work. 
The other part comes from a nice C++ demo. I use none of that here, but the way that code sample works shows some good memory mapping "tactics" that helped me around the sample I wanted to create.  Find it here at https://github.com/facine/easyBlack At its core, it's using memory mapped access and shows some of the relationships around addresses, offsets, and the like. 
And yes, memory map. We're going to use the big tools and go fast and lean with memory map. The result is some rather short code that does one thing. I include other files demonstrating inputs on other banks of GPIO pins - there are 4 of them BTW. Some more useful than others. 
Another reference: See Molloy's charts. They are everywhere so I won't link them here. They show you what pin number correlates to what GPIO number to what bank ID. It's not as confusing as you think.
For example, GPIO 1_12 in the code is recognized as (1<<12) on declaration. But we have to shift right that same designation on input. Don't forget that. The code attempt is as short and simple as possible but for more robust and readable code I would hold data in a structure to keep the concepts together. If you change the GPIO bank/pin of the input you still have to shift right the actual input to get that 1 or 0 you need. It will be obious in the code. 
The sample shows an input, and an output. One of the most important lines of the code is the mmap command. Make sure your selected GPIO is represented by the proper base. So
#define USR__IN (1<<12)  // Actual GPIO Name and function in Mode 7. BBB Pin 8_12
#define USR__OUT (1<<13) // BBB pin 8_13
means we are using
gpio_map = (char *)mmap(
            0,
            GPIO_SIZE,
            PROT_READ|PROT_WRITE,
            MAP_SHARED,
            mem_fd,
            BASEADDR_GPIO1     //  don't drop the base - needs to change depending on what GPIO pin is in use
    );
If we were using 
#define USR__IN1 (2<<1)  // GPIOX<<Y where X is bank and y is number of that bank in Mode 7
#define USR__IN2 (2<<4)
Then we would use 
  BASEADDR_GPIO2
You will also notice that I have a lot of unused addresses in the code. This is for reference to help get used to the idea that when dealing with memory maps, it's a world of addresses... and segfaults. 
