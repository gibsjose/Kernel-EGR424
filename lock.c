#include "lock.h"

void lock_init(lock_t *lock)
{
  lock->lock = 1;
  lock->count = 0;
  lock->threadID = -1;
}

//increment lock
unsigned lock_acquire(lock_t *lock, int aThreadID)
{
  if(lock->threadID == -1)
  {
    asm volatile("PUSH {r0-r2}\n"
      "MOV      R1, #0\n"
      "LDREX    R2, [R0]   @ R2 <- lock value\n"
      "CMP      R2, R1   @ Is it already 0? (hence locked?)\n"
      "ITT      NE\n"
      "STREXNE  R2, R1, [R0] @ If not, try to claim it by writing 0\n"
      "        @ R2 <- 0 if successful, 1 if failure\n"
      "CMPNE    R2, #1   @ and check success\n"
      "BEQ      1f     @ Branch taken if lock was already 0\n"
      "        @ (so the previous two xxxxNE instructions\n"
      "    @ did not execute) or STREXNE returned 1\n"
      "POP {r0-r2}");
    
    lock->threadID = aThreadID; //mark the lock with the thread ID of the caller
    lock->count = 1;  //this is the first lock count

    asm volatile(
      "MOV      R0, #1   @ Indicate success\n"
      "BX       LR\n"
      "\n"
  "1:    @ Local label... branch here from above with desination '1f'\n"
      "CLREX     @ We did not get the lock. Clear exclusive access\n"
      "MOV      R0, #0   @ Indicate failure\n"
      "BX       LR");
  }
  else if(aThreadID == lock->threadID)
  {
    lock->count++;  //increment the lock count
  }
  else
  {
    return 0; //indicate failure since the lock was assigned to a thread other than the calling one
  }
  
  return 0;   //satisfy the compiler even though this line will never be reached because the assembly code returns
}

//decrement lock and clear it if this was the last release
void lock_release(lock_t *lock, int aThreadID)
{
  if(aThreadID == lock->threadID) //check if it is the same thread that originally got the lock that is trying to unlock it
  {
    lock->count--;  //decrement the count
    if(lock->count == 0)  //if all locks are released...
    {
      lock->lock = 1; //completely release the lock
      lock->threadID = -1;  //reset the threadID so another thread can lock this lock variable
    }
  }
}
