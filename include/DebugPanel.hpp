#pragma once

#include <QDialog>
#include <QObject>

class QGridLayout;
class QWidget;

class DebugPanel final : public QDialog {
	Q_OBJECT
public:
	explicit DebugPanel(QWidget* const parent = nullptr);
	~DebugPanel();
signals:
	void reset_style();
};
