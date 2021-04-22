# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(priority-semaphore) begin
(priority-semaphore) Thread priority 30 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) Thread priority 29 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) Thread priority 28 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) Thread priority 27 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) Thread priority 26 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) Thread priority 25 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) Thread priority 24 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) Thread priority 23 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) Thread priority 22 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) Thread priority 21 woke up.
(priority-semaphore) Back in main thread.
(priority-semaphore) end
EOF
pass;
