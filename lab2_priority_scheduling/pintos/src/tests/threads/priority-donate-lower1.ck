# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(priority-donate-lower1) begin
(priority-donate-lower1) Main thread should have priority 41.  Actual priority: 41.
(priority-donate-lower1) Lowering base priority...
(priority-donate-lower1) Main thread should have priority 41.  Actual priority: 41.
(priority-donate-lower1) acquire: got the lock
(priority-donate-lower1) acquire: done
(priority-donate-lower1) acquire must already have finished.
(priority-donate-lower1) Main thread should have priority 21.  Actual priority: 21.
(priority-donate-lower1) end
EOF
pass;
