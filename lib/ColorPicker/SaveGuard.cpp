#include "SaveGuard.h"

namespace ColorPickerInternal {

SaveGuard::SaveGuard(QPainter* const painter) : painter(painter) {
	painter->save();
}

SaveGuard::~SaveGuard() {
	painter->restore();
}

}  // namespace ColorPickerInternal
