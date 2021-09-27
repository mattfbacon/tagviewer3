#pragma once

#include <QLabel>
#include <QObject>

enum class SpacingPlacement : char {
	BEFORE = 0,
	AROUND = 1,
	AFTER = 2,
};

class QPixmap;
class QResizeEvent;
class QEvent;
class QWidget;

class AspectRatioLabel final : public QLabel {
	Q_OBJECT
public:
	explicit AspectRatioLabel(SpacingPlacement fill_vertically = SpacingPlacement::AROUND, SpacingPlacement fill_horizontally = SpacingPlacement::AROUND, bool respect_rtl = true, QWidget* const parent = nullptr, Qt::WindowFlags const f = {});
	~AspectRatioLabel();

public slots:
	void setPixmap(QPixmap const& pm);

protected:
	void resizeEvent(QResizeEvent* const event) override;
	void changeEvent(QEvent* const event) override;

	void updateMargins();

	int pixmapWidth{
		0,
	};
	int pixmapHeight{
		0,
	};
	SpacingPlacement const fill_vertically;
	SpacingPlacement const fill_horizontally;
	bool const respect_rtl;

private:
	bool is_rtl();
};
