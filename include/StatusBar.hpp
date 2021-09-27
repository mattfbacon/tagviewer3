#pragma once

#include <QLabel>
#include <QObject>
#include <QStatusBar>

class StatusBar final : public QStatusBar {
	Q_OBJECT
public:
	explicit StatusBar();
	~StatusBar();
protected:
	QLabel test_label;
};
