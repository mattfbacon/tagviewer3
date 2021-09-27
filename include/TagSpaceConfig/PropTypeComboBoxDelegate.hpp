#pragma once

#include <QStyledItemDelegate>

class PropTypeComboBoxDelegate final : public QStyledItemDelegate {
public:
	PropTypeComboBoxDelegate(QObject* parent = nullptr);
	~PropTypeComboBoxDelegate();
	QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const& option, QModelIndex const& index) const override;
	void setEditorData(QWidget* editor, QModelIndex const& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, QModelIndex const& index) const override;
};
