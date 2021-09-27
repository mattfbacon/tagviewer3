#include <QKeySequence>

#include "Menu.hpp"

Menu::Menu()
	: QMenuBar{}
	, menu_tagspace{ tr("&TagSpace"), }
	, menu_media{ tr("&Media"), }
	, menu_filters{ tr("&Filters"), }
	, menu_window{ tr("&Window"), }
	, action_show_debug_panel(menu_window.addAction(tr("Show Debu&g Panel")))
{
	action_show_debug_panel->setShortcut(QKeySequence{
		tr("Ctrl+Shift+I", "Window|Show Debug Panel"),
	});
	addMenu(&menu_tagspace);
	addMenu(&menu_media);
	addMenu(&menu_filters);
	addMenu(&menu_window);
}

Menu::~Menu() {
	//
}
