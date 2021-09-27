#pragma once

#include <QObject>
#include <QTreeView>

#include "PropTypeComboBoxDelegate.hpp"

class PropsModel;

class PropsView final : public QTreeView {
	Q_OBJECT
public:
	explicit PropsView(PropsModel* const model = nullptr);
	~PropsView();
protected:
	PropTypeComboBoxDelegate type_delegate;
};
