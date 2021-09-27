#include "ComboBoxHelpers.hpp"
#include "TagSpaceConfig/PropsModel.hpp"
#include "TagSpaceConfig/PropsView.hpp"

PropsView::PropsView(PropsModel* const model) : QTreeView{} {
	setModel(model);
	setItemDelegateForColumn(1, &type_delegate);
	setItemsExpandable(false);
	setRootIsDecorated(false);
}

PropsView::~PropsView() {
	//
}
