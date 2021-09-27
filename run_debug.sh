#!/bin/bash
set -euxo pipefail

make debug
export LSAN_OPTIONS="suppressions=lsan.supp"
export ASAN_OPTIONS="detect_leaks=1 quarantine_size_mb=512 check_initialization_order=true detect_stack_use_after_return=true"
if which -- gammaray >/dev/null 2>/dev/null && [ "$#" -eq 0 ]; then
	gammaray ./dist/Debug/tagviewer
else
	./dist/Debug/tagviewer
fi
