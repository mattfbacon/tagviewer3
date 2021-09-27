#pragma once

class QWidget;

#include <QAction>
#include <QColor>
#include <QColorDialog>
#include <QToolButton>

#include "ColorIcon.h"

class ColorPicker : public QToolButton {
	Q_OBJECT
public:
	ColorPicker(QColor const& color = Qt::white, QWidget* const parent = nullptr);
	~ColorPicker();
	QColor color() const;
public slots:
	void set_color(QColor const& color);
	void show_dialog();
signals:
	void color_changed(QColor const& color);
protected:
	QColorDialog dialog;
	std::unique_ptr<ColorIcon> icon;
	QAction action;
	QColor m_color;
protected slots:
	/**
	 * \brief This will be triggered both by actually making a selection in the
	 * color dialog and by setting the color via `set_color`.
	 *
	 * This is because it's connected to colorSelected on the dialog, which is
	 * emitted manually by `set_color`. A bit of a bodge but it's fine as long as
	 * you don't check the color inside the dialog, and instead use `set_color`.
	 */
	void _update_color(QColor const& color);
};
