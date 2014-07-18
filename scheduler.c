#include "scheduler.h"

//#define STACK_SIZE (4096)   // Amount of stack space for each thread
#define STACK_SIZE (1532 + 64)  //include the 64 bytes for exception handling

typedef struct {
  int active;       // non-zero means thread is allowed to run
  char *stack;      // pointer to TOP of stack (highest memory location)
  jmp_buf state;    // saved state for longjmp()
} threadStruct_t;

// thread_t is a pointer to function with no parameters and
// no return value...i.e., a user-space thread.
typedef void (*thread_t)(void);

// These are the external user-space threads. In this program, we create
// the threads statically by placing their function addresses in
// threadTable[]. A more realistic kernel will allow dynamic creation
// and termination of threads.
extern void thread1(void);
extern void thread2(void);
extern void thread3(void);
extern void thread4(void);

static thread_t threadTable[] = {
  thread1,
  thread2,
  thread3,
  thread4
};
#define NUM_THREADS (sizeof(threadTable)/sizeof(threadTable[0]))

// These static global variables are used in scheduler(), in
// the yield() function, and in threadStarter()
static jmp_buf scheduler_buf;   // saves the state of the scheduler
static threadStruct_t threads[NUM_THREADS]; // the thread table
unsigned currThread;    // The currently active thread

// This function is called from within user thread context. It executes
// a jump back to the scheduler. When the scheduler returns here, it acts
// like a standard function return back to the caller of yield().
void yield(void)
{
  if (setjmp(threads[currThread].state) == 0) {
    // yield() called from the thread, jump to scheduler context
    longjmp(scheduler_buf, 1);
  } else {
    // longjmp called from scheduler, return to thread context
    return;
  }
}

void reportFreeStackSpace(unsigned threadID)
{
  //determine the number of bytes starting from the bottom of the stack that still have 0xFF in them
  //The thread's stack should be balanced at this stage, so the .stack value will be what malloc originally returned.
  char * lPtr = threads[threadID].stack - STACK_SIZE;
  while(*lPtr++ == 0xFF);
  //now lPtr points to the location one byte past the last untouched memory location, so move it back down
  lPtr--;

  iprintf("Thread %u:\r\n", threadID);
  iprintf("\tStart Address: %x\r\n", (unsigned)threads[threadID].stack);                      //top of stack
  iprintf("\tStop Address: %x\r\n", (unsigned)lPtr);                                          //last used location on the stack
  iprintf("\tBytes used: %u\r\n", (unsigned) (threads[threadID].stack - lPtr) );              //number of bytes used on the stack
  iprintf("\tBytes free: %u\r\n", STACK_SIZE - (unsigned)(threads[threadID].stack - lPtr) );  //number of bytes left over on the stack
}

// This is the starting point for all threads. It runs in user thread
// context using the thread-specific stack. The address of this function
// is saved by createThread() in the LR field of the jump buffer so that
// the first time the scheduler() does a longjmp() to the thread, we
// start here.
void threadStarter(void)
{
  // Call the entry point for this thread. The next line returns
  // only when the thread exits.
  (*(threadTable[currThread]))();

  // Do thread-specific cleanup tasks. Currently, this just means marking
  // the thread as inactive. Do NOT free the stack here because we're
  // still using it! Remember, this function runs in user thread context.
  threads[currThread].active = 0;

  // This yield returns to the scheduler and never returns back since
  // the scheduler identifies the thread as inactive.
  yield();
}

// This function is implemented in assembly language. It sets up the
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
        reportFreeStackSpace(currThread);
        free(threads[currThread].stack - STACK_SIZE);
      }
    }
  } while (1);
}