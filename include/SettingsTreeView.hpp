#pragma once

#include <QObject>
#include <QStandardItemModel>
#include <QTreeView>

class SettingsTreeView final : public QTreeView {
	Q_OBJECT
public:
	explicit SettingsTreeView(QStandardItemModel* const model, QWidget* const parent = nullptr);
	~SettingsTreeView();
protected slots:
	void selectionChanged(QItemSelection const& selections, QItemSelection const& old_selections) override;
signals:
	void section_selected(QModelIndex const& section);
};
