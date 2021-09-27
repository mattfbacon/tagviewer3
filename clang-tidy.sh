#!/bin/sh
find . -type f -wholename './src/**.cpp' -o -wholename './include/*.hpp' -exec clang-tidy {} -- -isystem/usr/include/qt -isystem/usr/include/qt/QtGui -isystem/usr/include/qt/QtCore -isystem/usr/include/qt/QtWidgets -isystem/usr/include/qt/QtSql -isystem/usr/include/qt/QtSvg -iquoteinclude -iquotelib/phantom/src/phantom -iquotelib/QFunctionUtils/src -iquotelib/QFontIcon/QFontIcon +
