#!/bin/sh
find . -type f -wholename './src/**.cpp' -o -wholename './include/*.hpp' -exec clang-format {} +
