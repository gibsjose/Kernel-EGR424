#include <stdio.h>
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "scheduler.h"
#include "lock.h"

//SVC Code handler
//void handleSVC(int code);

lock_t UARTthreadlock;

//Currently Active Thread
int currThread = -1;

volatile unsigned needToSaveThreadStates = 0;

//External user-space threads
extern void thread_UART1(void);
extern void thread_UART2(void);
extern void thread_OLED(void);
extern void thread_LED(void);

//Thread Function Table
static thread_t threadTable[] = {
  thread_UART1,
  thread_UART2,
  thread_OLED,
  thread_LED
};

#define NUM_THREADS (sizeof(threadTable)/sizeof(threadTable[0]))

// Thread Table
static threadStruct_t threads[NUM_THREADS];

//@TODO Add header...
extern void createThread(unsigned *p_registers, char **p_stack);

extern void saveThreadState(unsigned *p_registers);
extern void restoreThreadState(unsigned *p_registers);

//Scheduler (SysTick 1ms Handler)
void Scheduler(void)
{
  //Save current thread state
  if(currThread != -1)
    saveThreadState(threads[currThread].registers);

  do
  {
    if (++currThread >= NUM_THREADS) {
      currThread = 0;
    }
  } while (threads[currThread].active != 1);

  //Restore the thread state for the thread about to be executed
  restoreThreadState(threads[currThread].registers);
}

//Thread Initialization
// For each thread:
//  - Marks as active
//  - Allocates the Process Stack
//  - Calls the createThread() function to create the thread
void initThreads(void)
{
  unsigned i;

  // Create all the threads and allocate a stack for each one
  for (i = 0; i < NUM_THREADS; i++) 
  {
    //Mark thread as runnable
    threads[i].active = 1;

    //Allocate stack
    threads[i].stack = (char *)malloc(STACK_SIZE) + STACK_SIZE;

    if (threads[i].stack == 0) {
      //out of memory
      exit(1);
    }

    //Create each thread
    createThread(threads[i].registers, &(threads[i].stack));
  }

  //initialize the global lock variable for the UART threads
  lock_init(&UARTthreadlock);
}

//This function is called from within user thread context. It allows the user
// threads to give up their time slot and returns the context to the Scheduler
void yield(void)
{
  //Generate an SVC Exception with YIELD Code (0x88)
  asm volatile ("svc #0x88");
}

//This is the starting point for all threads. It runs in user thread
// context using the thread-specific stack. The address of this function
// is saved by createThread() in the LR field of the jump buffer so that
// the first time the scheduler() does a longjmp() to the thread, we
// start here.
void threadStarter(void)
{
  //Call the entry point for this thread. The next line returns
  // only when the thread exits.
  (*(threadTable[currThread]))();

  //Do thread-specific cleanup tasks. Currently, this just means marking
  // the thread as inactive. Do NOT free the stack here because we're
  // still using it! Remember, this function runs in user thread context.
  threads[currThread].active = 0;

  //This yield returns to the scheduler and never returns back since
  // the scheduler identifies the thread as inactive.
  yield();
}

//The SVC Handler interprets the arguments for SVC Calls
// so that user threads can properly yield() by generating
// a SYSTick interrupt, which requires privileged access
/*
void SVCHandler(void)
{
  asm volatile ("ldr r0, [r13, #24]\n"
                "sub r0, r0, #2\n"
                "ldrb r0, [r0]\n"
                "b handleSVC\n"
    );
}
*/

//Generates a SysTick Interrupt
void generateSysTickInterrupt(void)
{
  NVIC_INT_CTRL_R |= NVIC_INT_CTRL_PENDSTSET;
}

//Handles SVC Codes: yield() will raise an SVC Exception
// so that it can generate a SysTick Interrupt, but it must be in
// privileged mode to do so, thus the SVC architecture
/*
void handleSVC(int code)
{
  if(code == YIELD)
  {
    generateSysTickInterrupt();
  }
}
*/