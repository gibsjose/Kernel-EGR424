#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#define STACK_SIZE (4096)   // Amount of stack space for each thread
#define YIELD 		0x88	//Arbitrary value for yield() to call

//Thread structure
typedef struct {
  int active;       // non-zero means thread is allowed to run
  char *stack;      // pointer to TOP of stack (highest memory location)
} threadStruct_t;

//Pointer to function with no parameters and
// no return value...i.e., a user-space thread.
typedef void (*thread_t)(void);

//Yield() function
void yield(void);

#endif //_SCHEDULER_H_
