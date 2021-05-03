#ifndef LOCK_H
#define LOCK_H

#include "threads/semaphore.h"

/* Lock */
struct lock {
  struct thread *holder;      /* Thread holding lock (for debugging) */
  struct semaphore semaphore; /* Binary semaphore controlling access */
  /*@a*/
  struct list_elem list_elem; /* To use when adding to a list */
  int priority;
  /*@e*/
};

void lock_init(struct lock *);

/*@a*/
bool lock_priority_gt(const struct list_elem *a, const struct list_elem *b,
                      void *aux);
void trickle_priority_donation(struct thread *initial_thread, int new_priority);
void give_priority_donation(struct lock *lock);
bool lock_remove_from_list(struct lock *lock, struct list *l);
bool find_lock(struct lock *lock, struct list *l);
void thread_revoke_donated_priority(void);
bool thread_update_donated_priority(void);
/*@e*/

void lock_acquire(struct lock *);
void lock_release(struct lock *);
bool lock_held_by_current_thread(const struct lock *);

#endif /* UCSC CSE130 */
