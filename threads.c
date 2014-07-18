#ifndef THREADS_H_
#define THREADS_H_

#include <stdio.h>
#include "scheduler.h"

// These are the user-space threads. Note that they are completely oblivious
// to the technical concerns of the scheduler. The only interface to the
// scheduler is the single function yield() and the global variable
// currThread which indicates the number of the thread currently
// running.

static unsigned _Thread_UART_notify = 0;
static unsigned _Thread_OLED_notify = 0;
static unsigned _Thread_LED_notify = 0;

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

}

void thread_LED(void)
{
  unsigned count;

  for (count=0; count < 3; count++) {
    iprintf("In thread %u -- pass %d\r\n", currThread, count);
    yield();
  }
}

void thread_idle()
{
  //this keeps the processor running when there is no work to do.
  while(1);
}

#endif