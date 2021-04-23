#ifndef LOCK_H
#define LOCK_H

#include "threads/semaphore.h"

/* Lock */
struct lock {
  struct thread *holder;      /* Thread holding lock (for debugging) */
  struct semaphore semaphore; /* Binary semaphore controlling access */
  /*@a*/
  /*@e*/
};

void lock_init(struct lock *);

/*@a*/
void trickle_priority_donation(struct thread *initial_thread, int new_priority);
void give_priority_donation(struct lock *lock);
/*@e*/

void lock_acquire(struct lock *);
void lock_release(struct lock *);
bool lock_held_by_current_thread(const struct lock *);

#endif /* UCSC CSE130 */
