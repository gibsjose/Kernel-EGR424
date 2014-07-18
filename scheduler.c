#include "scheduler.h"

//External user-space threads
extern void thread_UART(void);
extern void thread_OLED(void);
extern void thread_LED(void);

//SVC Code handler
void handleSVC(int code);

// Thread Table
static threadStruct_t threads[NUM_THREADS];

//Currently Active Thread
unsigned currThread;

//Thread Function Table
static thread_t threadTable[] = {
  thread_UART,
  thread_OLED,
  thread_LED
};

#define NUM_THREADS (sizeof(threadTable)/sizeof(threadTable[0]))

//The SVC Handler interprets the arguments for SVC Calls
// so that user threads can properly yield() by generating
// a SYSTick interrupt, which requires privileged access
void SVCHandler(void)
{
  asm volatile ("ldr r0, [r13, #24]\n"
                "sub r0, r0, #2\n"
                "ldrb r0, [r0]\n"
                "b handleSVC\n"
    );
}

//Generates a SysTick Interrupt
void generateSysTickInterrupt(void)
{
  PENDSTSET |= 0x01;
}

//Changes from privileged to unprivileged
void privToUnpriv(void)
{
  asm volatile( "mrs r3, control\n"
                "orr r3, r3, #1\n"
                "msr control, r3\n"
                "isb"
    );
}

//Changes from unprivileged to privileged
void unprivToPriv(void)
{
  asm volatile( "mrs r3, control\n"
                "and r3, r3, 0xfe\n"
                "msr control, r3\n"
                "isb"
    );
}

//Obtains the current privilege level
int getPriv(void)
{
  asm volatile( "mrs r3, control\n"
                "and r0, r3, #1\n"
                "bx lr"
    );
}

//Handles SVC Codes: yield() will raise an SVC Exception
// so that it can generate a SysTick Interrupt, but it must be in
// privileged mode to do so, thus the SVC architecture
void handleSVC(int code)
{
  if(code == YIELD)
  {
    generateSysTickInterrupt();
  }
}

//Scheduler (SYSTick 1ms Handler)
void Scheduler(void)
{

}

//Scheduler Initialization
// For each thread:
//  - Marks as active
//  - Allocates the Process Stack
//  - Calls the createThread() function to create the thread
void initScheduler(void)
{
  // Create all the threads and allocate a stack for each one
  for (int i = 0; i < NUM_THREADS; i++) 
  {
    //Mark thread as runnable
    threads[i].active = 1;

    //Allocate stack
    threads[i].stack = (char *)malloc(STACK_SIZE) + STACK_SIZE;
    if (threads[i].stack == 0) {
      iprintf("Out of memory\r\n");
      exit(1);
    }

    //Create each thread
    createThread(threads[i].state, threads[i].stack);
  }
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

//This function is implemented in assembly language. It sets up the
// initial jump-buffer (as would setjmp()) but with our own values
// for the stack (passed to createThread()) and LR (always set to
// threadStarter() for each thread).
extern void createThread(jmp_buf buf, char *stack);

// This is the "main loop" of the program.
void scheduler(void)
{
  unsigned i;

  currThread = -1;
  
  do {
    // It's kinda inefficient to call setjmp() every time through this
    // loop, huh? I'm sure your code will be better.
    if (setjmp(scheduler_buf)==0) {

      // We saved the state of the scheduler, now find the next
      // runnable thread in round-robin fashion. The 'i' variable
      // keeps track of how many runnable threads there are. If we
      // make a pass through threads[] and all threads are inactive,
      // then 'i' will become 0 and we can exit the entire program.
      i = NUM_THREADS;
      do {
        // Round-robin scheduler
        if (++currThread == NUM_THREADS) {
          currThread = 0;
        }

        if (threads[currThread].active) {
          longjmp(threads[currThread].state, 1);
        } else {
          i--;
        }
      } while (i > 0);

      // No active threads left. Leave the scheduler, hence the program.
      return;

    } else {
      // yield() returns here. Did the thread that just yielded to us exit? If
      // so, clean up its entry in the thread table.

      if (! threads[currThread].active) {
        free(threads[currThread].stack - STACK_SIZE);
      }
    }
  } while (1);
}