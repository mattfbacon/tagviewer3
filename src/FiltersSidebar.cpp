#include <QIcon>

#include "FiltersSidebar.hpp"

FiltersSidebar::FiltersSidebar() : QFormLayout{} {
	test_button.setText(QStringLiteral("Add"));
	addRow(QStringLiteral("Filter 1"), &test_button);
}

FiltersSidebar::~FiltersSidebar() {
	//
}
