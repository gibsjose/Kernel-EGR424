#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdlib.h>
#include "inc/lm3s6965.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#define STACK_SIZE (4096)   // Amount of stack space for each thread
#define YIELD 		0x88	//Arbitrary value for yield() to call

//Thread structure
typedef struct {
  int active;       		//Non-zero means thread is allowed to run
  char *stack;      		//Pointer to TOP of stack (highest memory location)
  unsigned registers[40];	//Registers R4-R11, R13, and LR
} threadStruct_t;

//Pointer to function with no parameters and
// no return value...i.e., a user-space thread.
typedef void (*thread_t)(void);

void initThreads(void);
void privToUnpriv(void);

//Yield() function
void yield(void);

#endif //_SCHEDULER_H_
