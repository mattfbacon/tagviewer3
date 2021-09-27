#include "StatusBar.hpp"

StatusBar::StatusBar() : QStatusBar{}, test_label{ QStringLiteral("Status bar") } {
	addPermanentWidget(&test_label);
	setSizeGripEnabled(true);
}

StatusBar::~StatusBar() {
	//
}
