#include "tests/threads/tests.h"
#include <debug.h>
#include <string.h>
#include <stdio.h>

struct test 
  {
    const char *name;
    test_func *function;
  };

static const struct test tests[] = 
  {
    {"alarm-single", test_alarm_single},
    {"alarm-multiple", test_alarm_multiple},
    {"alarm-simultaneous", test_alarm_simultaneous},
    {"alarm-zero", test_alarm_zero},
    {"alarm-negative", test_alarm_negative},

    {"alarm-priority", test_alarm_priority},
    {"priority-fifo", test_priority_fifo},
    {"priority-preempt", test_priority_preempt},
    {"priority-change", test_priority_change},

    {"priority-semaphore", test_priority_semaphore},
    {"priority-condvar", test_priority_condvar},

    {"priority-donate-one", test_priority_donate_one},
    {"priority-donate-single", test_priority_donate_single},
    {"priority-donate-lower1", test_priority_donate_lower1},
    {"priority-donate-lower2", test_priority_donate_lower2},
    {"priority-donate-multiple1", test_priority_donate_multiple1},
    {"priority-donate-multiple2", test_priority_donate_multiple2},
    {"priority-donate-semaphore", test_priority_donate_semaphore},
    {"priority-donate-condvar", test_priority_donate_condvar},

    {"priority-donate-nest", test_priority_donate_nest},
    {"priority-donate-chain", test_priority_donate_chain},
  };

static const char *test_name;

/* Runs the test named NAME. */
void
run_test (const char *name) 
{
  const struct test *t;

  for (t = tests; t < tests + sizeof tests / sizeof *tests; t++)
    if (!strcmp (name, t->name))
      {
        test_name = name;
        msg ("begin");
        t->function ();
        msg ("end");
        return;
      }
  PANIC ("no test named \"%s\"", name);
}

/* Prints FORMAT as if with printf(),
   prefixing the output by the name of the test
   and following it with a new-line character. */
void
msg (const char *format, ...) 
{
  va_list args;
  
  printf ("(%s) ", test_name);
  va_start (args, format);
  vprintf (format, args);
  va_end (args);
  putchar ('\n');
}

/* Prints failure message FORMAT as if with printf(),
   prefixing the output by the name of the test and FAIL:
   and following it with a new-line character,
   and then panics the kernel. */
void
fail (const char *format, ...) 
{
  va_list args;
  
  printf ("(%s) FAIL: ", test_name);
  va_start (args, format);
  vprintf (format, args);
  va_end (args);
  putchar ('\n');

  PANIC ("test failed");
}

/* Prints a message indicating the current test passed. */
void
pass (void) 
{
  printf ("(%s) PASS\n", test_name);
}

