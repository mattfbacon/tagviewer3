#include <QPainter>
#include <QRect>

#include "ColorIcon.h"
#include "SaveGuard.h"

using namespace ColorPickerInternal;

QColor const ColorIconEngine::checkerboard_colors[2] = {
	QColor{ 0, 0, 0, 0x44 },
	QColor{ 255, 255, 255, 0x44 },
};

ColorIconEngine::ColorIconEngine(QColor const& color) : QIconEngine{}, color_brush{ color } {
	//
}

ColorIconEngine::~ColorIconEngine() {
	//
}

void ColorIconEngine::paint(QPainter* const painter, QRect const& rect, QIcon::Mode const, QIcon::State const) {
	SaveGuard guard{ painter };
	int const num_squares_horiz = rect.width() / CHECKERBOARD_DENSITY + 1;  // hang over the edge
	int const num_squares_vert = rect.height() / CHECKERBOARD_DENSITY + 1;  // ''
	painter->setClipRect(rect);
	painter->setClipping(true);
	{
		QRect temp{ rect.topLeft(), QSize{ CHECKERBOARD_DENSITY, CHECKERBOARD_DENSITY } };
		for (int y = 0; y < num_squares_vert; ++y) {
			temp.setY(rect.top() + y * CHECKERBOARD_DENSITY);
			temp.setHeight(CHECKERBOARD_DENSITY);  // QRect stores {x,y}{1,2} internally
			for (int x = 0; x < num_squares_horiz; ++x) {
				temp.setX(rect.left() + x * CHECKERBOARD_DENSITY);
				temp.setWidth(CHECKERBOARD_DENSITY);
				painter->fillRect(temp, checkerboard_colors[(x + y) % 2]);
			}
		}
	}
	painter->fillRect(rect, color_brush);
}

ColorIconEngine* ColorIconEngine::clone() const {
	return new ColorIconEngine{ *this };
}

QColor const& ColorIconEngine::color() const {
	return color_brush.color();
}

void ColorIconEngine::set_color(QColor const& color) {
	color_brush.setColor(color);
}

ColorIcon* ColorIcon::create(QColor const& color) {
	return new ColorIcon{ new ColorIconEngine{ color } };
}

ColorIcon::ColorIcon(ColorIconEngine* const engine) : QObject{}, QIcon{ engine }, engine{ engine } {
	//
}

ColorIcon::ColorIcon(ColorIcon&& other) : QObject{}, QIcon{ other.engine }, engine{ other.engine } {}

ColorIcon::~ColorIcon() {
	//
}

QColor const& ColorIcon::color() const {
	return engine->color();
}

void ColorIcon::set_color(QColor const& color) {
	engine->set_color(color);
	emit color_changed(color);
}
