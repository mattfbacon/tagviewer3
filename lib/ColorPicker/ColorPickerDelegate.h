#pragma once

#include <QStyledItemDelegate>

#include "ColorPicker.h"

class ColorPickerDelegate final : public QStyledItemDelegate {
	Q_OBJECT
public:
	ColorPickerDelegate(QObject* parent = nullptr);
	~ColorPickerDelegate();

	void paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const override;
	QSize sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const override;
	QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const& option, QModelIndex const& index) const override;
	// void destroyEditor(QWidget* editor, QModelIndex const& index) const override;
	void setEditorData(QWidget* editor, QModelIndex const& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, QModelIndex const& index) const override;
	void updateEditorGeometry(QWidget* editor, QStyleOptionViewItem const& option, QModelIndex const& index) const override;
protected:
	ColorPicker mutable helper_picker;
	static int constexpr MARGIN = 0;
};
