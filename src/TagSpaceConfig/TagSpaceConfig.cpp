#include <QSizePolicy>

#include "TagSpaceConfig/PropsView.hpp"
#include "TagSpaceConfig/TagSpaceConfig.hpp"
#include "TagSpaceConfig/TagsView.hpp"

#include "initptr.hpp"

TagSpaceConfig::TagSpaceConfig()
	: QToolBox{}
	, INIT_PTR(tags_page, &tags_model)
	, INIT_PTR(props_page, &props_model) {
	tags_model.add_tag(Tag{ QStringLiteral("Test tag"), QColor(10, 40, 70) });
	props_model.add_prop(Prop{ QStringLiteral("test prop"), QVariant::String });
	addItem(tags_page, tr("Tags"));
	addItem(props_page, tr("Properties"));
	setCurrentIndex(0);
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding, QSizePolicy::Frame));
	QObject::connect(&tags_model, &TagsModel::dataChanged, this, &TagSpaceConfig::tags_changed);
	QObject::connect(&props_model, &PropsModel::dataChanged, this, &TagSpaceConfig::props_changed);
	QObject::connect(this, &TagSpaceConfig::tags_changed, this, &TagSpaceConfig::data_changed);
	QObject::connect(this, &TagSpaceConfig::props_changed, this, &TagSpaceConfig::data_changed);
}

TagsList const& TagSpaceConfig::get_tags() {
	return tags_model.save();
}

PropsList const& TagSpaceConfig::get_props() {
	return props_model.save();
}

TagSpaceConfig::~TagSpaceConfig() {
	//
}
