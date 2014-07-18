#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#define STACK_SIZE (4096)   // Amount of stack space for each thread
//#define STACK_SIZE (1532 + 64)  //include the 64 bytes for exception handling

typedef struct {
  int active;       // non-zero means thread is allowed to run
  char *stack;      // pointer to TOP of stack (highest memory location)
  jmp_buf state;    // saved state for longjmp()
} threadStruct_t;

// thread_t is a pointer to function with no parameters and
// no return value...i.e., a user-space thread.
typedef void (*thread_t)(void);

extern unsigned currThread;
extern void yield(void);

void scheduler();

#endif // _SCHEDULER_H_
