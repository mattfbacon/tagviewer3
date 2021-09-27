#include <QAction>

#include "ActionBar.hpp"

ActionBar::ActionBar() : QToolBar{} {
	setToolButtonStyle(Qt::ToolButtonIconOnly);
}

ActionBar::~ActionBar() {
	//
}
