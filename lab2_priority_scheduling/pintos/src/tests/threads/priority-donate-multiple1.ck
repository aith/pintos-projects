# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(priority-donate-multiple1) begin
(priority-donate-multiple1) Main thread should have priority 32.  Actual priority: 32.
(priority-donate-multiple1) Main thread should have priority 33.  Actual priority: 33.
(priority-donate-multiple1) Thread b acquired lock b.
(priority-donate-multiple1) Thread b finished.
(priority-donate-multiple1) Thread b should have just finished.
(priority-donate-multiple1) Main thread should have priority 32.  Actual priority: 32.
(priority-donate-multiple1) Thread a acquired lock a.
(priority-donate-multiple1) Thread a finished.
(priority-donate-multiple1) Thread a should have just finished.
(priority-donate-multiple1) Main thread should have priority 31.  Actual priority: 31.
(priority-donate-multiple1) end
EOF
pass;
