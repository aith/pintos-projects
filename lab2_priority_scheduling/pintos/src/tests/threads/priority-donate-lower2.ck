# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(priority-donate-lower2) begin
(priority-donate-lower2) Thread B finished.
(priority-donate-lower2) Thread A finished.
(priority-donate-lower2) end
EOF
pass;