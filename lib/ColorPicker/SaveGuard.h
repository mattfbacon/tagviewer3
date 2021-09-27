#pragma once

#include <QPainter>

namespace ColorPickerInternal {

class SaveGuard {
public:
	SaveGuard(QPainter* const painter);
	~SaveGuard();
protected:
	QPainter* const painter;
};

}  // namespace ColorPickerInternal
