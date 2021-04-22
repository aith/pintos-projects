#ifndef TESTS_THREADS_TESTS_H
#define TESTS_THREADS_TESTS_H

void run_test (const char *);

typedef void test_func (void);

extern test_func test_alarm_single;
extern test_func test_alarm_multiple;
extern test_func test_alarm_simultaneous;
extern test_func test_alarm_zero;
extern test_func test_alarm_negative;

extern test_func test_alarm_priority;

extern test_func test_priority_semaphore;
extern test_func test_priority_condvar;
extern test_func test_priority_preempt;
extern test_func test_priority_fifo;

extern test_func test_priority_change;
extern test_func test_priority_donate_one;
extern test_func test_priority_donate_single;
extern test_func test_priority_donate_lower1;
extern test_func test_priority_donate_lower2;
extern test_func test_priority_donate_multiple1;
extern test_func test_priority_donate_multiple2;
extern test_func test_priority_donate_semaphore;
extern test_func test_priority_donate_condvar;
extern test_func test_priority_donate_nest;
extern test_func test_priority_donate_chain;

void msg (const char *, ...);
void fail (const char *, ...);
void pass (void);

#endif /* tests/threads/tests.h */

