#!/bin/bash

if [ "$#" -ne 1 ]
then
  echo "Usage: $0 test-to-run" >&2
  exit 1
fi

../utils/pintos -v -k -T 20 --qemu  -- -q  run $1
