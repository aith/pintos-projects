/*
 * Thread A ( the main thread ) gives itself a high pririty, aquires
 * a lock then creates Thread B with a lower priority. 
 * 
 * Thread A then lowers it's priority to below Thread B which causes a 
 * context switch as Thread B now has the hightest priority in the system.
 * 
 * Thread B attemots to acquire the lock but cannot so donates to Thread A
 * which now releases the lock causing a revocation of the donated priority
 * and a context switch as Thread B now has the highest priority in the system.
 * 
 * Thread B now releases the lock and exits leaving Thread A to complet as it 
 * is the only thrdad left in a runnable state.
 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/lock.h"
#include "threads/thread.h"

static thread_func b_thread_func;

static struct lock lock;

void test_priority_donate_lower2(void) 
{
    thread_set_priority(40);

    lock_init(&lock);
    lock_acquire(&lock);

    thread_create("Thread B", 30, b_thread_func, NULL);

    thread_set_priority(20);
    lock_release(&lock);

    msg("Thread A finished.");
}

static void b_thread_func(void *aux UNUSED) 
{
    lock_acquire(&lock);
    lock_release(&lock);
    msg("%s finished.", thread_current()->name);
}