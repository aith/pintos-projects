#ifndef CONDVAR_H
#define CONDVAR_H

#include "threads/lock.h"
#include "threads/semaphore.h"

/* Condition variable */
struct condvar {
  struct list waiters; // List of semaphores
};

void condvar_init(struct condvar *);
/*@a*/
bool condvar_priority_gt(const struct list_elem *a, const struct list_elem *b,
                         void *aux);
/*@e*/
void condvar_wait(struct condvar *, struct lock *);
void condvar_signal(struct condvar *, struct lock *);
void condvar_broadcast(struct condvar *, struct lock *);

#endif /* UCSC CSE130 */
