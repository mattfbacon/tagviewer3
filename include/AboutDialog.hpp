#pragma once

class QWidget;

#include <QDialog>
#include <QObject>
#include <QPixmap>

class AboutDialog final : public QDialog {
	Q_OBJECT
public:
	explicit AboutDialog(QWidget* const parent);
	~AboutDialog();
protected:
	QPixmap const icon_pixmap{ QStringLiteral(":/icon.svg") };
};
