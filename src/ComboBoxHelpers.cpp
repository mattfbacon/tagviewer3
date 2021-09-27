#include <QString>

#include "ComboBoxHelpers.hpp"

#include "pragmas.hpp"

namespace ComboBoxHelpers {

namespace NewPropType {
std::array<QVariantType, ITEMS> const types{
	QVariant::String, QVariant::Int, QVariant::Double, QVariant::Bool, QVariant::DateTime,
};
// avoiding vertical alignment is the hill I will die on
// clang-format off
std::array<QString, ITEMS> const names{
	QStringLiteral("String"),
	QStringLiteral("Whole Number"),
	QStringLiteral("Decimal Number"),
	QStringLiteral("Boolean (yes/no)"),
	QStringLiteral("Date and Time"),
};
// clang-format on

int from_saved(QVariantType const x) {
	auto const begin = std::cbegin(types);
	auto const end = std::cend(types);
	auto const found = std::find(begin, end, x);
	Q_ASSERT(found != end);
	if (Q_UNLIKELY(found == end)) {
		return -1;
	} else {
		return found - begin;
	}
}
QVariantType to_saved(int const x) {
	if (x == -1) {
		return QVariant::Invalid;
	}
	return types[x];
}

ComboBox::ComboBox(QWidget* const parent)
	: QComboBox{ parent } {
	for (auto const& str : ComboBoxHelpers::NewPropType::names) {
		addItem(str);
	}
}
QVariantType ComboBox::to_saved() const {
	return ComboBoxHelpers::NewPropType::to_saved(currentIndex());
}
void ComboBox::from_saved(QVariantType const type) {
	setCurrentIndex(ComboBoxHelpers::NewPropType::from_saved(type));
}

}  // namespace NewPropType

namespace SlideshowWrapAround {}  // namespace SlideshowWrapAround

}  // namespace ComboBoxHelpers
