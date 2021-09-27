#pragma once

#include <QObject>
#include <QTreeView>

#include "ColorPickerDelegate.h"

class TagsModel;

class TagsView final : public QTreeView {
	Q_OBJECT
public:
	explicit TagsView(TagsModel* const model = nullptr);
	~TagsView();
protected:
	ColorPickerDelegate color_delegate;
};
