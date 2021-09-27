#include "FileList.hpp"

FileList::FileList()
	: QListView{}
	, strings{ QStringLiteral("foo"), QStringLiteral("bar"), QStringLiteral("baz"), }
	, model{ strings, }
{
	setModel(&model);
}

FileList::~FileList() {
	//
}
