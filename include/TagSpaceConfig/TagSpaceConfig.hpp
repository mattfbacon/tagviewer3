#pragma once

#include <QObject>
#include <QToolBox>

class TagsView;
class PropsView;

#include "TagSpaceConfig/PropsModel.hpp"
#include "TagSpaceConfig/TagsModel.hpp"

class TagSpaceConfig final : public QToolBox {
	Q_OBJECT
public:
	explicit TagSpaceConfig();
	~TagSpaceConfig();
	TagsList const& get_tags();
	PropsList const& get_props();
signals:
	void tags_changed();
	void props_changed();
	void data_changed();
protected:
	TagsModel tags_model;
	TagsView* const tags_page;
	PropsModel props_model;
	PropsView* const props_page;
};
