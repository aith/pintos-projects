/*
 * This file is derived from source code for the Pintos
 * instructional operating system which is itself derived
 * from the Nachos instructional operating system. The
 * Nachos copyright notice is reproduced in full below.
 *
 * Copyright (C) 1992-1996 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose, without fee, and
 * without written agreement is hereby granted, provided that the
 * above copyright notice and the following two paragraphs appear
 * in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
 * AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 * BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 * Modifications Copyright (C) 2017-2021 David C. Harrison.
 * All rights reserved.
 */

#include <stdio.h>
#include <string.h>

#include "threads/interrupt.h"
#include "threads/lock.h"
#include "threads/thread.h"

/*
 * Initializes LOCK.  A lock can be held by at most a single
 * thread at any given time.  Our locks are not "recursive", that
 * is, it is an error for the thread currently holding a lock to
 * try to acquire that lock.
 *
 * A lock is a specialization of a semaphore with an initial
 * value of 1.  The difference between a lock and such a
 * semaphore is twofold.  First, a semaphore can have a value
 * greater than 1, but a lock can only be owned by a single
 * thread at a time.  Second, a semaphore does not have an owner,
 * meaning that one thread can "down" the semaphore and then
 * another one "up" it, but with a lock the same thread must both
 * acquire and release it.  When these restrictions prove
 * onerous, it's a good sign that a semaphore should be used,
 * instead of a lock.
 */
void lock_init(struct lock *lock) {
  ASSERT(lock != NULL);

  lock->holder = NULL;
  lock->priority = -1;
  semaphore_init(&lock->semaphore, 1);
}

/*@a*/
bool lock_priority_gt(const struct list_elem *a, const struct list_elem *b,
                      void *aux) {
  struct lock *entry_a = list_entry(a, struct lock, list_elem);
  struct lock *entry_b = list_entry(b, struct lock, list_elem);
  return entry_a->priority > entry_b->priority;
}

/* A recursive function that passes locks' priorities down until the last
lock.
 */
void trickle_priority_donation(struct thread *initial_thread,
                               int new_priority) {
  struct lock *lock_current = initial_thread->lock_waiting_on;
  while (lock_current != NULL) {
    struct thread *thread_current =
        lock_current->holder; // The lock to be given donation
    thread_current->priority = new_priority;
    lock_current = thread_current->lock_waiting_on;
  }
}
/*@e*/

/*@a*/
void give_priority_donation(struct lock *lock) {
  struct thread *receiver = lock->holder;
  if (thread_get_priority() > receiver->priority) {
    receiver->priority = thread_get_priority();
  }
  thread_preempt();
}
/*@e*/

/*
 * Acquires LOCK, sleeping until it becomes available if
 * necessary.  The lock must not already be held by the current
 * thread.
 *
 * This function may sleep, so it must not be called within an
 * interrupt handler.  This function may be called with
 * interrupts disabled, but interrupts will be turned back on if
 * we need to sleep.
 */
void lock_acquire(struct lock *lock) {
  ASSERT(lock != NULL);
  ASSERT(!intr_context());
  ASSERT(!lock_held_by_current_thread(lock));
  /*@a* Donation! /
  /* If the thread attempts to acquire a lock that's held, try to donate
   * the current running thread's priority to the the holder before blocking,
   * demonstrated in https://www.youtube.com/watch?v=dQwiWcHqS_8 and in
   * https://www.youtube.com/watch?v=nVUQ4f1-roM */
  if (lock->holder == NULL) {
    /* Acquire the lock since its untaken */
  } else {
    /* Do donation to get lock */
    struct thread *holder = lock->holder;
    /* Checks if the wanted lock is already one of the holder's donors. */
    if (find_lock(lock, &holder->priority_donor_locks)) {
      /* Reinvoke donation */
      lock->priority = holder->priority; // Donate priority
    } else {
      /* Donate priority to holder from current thread */
      trickle_priority_donation(holder, thread_get_priority());
      thread_current()->lock_waiting_on = lock;
      lock->priority = thread_get_priority(); // Donate priority
      list_insert_ordered(
          &holder->priority_donor_locks,
          &lock->list_elem, // Keep track of the donors, add here
          lock_priority_gt, NULL);
    }
    holder->priority = thread_get_priority(); // Donation!
    thread_preempt();
  }
  semaphore_down(&lock->semaphore);
  lock->holder = thread_current();
  /*@e*/
}

/*@a*/
bool lock_remove_from_list(struct lock *lock, struct list *l) {
  for (struct list_elem *curr_elem = list_begin(l); curr_elem != list_end(l);
       curr_elem = list_next(curr_elem)) {
    struct lock *curr_lock = list_entry(curr_elem, struct lock, list_elem);
    if (lock == curr_lock) {
      list_remove(curr_elem);
      return true;
    }
  }
  return false;
}

/* Searches for lock in list */
bool find_lock(struct lock *lock, struct list *l) {
  bool result = false;
  for (struct list_elem *curr_elem = list_begin(l); curr_elem != list_end(l);
       curr_elem = list_next(curr_elem)) {
    struct lock *donor_lock = list_entry(curr_elem, struct lock, list_elem);
    if (lock == donor_lock) {
      result = true;
      break;
    }
  }
  return result;
}

void thread_revoke_donated_priority(void) {
  thread_current()->priority = thread_current()->base_priority;
}

/* If thread has donors, take the priority of highest one */
bool thread_update_donated_priority(void) {
  if (!list_empty(&thread_current()->priority_donor_locks)) {
    struct lock *highest_priority_lock =
        list_entry(list_front(&thread_current()->priority_donor_locks),
                   struct lock, list_elem);
    thread_current()->priority = highest_priority_lock->priority;
    return true;
  }
  return false;
}
/*@e*/

/*
 * Releases LOCK, which must be owned by the current thread.
 *
 * An interrupt handler cannot acquire a lock, so it does not
 * make sense to try to release a lock within an interrupt
 * handler.
 */
void lock_release(struct lock *lock) {
  ASSERT(lock != NULL);
  ASSERT(lock_held_by_current_thread(lock)); /* need to use this reseting */
  struct thread *prev = lock->holder;
  lock->holder = NULL;
  semaphore_up(&lock->semaphore);
  /*@a*/
  /* If there are donor locks, then give up the donation */
  if (!list_empty(&prev->priority_donor_locks)) {
    lock_remove_from_list(lock, &thread_current()->priority_donor_locks);
    thread_revoke_donated_priority(); // Thread loses donated priority and goes
                                      // back to base
    thread_update_donated_priority(); // Get new donation if donors still exist
    thread_preempt();
  }
  thread_preempt(); // Make sure the current_thread is reset
  /*@e*/
}

/*
 * Returns true if the current thread holds LOCK, false otherwise.
 * Note that testing whether some other thread holds a lock would be racy.
 */
bool lock_held_by_current_thread(const struct lock *lock) {
  ASSERT(lock != NULL);
  return lock->holder == thread_current();
}
