#include "SettingsTreeView.hpp"

SettingsTreeView::SettingsTreeView(QStandardItemModel* const model, QWidget* const parent)
	: QTreeView{
		parent,
	} {
	setHeaderHidden(true);
	setUniformRowHeights(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setDropIndicatorShown(false);
	setDragEnabled(false);
	setDragDropMode(QAbstractItemView::NoDragDrop);
	setSelectionBehavior(QAbstractItemView::SelectItems);

	setModel(model);

	QObject::connect(this, &QTreeView::clicked, this, &SettingsTreeView::section_selected);
	QObject::connect(this, &QTreeView::activated, this, &SettingsTreeView::section_selected);
}

SettingsTreeView::~SettingsTreeView() {
	//
}

void SettingsTreeView::selectionChanged(QItemSelection const& selections, QItemSelection const& old_selections) {
	if (selections.size() >= 1) {
		auto const& selected_items = selections.at(0).indexes();
		if (selected_items.size() >= 1) { emit section_selected(selected_items.at(0)); }
	}
	QTreeView::selectionChanged(selections, old_selections);
}
