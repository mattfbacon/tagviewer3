#pragma once

#include <QIcon>

namespace Icons {

void load();

extern QPalette helper_palette;

extern QIcon file_open;  // folder_open
extern QIcon file_new;  // create_new_folder
extern QIcon file_reopen;  // call_missed_outgoing

extern QIcon go_first;  // first_page
extern QIcon go_prev;  // navigate_before
extern QIcon go_next;  // navigate_next
extern QIcon go_last;  // last_page

extern QIcon media_tagspace_edit;  // tune
extern QIcon media_add;  // add_photo_alternate
extern QIcon media_open_external;  // open_in_browser
extern QIcon media_export;  // file_download
extern QIcon media_remove;  // remove_circle
extern QIcon media_replace;  // flip_camera_ios

extern QIcon slideshow_start;  // play_circle
extern QIcon slideshow_start_fullscreen;  // slideshow
extern QIcon slideshow_stop;  // stop

extern QIcon meta_settings;  // settings
extern QIcon meta_about;  // info
extern QIcon meta_help;  // help

extern QIcon fullscreen_enter;  // fullscreen
extern QIcon fullscreen_exit;  // fullscreen_exit

// technically only LIGHT_THEME_DARK and vice versa are required
// but for consistency all combinations are included.
extern QIcon theme_light;  // light_mode
extern QIcon theme_dark;  // dark_mode
}  // namespace Icons
