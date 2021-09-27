#include "ComboBoxHelpers.hpp"
#include "TagSpaceConfig/PropTypeComboBoxDelegate.hpp"

PropTypeComboBoxDelegate::PropTypeComboBoxDelegate(QObject* const parent) : QStyledItemDelegate{ parent } {}

PropTypeComboBoxDelegate::~PropTypeComboBoxDelegate() {}

QWidget* PropTypeComboBoxDelegate::createEditor(QWidget* const parent, QStyleOptionViewItem const&, QModelIndex const&) const {
	return new ComboBoxHelpers::NewPropType::ComboBox{ parent };
}

void PropTypeComboBoxDelegate::setEditorData(QWidget* const editor, QModelIndex const& index) const {
	reinterpret_cast<ComboBoxHelpers::NewPropType::ComboBox* const>(editor)->from_saved(index.data(Qt::EditRole).value<QVariantType>());
}

void PropTypeComboBoxDelegate::setModelData(QWidget* const editor, QAbstractItemModel* const model, QModelIndex const& index) const {
	model->setData(index, reinterpret_cast<ComboBoxHelpers::NewPropType::ComboBox* const>(editor)->to_saved(), Qt::EditRole);
}
