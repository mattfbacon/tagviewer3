#pragma once

#include <QBrush>
#include <QIcon>
#include <QIconEngine>
#include <QObject>
class QRect;

class ColorIconEngine : public QIconEngine {
public:
	ColorIconEngine(QColor const& color);
	~ColorIconEngine();
	void paint(QPainter* painter, QRect const& rect, QIcon::Mode mode, QIcon::State state) override;
	ColorIconEngine* clone() const override;
	QColor const& color() const;
	void set_color(QColor const& color);
protected:
	QBrush color_brush;
	static int constexpr CHECKERBOARD_DENSITY = 8;  // pixels per square
	static QColor const checkerboard_colors[2];
};

class ColorIcon
	: public QObject
	, public QIcon {
	Q_OBJECT
public:
	static ColorIcon* create(QColor const& color);
	ColorIcon(ColorIcon&& other);
	~ColorIcon();
	QColor const& color() const;
public slots:
	void set_color(QColor const& color);
signals:
	void color_changed(QColor const& color);
protected:
	ColorIcon(ColorIconEngine* engine);
	ColorIconEngine* const engine;
};
