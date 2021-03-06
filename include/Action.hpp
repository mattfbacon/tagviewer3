#pragma once

enum class Action : unsigned int {
	FILE_OPEN,
	FILE_NEW,
	FILE_REOPEN,
	GO_FIRST,
	GO_PREV,
	GO_NEXT,
	GO_LAST,
	MEDIA_TAGSPACE_EDIT,
	MEDIA_ADD,
	MEDIA_OPEN_EXTERNAL,
	MEDIA_EXPORT,
	MEDIA_REMOVE,
	MEDIA_REPLACE,
	SLIDESHOW_START,
	SLIDESHOW_START_FULLSCREEN,
	META_SETTINGS,
	META_ABOUT,
	META_HELP,
	TOGGLE_FULLSCREEN,
	TOGGLE_THEME,
};
