#include <QApplication>
#include <QEvent>
#include <QPixmap>

#include "AspectRatioLabel.hpp"

static int margin_coefficient[] = { 2, 1, 0 };

AspectRatioLabel::AspectRatioLabel(SpacingPlacement fill_vertically, SpacingPlacement fill_horizontally, bool respect_rtl, QWidget*const parent, Qt::WindowFlags const f)
	: QLabel{ parent, f, }
	, fill_vertically(fill_vertically)
	, fill_horizontally(fill_horizontally)
	, respect_rtl(respect_rtl)
{
	//
}

AspectRatioLabel::~AspectRatioLabel() {
	//
}

void AspectRatioLabel::setPixmap(QPixmap const& pm) {
	pixmapWidth = pm.width();
	pixmapHeight = pm.height();

	updateMargins();
	QLabel::setPixmap(pm);
}

void AspectRatioLabel::resizeEvent(QResizeEvent* const event) {
	updateMargins();
	QLabel::resizeEvent(event);
}

void AspectRatioLabel::changeEvent(QEvent* const event) {
	if (event->type() == QEvent::LayoutDirectionChange) {
		updateMargins();
	}
	QLabel::changeEvent(event);
}

void AspectRatioLabel::updateMargins() {
	if (pixmapWidth <= 0 || pixmapHeight <= 0) {
		return;
	}

	int const w = width();
	int const h = height();

	if (w <= 0 || h <= 0) {
		return;
	}

	if (w * pixmapHeight > h * pixmapWidth) {
		int const m = (w - (pixmapWidth * h / pixmapHeight)) / 2;
		int left_coefficient = margin_coefficient[static_cast<int>(fill_horizontally)];
		if (respect_rtl && is_rtl()) {
			left_coefficient = 2 - left_coefficient;
		}  // swap side
		setContentsMargins(m * left_coefficient, 0, m * (2 - left_coefficient), 0);
	} else {
		int const m = (h - (pixmapHeight * w / pixmapWidth)) / 2;
		int const top_coefficient = margin_coefficient[static_cast<int>(fill_vertically)];
		setContentsMargins(0, m * top_coefficient, 0, m * (2 - top_coefficient));
	}
}

bool AspectRatioLabel::is_rtl() {
	Qt::LayoutDirection direction;
	QWidget const* current = this;
	do {
		direction = current->layoutDirection();
		current = current->parentWidget();
		if (current == nullptr) {
			direction = qApp->layoutDirection();
			break;
		}
	} while (direction == Qt::LayoutDirectionAuto);
	return direction == Qt::RightToLeft;
}
