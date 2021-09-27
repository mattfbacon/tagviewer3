#pragma once

#define DO_PRAGMA_(x) _Pragma(#x)
#define DO_PRAGMA(x) DO_PRAGMA_(x)

#define COMPILER_PRAGMA(STR) \
	DO_PRAGMA(GCC STR); \
	DO_PRAGMA(clang STR)
