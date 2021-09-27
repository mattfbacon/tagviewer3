#pragma once

class QAction;

#include <QObject>
#include <QToolBar>

class ActionBar final : public QToolBar {
	Q_OBJECT
public:
	explicit ActionBar();
	~ActionBar();
};
