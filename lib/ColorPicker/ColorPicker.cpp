#include <QColorDialog>
#include <QDebug>

#include "ColorPicker.h"

ColorPicker::ColorPicker(QColor const& color, QWidget*const parent)
	: QToolButton{ parent, }
	, dialog{ color }
	, icon{ ColorIcon::create(color) }
	, action{ *icon, tr("Pick Color") }
	, m_color { color }
{
	setDefaultAction(&action);

	QObject::connect(&dialog, &QColorDialog::colorSelected, this, &ColorPicker::_update_color);
	QObject::connect(&dialog, &QColorDialog::colorSelected, this, &ColorPicker::color_changed);
	// QObject::connect(&dialog, &QColorDialog::)
	QObject::connect(this, &QToolButton::triggered, &dialog, &QColorDialog::show);
	QObject::connect(icon.get(), &ColorIcon::color_changed, this, static_cast<void (QWidget::*)(void)>(&QWidget::update));  // redraw icon color
}

ColorPicker::~ColorPicker() {
	//
}

QColor ColorPicker::color() const {
	return m_color;
}

void ColorPicker::set_color(QColor const& color) {
	if (color == m_color) {
		return;
	}
	dialog.setCurrentColor(color);
	// the above will not make the dialog emit colorSelected, so do it ourselves
	emit dialog.colorSelected(color);
}

void ColorPicker::show_dialog() {
	dialog.show();
}

void ColorPicker::_update_color(QColor const& color) {
	m_color = color;
	icon->set_color(color);
}
