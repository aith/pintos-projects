# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(priority-donate-semaphore) begin
(priority-donate-semaphore) Thread L acquired lock.
(priority-donate-semaphore) Thread L downed semaphore.
(priority-donate-semaphore) Thread H acquired lock.
(priority-donate-semaphore) Thread H finished.
(priority-donate-semaphore) Thread M finished.
(priority-donate-semaphore) Thread L finished.
(priority-donate-semaphore) Main thread finished.
(priority-donate-semaphore) end
EOF
pass;
