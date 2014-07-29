#include <stdio.h>
#include "scheduler.h"
#include "oled_driver.h"
#include "lock.h"

#define LED_TI (HWREG(0x40025000 + 0x004))

// currThread indicates the number of the thread currently running.
extern unsigned currThread;

// This is the lock variable used by the UART threads to "lock" the peripheral.
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
      yield();
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
      }
      lock_release(&UARTthreadlock, currThread);
      yield();
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