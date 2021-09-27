#include "TagSpaceConfig/TagsModel.hpp"
#include "TagSpaceConfig/TagsView.hpp"

TagsView::TagsView(TagsModel* const model) : QTreeView{} {
	setModel(model);
	setItemDelegateForColumn(1, &color_delegate);
	setItemsExpandable(false);
	setRootIsDecorated(false);
}

TagsView::~TagsView() {
	//
}
