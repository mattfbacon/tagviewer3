#include <QByteArray>
#include <QList>
#include <QString>

#include "definitions.hpp"

QString const EMPTY_STRING = QStringLiteral("");

#pragma region Icons
#include "qfonticon.h"

#include "icons.hpp"
#include "templates.hpp"

/* Old icons
QString const& ICON_FILE_OPEN = QStringLiteral("\ue2c8"); // folder_open
QString const& ICON_FILE_NEW = QStringLiteral("\ue2cc"); // create_new_folder
QString const& ICON_FILE_REOPEN = QStringLiteral("\ue0e4"); // call_missed_outgoing

QString const& ICON_GO_FIRST = QStringLiteral("\ue5dc"); // first_page
QString const& ICON_GO_PREV = QStringLiteral("\ue408"); // navigate_before
QString const& ICON_GO_NEXT = QStringLiteral("\ue409"); // navigate_next
QString const& ICON_GO_LAST = QStringLiteral("\ue5dd"); // last_page

QString const& ICON_MEDIA_TAGSPACE_EDIT = QStringLiteral("\ue429"); // tune
QString const& ICON_MEDIA_ADD = QStringLiteral("\ue43e"); // add_photo_alternate
QString const& ICON_MEDIA_OPEN_EXTERNAL = QStringLiteral("\ue89d"); // open_in_browser
QString const& ICON_MEDIA_EXPORT = QStringLiteral("\ue2c4"); // file_download
QString const& ICON_MEDIA_REMOVE = QStringLiteral("\ue15c"); // remove_circle
QString const& ICON_MEDIA_REPLACE = QStringLiteral("\uea38"); // flip_camera_ios

QString const& ICON_SLIDESHOW_START = QStringLiteral("\ue1c4"); // play_circle
QString const& ICON_SLIDESHOW_START_FULLSCREEN = QStringLiteral("\ue41b"); // slideshow
QString const& ICON_SLIDESHOW_STOP = QStringLiteral("\ue047"); // stop

QString const& ICON_META_SETTINGS = QStringLiteral("\ue8b8"); // settings
QString const& ICON_META_ABOUT = QStringLiteral("\ue88e"); // info
QString const& ICON_META_HELP = QStringLiteral("\ue887"); // help

QString const& ICON_FULLSCREEN_ENTER = QStringLiteral("\ue5d0"); // fullscreen
QString const& ICON_FULLSCREEN_EXIT = QStringLiteral("\ue5d1"); // fullscreen_exit

QString const& ICON_THEME_LIGHT = QStringLiteral("\ue518"); // light_mode
QString const& ICON_THEME_DARK = QStringLiteral("\ue51c"); // dark_mode
*/

namespace Icons {

QPalette helper_palette;

#define LOAD_ICON(NAME, CODE) NAME = FIcon(CODE, helper_palette)

void load() {
	LOAD_ICON(file_open, 0xe2c8);
	LOAD_ICON(file_new, 0xe2cc);
	LOAD_ICON(file_reopen, 0xe0e4);

	LOAD_ICON(go_first, 0xe5dc);
	LOAD_ICON(go_prev, 0xe408);
	LOAD_ICON(go_next, 0xe409);
	LOAD_ICON(go_last, 0xe5dd);

	LOAD_ICON(media_tagspace_edit, 0xe429);
	LOAD_ICON(media_add, 0xe43e);
	LOAD_ICON(media_open_external, 0xe89d);
	LOAD_ICON(media_export, 0xe2c4);
	LOAD_ICON(media_remove, 0xe15c);
	LOAD_ICON(media_replace, 0xea38);

	LOAD_ICON(slideshow_start, 0xe1c4);
	LOAD_ICON(slideshow_start_fullscreen, 0xe41b);
	LOAD_ICON(slideshow_stop, 0xe047);

	LOAD_ICON(meta_settings, 0xe8b8);
	LOAD_ICON(meta_about, 0xe88e);
	LOAD_ICON(meta_help, 0xe887);

	LOAD_ICON(fullscreen_enter, 0xe5d0);
	LOAD_ICON(fullscreen_exit, 0xe5d1);

	LOAD_ICON(theme_light, 0xe518);
	LOAD_ICON(theme_dark, 0xe51c);
}

// null initializers

QIcon file_open;
QIcon file_new;
QIcon file_reopen;

QIcon go_first;
QIcon go_prev;
QIcon go_next;
QIcon go_last;

QIcon media_tagspace_edit;
QIcon media_add;
QIcon media_open_external;
QIcon media_export;
QIcon media_remove;
QIcon media_replace;

QIcon slideshow_start;
QIcon slideshow_start_fullscreen;
QIcon slideshow_stop;

QIcon meta_settings;
QIcon meta_about;
QIcon meta_help;

QIcon fullscreen_enter;
QIcon fullscreen_exit;

QIcon theme_light;
QIcon theme_dark;

}  // namespace Icons

#include "links.hpp"

namespace Links {

QString const help = QStringLiteral("https://tagviewer.gitbook.io/tagviewer/v/v3/");

}

#include "themes.hpp"

namespace Themes {

QPalette const light = by_name(ThemeType::POLAR);
QPalette const dark = by_name(ThemeType::CARBON);

}  // namespace Themes
