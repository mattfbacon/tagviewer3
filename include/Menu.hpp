#pragma once

#include <QMenu>
#include <QMenuBar>
#include <QObject>

class MainWindow;

class Menu final : public QMenuBar {
	Q_OBJECT
public:
	explicit Menu();
	~Menu();
protected:
	QMenu menu_tagspace;
	QMenu menu_media;
	QMenu menu_filters;
	QMenu menu_window;
	QAction* const action_show_debug_panel;
	friend class MainWindow;
};
