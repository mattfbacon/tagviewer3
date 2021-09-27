#pragma once

#include <array>

#include <QComboBox>

#include "QVariantType.hpp"

namespace ComboBoxHelpers {

namespace NewPropType {
unsigned long constexpr ITEMS = 5;
extern std::array<QVariantType, ITEMS> const types;
extern std::array<QString, ITEMS> const names;

int from_saved(QVariantType x);
QVariantType to_saved(int x);

class ComboBox final : public QComboBox {
public:
	ComboBox(QWidget* const parent = nullptr);
	QVariantType to_saved() const;
	void from_saved(QVariantType const type);
};

}  // namespace NewPropType

namespace SlideshowWrapAround {
__attribute__((always_inline)) inline int from_saved(bool const x) {
	return x ? 1 : 0;
}
__attribute__((always_inline)) inline bool to_saved(int const x) {
	return x == 1;
}
}  // namespace SlideshowWrapAround

}  // namespace ComboBoxHelpers
