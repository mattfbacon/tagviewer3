#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QObject>
#include <QVariant>

#include "QVariantType.hpp"

#pragma region Forward Declarations
enum class Action : unsigned int;
class MainWindow;
class ActionBar;
class FileList;
class PropertiesSidebar;
class FiltersSidebar;
class Menu;
class StatusBar;
class CenterPanel;
class QSettings;
class AboutDialog;
class SettingsDialog;
class DebugPanel;
#pragma endregion

#define MAINWINDOW_UPDATE_SLOT(NAME) update_##NAME
#define MAINWINDOW_UPDATE_SIGNAL(NAME) NAME##_updated
#define MAINWINDOW_MEMBER_PREFIX(NAME) m_##NAME

#define DECLARE_MAINWINDOW_UPDATE_SLOT(NAME, ...) Q_SLOT void MAINWINDOW_UPDATE_SLOT(NAME)(__VA_ARGS__)
#define DECLARE_MAINWINDOW_UPDATE_SIGNAL(NAME, ...) Q_SIGNAL void MAINWINDOW_UPDATE_SIGNAL(NAME)(__VA_ARGS__)
#define DECLARE_MAINWINDOW_UPDATE_PAIR(NAME, ...) \
	DECLARE_MAINWINDOW_UPDATE_SLOT(NAME, __VA_ARGS__); \
	DECLARE_MAINWINDOW_UPDATE_SIGNAL(NAME, __VA_ARGS__)

#define DECLARE_MAINWINDOW_PROPERTY(NAME, TYPE, SHARED_TYPE) \
public: \
	Q_PROPERTY(TYPE NAME MEMBER MAINWINDOW_MEMBER_PREFIX(NAME) READ NAME WRITE MAINWINDOW_UPDATE_SLOT(NAME) NOTIFY MAINWINDOW_UPDATE_SIGNAL(NAME)) \
	SHARED_TYPE NAME() const; \
	Q_SLOT void MAINWINDOW_UPDATE_SLOT(NAME)(SHARED_TYPE); \
	Q_SIGNAL void MAINWINDOW_UPDATE_SIGNAL(NAME)(SHARED_TYPE); \
protected: \
	TYPE MAINWINDOW_MEMBER_PREFIX(NAME)

class MainWindow final : public QMainWindow {
	Q_OBJECT
	DECLARE_MAINWINDOW_PROPERTY(fullscreen, bool, bool);
public:
	explicit MainWindow();
	~MainWindow();
	void changeEvent(QEvent* event) override;
public slots:
	void clear_tagspace_history();

	void show_settings();
	void show_about();
	void show_debug_panel();
protected slots:
	void MAINWINDOW_UPDATE_SLOT(dark_mode)(bool is_dark_mode);
	void update_action_fullscreen(bool is_fullscreen);
	void update_action_dark_mode(bool is_dark_mode);
protected:
	// TagSpace* tagspace;

	Menu* const menu;
	StatusBar* const statusbar;

	ActionBar* const actionbar;
	QDockWidget actionbar_dock;

	FileList* const file_list;
	QDockWidget file_list_dock;

	PropertiesSidebar* const props_sidebar;
	QWidget* const props_sidebar_surrogate;
	QDockWidget props_sidebar_dock;

	FiltersSidebar* const filters_sidebar;
	QWidget* const filters_sidebar_surrogate;
	QDockWidget filters_sidebar_dock;

	CenterPanel* const center_panel;

	AboutDialog* const about_dialog;
	SettingsDialog* const settings_dialog;

	DebugPanel* const debug_panel;

	void actionbar_handler(Action action);
public:  // ACTIONS
	QAction* action_file_open;
	QAction* action_file_new;
	QAction* action_file_reopen;

	QAction* action_go_first;
	QAction* action_go_prev;
	QAction* action_go_next;
	QAction* action_go_last;

	QAction* action_media_tagspace_edit;
	QAction* action_media_add;
	QAction* action_media_open_external;
	QAction* action_media_export;
	QAction* action_media_remove;
	QAction* action_media_replace;

	QAction* action_slideshow_start;
	QAction* action_slideshow_start_fullscreen;

	QAction* action_meta_settings;
	QAction* action_meta_about;
	QAction* action_meta_help;

	QAction* action_toggle_fullscreen;
	QAction* action_toggle_theme;
};
