#ifndef LOCK_H_
#define LOCK_H_

typedef struct lock_t {
  unsigned lock;	//this must be first in the structure because the lock_acquire assembly code depends on that
  unsigned count;
  int threadID;
} lock_t;

void lock_init(lock_t *lock);
unsigned lock_acquire(lock_t *lock, int aThreadID);
void lock_release(lock_t *lock, int aThreadID);

#endif