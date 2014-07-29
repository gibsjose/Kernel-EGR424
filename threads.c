#include <stdio.h>
#include "scheduler.h"
#include "oled_driver.h"
#include "lock.h"

#define LED_TI (HWREG(0x40025000 + 0x004))

// These are the user-space threads. Note that they are completely oblivious
// to the technical concerns of the scheduler. The only interface to the
// scheduler is the single function yield() and the global variable
// currThread which indicates the number of the thread currently
// running.
extern unsigned currThread;

// This is the lock variable used by all threads.
extern lock_t UARTthreadlock;

void thread_UART1(void)
{
  unsigned count;
  volatile long i;

  while(1)
  {
    if(lock_acquire(&UARTthreadlock, currThread))
    {
      for (count = 0; count < 3; count++) {
        for(i = 0; i < 100000; i++);
        iprintf("In UART thread %d -- pass %d\r\n", currThread, count);
        yield();
      }
      lock_release(&UARTthreadlock, currThread);
      break;  //end the while loop, and thus the thread
    }
  }
}

void thread_UART2(void)
{
  unsigned count;
  volatile long i;
  
  while(1)
  {
    if(lock_acquire(&UARTthreadlock, currThread))
    {
      for (count = 0; count < 5; count++) {
        for(i = 0; i < 100000; i++);
        iprintf("In UART thread %d -- pass %d\r\n", currThread, count);
        yield();
      }
      lock_release(&UARTthreadlock, currThread);
      break;  //end the while loop, and thus the thread
    }
  }
}

void thread_OLED(void)
{
  while(1)
  {
    volatile long i;

    oled_d_clear();
    for(i = 0; i < 100000; i++);
    oled_d_print_xy("Hello, world!", 40, 40);
	  for(i = 0; i < 100000; i++);
    
    yield();
  }
}

void thread_LED(void)
{
  while(1)
  {
    volatile unsigned long i;
    for(i = 0; i < 100000; i++);
    LED_TI ^= 1;  //toggle the LED
  }
}