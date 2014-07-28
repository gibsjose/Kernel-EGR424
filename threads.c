#include <stdio.h>
#include "scheduler.h"
#include "oled_driver.h"

#define LED_TI (HWREG(0x40025000 + 0x004))

// These are the user-space threads. Note that they are completely oblivious
// to the technical concerns of the scheduler. The only interface to the
// scheduler is the single function yield() and the global variable
// currThread which indicates the number of the thread currently
// running.
extern unsigned currThread;

void thread_UART(void)
{
  unsigned count;

  for (count = 0; count < 10; count++) {
    iprintf("In UART thread %u -- pass %d\r\n", currThread, count);
    
    yield();
  }
}

void thread_OLED(void)
{
  while(1)
  {
    iprintf("HERE!\r\n");
    //oled_d_clear();
    //unsigned i;
    //for(i = 0; i < 1000000; i++);
    //oled_d_print_xy("Hello, world!", 40, 40);
	  //for(i = 0; i < 1000000; i++);
    yield();
  }
}

void thread_LED(void)
{
  while(1)
  {
    unsigned long i;
    for(i = 0; i < 1000; i++); //murder time (well, kill it, really) //No... actually murder it. 65535 isn't enough, brah
    LED_TI ^= 1;  //toggle the LED
  }
}