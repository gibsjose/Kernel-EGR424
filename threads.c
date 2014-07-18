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
  //for now, just write a simple string to the OLED
  oled_d_clear();
  oled_d_print_xy("Hello, world!", 40, 40);
}

void thread_LED(void)
{
  //for now, just turn on the LED
  LED_TI = 1;
}

void thread_idle()
{
  //this keeps the processor running when there is no work to do.
  while(1);
}