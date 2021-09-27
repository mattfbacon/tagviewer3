#include <QDebug>
#include <QPainter>

#include "ColorPickerDelegate.h"
#include "SaveGuard.h"

using namespace ColorPickerInternal;

ColorPickerDelegate::ColorPickerDelegate(QObject* const parent) : QStyledItemDelegate{ parent } {
	//
}

ColorPickerDelegate::~ColorPickerDelegate() {
	//
}

void ColorPickerDelegate::paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const {
	SaveGuard guard{ painter };
	painter->setBrush(QBrush{ index.data(Qt::EditRole).value<QColor>() });
	painter->setClipRect(option.rect);
	painter->setClipping(true);
	painter->setPen(Qt::NoPen);
	painter->drawRect(QRect{ option.rect.topLeft() + QPoint{ MARGIN, MARGIN }, QSize{ option.rect.height() - MARGIN * 2, option.rect.height() - MARGIN * 2 } });
	painter->setClipping(false);
	painter->translate(QPoint{ option.rect.height(), 0 });
	QStyledItemDelegate::paint(painter, option, index);
}

QSize ColorPickerDelegate::sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const {
	auto size = QStyledItemDelegate::sizeHint(option, index);
	size.rwidth() += option.rect.height();
	return size;
}

class QWidgetSurrogate final : public QWidget {
public:
	QWidgetSurrogate(void* data, QWidget*const parent = nullptr) : QWidget{ parent, }, data(data) {
		//
	}
	void* data;
};

QWidget* ColorPickerDelegate::createEditor(QWidget* const parent, QStyleOptionViewItem const&, QModelIndex const& index) const {
	(void)index;
	auto* const picker = new QColorDialog{ Qt::white, parent };
	picker->setModal(true);
	auto* const surrogate = new QWidgetSurrogate{ picker, parent };
	QObject::connect(picker, &QColorDialog::colorSelected, [this, surrogate, index](QColor const& color) {
		// this is a hack but it seems to work well
		const_cast<QAbstractItemModel*>(index.model())->setData(index, color, Qt::EditRole);
		emit const_cast<ColorPickerDelegate*>(this)->commitData(surrogate);
		emit const_cast<ColorPickerDelegate*>(this)->closeEditor(surrogate);
	});
	picker->setAttribute(Qt::WA_DeleteOnClose);
	picker->show();
	return surrogate;
}

void ColorPickerDelegate::setEditorData(QWidget* const editor, QModelIndex const& index) const {
	reinterpret_cast<QColorDialog*>(reinterpret_cast<QWidgetSurrogate*>(editor)->data)->setCurrentColor(index.data(Qt::EditRole).value<QColor>());
}

void ColorPickerDelegate::setModelData(QWidget* const editor, QAbstractItemModel* const model, QModelIndex const& index) const {
	if (auto const selectedColor = reinterpret_cast<QColorDialog*>(reinterpret_cast<QWidgetSurrogate*>(editor)->data)->selectedColor(); selectedColor.isValid()) {
		model->setData(index, selectedColor, Qt::EditRole);
	}
}

void ColorPickerDelegate::updateEditorGeometry(QWidget* const editor, QStyleOptionViewItem const& option, QModelIndex const&) const {
	editor->setGeometry(option.rect);
}
