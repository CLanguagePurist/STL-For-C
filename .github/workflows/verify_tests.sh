#!/bin/bash
declare status="FAIL"

if [[ -n $(grep 'Fail:               0' ./build/meson-logs/testlog.txt) ]] && \
   [[ -n $(grep 'Timeout:            0' ./build/meson-logs/testlog.txt) ]] && \
   [[ -n $(grep 'Unexpected Pass:    0' ./build/meson-logs/testlog.txt) ]]; then status="PASS"; fi

echo $status
