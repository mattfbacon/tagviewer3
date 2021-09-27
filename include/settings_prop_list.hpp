// no #pragma once
#define GROUP_INTERFACE "interface/"
#define GROUP_BEHAVIOR "behavior/"
#define GROUP_HISTORY "history/"
#define GROUP_SLIDESHOW "slideshow/"
#define GROUP_DATA "data/"
#define GROUP_TAGSPACE_DEFAULTS "tagspace_defaults/"
#define GROUP_TAG "tag/"
#define GROUP_PROP "prop/"

PROP(dark_mode, bool, bool, false, GROUP_INTERFACE "dark_mode");
PROP(save_state, bool, bool, true, GROUP_INTERFACE "save_state");
PROP(save_tagspace_history, bool, bool, true, GROUP_BEHAVIOR GROUP_HISTORY "save");
PROP(auto_reopen, bool, bool, false, GROUP_BEHAVIOR GROUP_HISTORY "auto_reopen");
PROP(slideshow_interval, int, int, (int)3000, GROUP_BEHAVIOR GROUP_SLIDESHOW "interval");
PROP(slideshow_end_on_fullscreen_exit, bool, bool, true, GROUP_BEHAVIOR GROUP_SLIDESHOW "end_on_fullscreen_exit");
PROP(slideshow_wrap_around, bool, bool, false, GROUP_BEHAVIOR GROUP_SLIDESHOW "wrap_around");
PROP(custom_css, QString, QString const&, QStringLiteral(""), GROUP_INTERFACE "custom_css");
PROP(new_tag_name, QString, QString const&, QObject::tr("New Tag", "default name"), GROUP_BEHAVIOR GROUP_TAGSPACE_DEFAULTS GROUP_TAG "name");
PROP(new_tag_color, QColor, QColor const&, (QColor{ 204, 204, 204 }), GROUP_BEHAVIOR GROUP_TAGSPACE_DEFAULTS GROUP_TAG "color");
PROP(new_prop_name, QString, QString const&, QObject::tr("New Prop", "default name"), GROUP_BEHAVIOR GROUP_TAGSPACE_DEFAULTS GROUP_PROP "name");
PROP(new_prop_type, QVariantType, QVariantType, QVariant::String, GROUP_BEHAVIOR GROUP_TAGSPACE_DEFAULTS GROUP_PROP "type");

PROP(
	tagspace_default_tags,
	TagsList,
	TagsList const&,
	(TagsList{
		Tag{ QObject::tr("Favorite", "tag name"), 0xf1'c4'0f, 0 },
		Tag{ QObject::tr("Low-Quality", "tag name"), 0xe7'4c'3c, 1 },
	}),
	GROUP_BEHAVIOR GROUP_TAGSPACE_DEFAULTS "tags");
PROP(
	tagspace_default_props,
	PropsList,
	PropsList const&,
	(PropsList{
		Prop{ QObject::tr("Description", "prop name"), QVariant::String },
	}),
	GROUP_BEHAVIOR GROUP_TAGSPACE_DEFAULTS "props");

PROP(tagspace_history, QStringList, QStringList const&, QStringList{}, GROUP_DATA "window_geometry");
PROP(window_geometry, QByteArray, QByteArray const&, QByteArray{}, GROUP_DATA "window_state");
PROP(window_state, QByteArray, QByteArray const&, QByteArray{}, GROUP_DATA "history");
