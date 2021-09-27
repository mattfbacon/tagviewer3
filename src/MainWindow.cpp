#include <QApplication>
#include <QDesktopServices>
#include <QIcon>
#include <QList>
#include <QMessageBox>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QUrl>

#include "qfonticon.h"

#include "AboutDialog.hpp"
#include "Action.hpp"
#include "ActionBar.hpp"
#include "CenterPanel.hpp"
#include "DebugPanel.hpp"
#include "FileList.hpp"
#include "FiltersSidebar.hpp"
#include "MainWindow.hpp"
#include "Menu.hpp"
#include "PropertiesSidebar.hpp"
#include "QVariantType.hpp"
#include "Settings.hpp"
#include "SettingsDialog.hpp"
#include "StatusBar.hpp"

#include "icons.hpp"
#include "initptr.hpp"
#include "links.hpp"
#include "templates.hpp"
#include "themes.hpp"

static QUrl const help_link(Links::help);

static QIcon const* fullscreen_icons[2] = {
	&Icons::fullscreen_enter,
	&Icons::fullscreen_exit,
};

static QIcon const* dark_mode_icons[2] = {
	&Icons::theme_dark,
	&Icons::theme_light,
};

extern QStyle* original_style;

MainWindow::MainWindow()
	: QMainWindow{}
	, INIT_PTR(menu)
	, INIT_PTR(statusbar)
	, INIT_PTR(actionbar)
	, actionbar_dock{ tr("Toolbar"), }
	, INIT_PTR(file_list)
	, file_list_dock{ tr("Media"), }
	, INIT_PTR(props_sidebar)
	, INIT_PTR(props_sidebar_surrogate)
	, props_sidebar_dock{ tr("Properties"), }
	, INIT_PTR(filters_sidebar)
	, INIT_PTR(filters_sidebar_surrogate)
	, filters_sidebar_dock{ tr("Filters"), }
	, INIT_PTR(center_panel)
	, INIT_PTR(about_dialog, this)
	, INIT_PTR(settings_dialog, this)
	, INIT_PTR(debug_panel, this)
{
#pragma region Bars
	setMenuBar(menu);
	setStatusBar(statusbar);
#pragma endregion

	Icons::helper_palette = palette();

#pragma region Actionbar
	action_file_open = actionbar->addAction(Icons::file_open, tr("Open TagSpace"));

	action_file_new = actionbar->addAction(Icons::file_new, tr("New TagSpace"));

	action_file_reopen = actionbar->addAction(Icons::file_reopen, tr("Reopen TagSpace"));

	actionbar->addSeparator();

	action_go_first = actionbar->addAction(Icons::go_first, tr("Go to First"));

	action_go_prev = actionbar->addAction(Icons::go_prev, tr("Go to Previous"));

	action_go_next = actionbar->addAction(Icons::go_next, tr("Go to Next"));

	action_go_last = actionbar->addAction(Icons::go_last, tr("Go to Last"));

	actionbar->addSeparator();

	action_media_tagspace_edit = actionbar->addAction(Icons::media_tagspace_edit, tr("Configure TagSpace"));

	action_media_add = actionbar->addAction(Icons::media_add, tr("Add Media"));

	action_media_open_external = actionbar->addAction(Icons::media_open_external, tr("Open Media Externally"));

	action_media_export = actionbar->addAction(Icons::media_export, tr("Extract Current Media"));

	action_media_remove = actionbar->addAction(Icons::media_remove, tr("Remove Current Media"));

	action_media_replace = actionbar->addAction(Icons::media_replace, tr("Replace Current Media"));

	actionbar->addSeparator();

	action_slideshow_start = actionbar->addAction(Icons::slideshow_start, tr("Start Slideshow"));

	action_slideshow_start_fullscreen = actionbar->addAction(Icons::slideshow_start_fullscreen, tr("Start Slideshow (Fullscreen)"));

	actionbar->addSeparator();

	action_meta_settings = actionbar->addAction(Icons::meta_settings, tr("Settings"));
	QObject::connect(action_meta_settings, &QAction::triggered, this, &MainWindow::show_settings);

	action_meta_about = actionbar->addAction(Icons::meta_about, tr("About"));
	QObject::connect(action_meta_about, &QAction::triggered, this, &MainWindow::show_about);

	action_meta_help = actionbar->addAction(Icons::meta_help, tr("Help"));
	QObject::connect(action_meta_help, &QAction::triggered, []() {
		QDesktopServices::openUrl(help_link);
	});

	actionbar->addSeparator();

	action_toggle_fullscreen = actionbar->addAction(*fullscreen_icons[fullscreen()], tr("Toggle Fullscreen"));
	QObject::connect(action_toggle_fullscreen, &QAction::triggered, [this]() {
		MAINWINDOW_UPDATE_SLOT(fullscreen)(!fullscreen());
	});

	action_toggle_theme = actionbar->addAction(*dark_mode_icons[g_settings->dark_mode()], tr("Toggle Dark Mode"));
	QObject::connect(action_toggle_theme, &QAction::triggered, this, []() {
		g_settings->set_dark_mode(!g_settings->dark_mode());
	});
#pragma endregion

#pragma region Central Widget
	setCentralWidget(center_panel);
#pragma endregion

#pragma region Dock Widgets
	actionbar_dock.setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable /* | QDockWidget::DockWidgetVerticalTitleBar */);
	actionbar_dock.setWidget(actionbar);
	actionbar_dock.setObjectName(QStringLiteral("actionbar"));

	file_list_dock.setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	file_list_dock.setWidget(file_list);
	file_list_dock.setObjectName(QStringLiteral("file_list"));

	props_sidebar_surrogate->setLayout(props_sidebar);
	props_sidebar_dock.setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	props_sidebar_dock.setWidget(props_sidebar_surrogate);
	props_sidebar_dock.setObjectName(QStringLiteral("props_sidebar"));

	filters_sidebar_surrogate->setLayout(filters_sidebar);
	filters_sidebar_dock.setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	filters_sidebar_dock.setWidget(filters_sidebar_surrogate);
	filters_sidebar_dock.setObjectName(QStringLiteral("filters_sidebar"));

	addDockWidget(Qt::TopDockWidgetArea, &actionbar_dock);
	addDockWidget(Qt::LeftDockWidgetArea, &file_list_dock);
	addDockWidget(Qt::RightDockWidgetArea, &props_sidebar_dock);
	addDockWidget(Qt::RightDockWidgetArea, &filters_sidebar_dock);
	tabifyDockWidget(&props_sidebar_dock, &filters_sidebar_dock);
#pragma endregion

#pragma region Connections
	QObject::connect(this, &MainWindow::MAINWINDOW_UPDATE_SIGNAL(fullscreen), this, &MainWindow::update_action_fullscreen);
	QObject::connect(g_settings.get(), &Settings::SETTINGS_NOTIFY_SUFFIX(dark_mode), this, &MainWindow::MAINWINDOW_UPDATE_SLOT(dark_mode));

	QObject::connect(debug_panel, &DebugPanel::reset_style, this, []() {
		qApp->setStyle(original_style);
	});
#pragma endregion

#pragma region Actions
	QObject::connect(menu->action_show_debug_panel, &QAction::triggered, this, &MainWindow::show_debug_panel);
#pragma endregion

#pragma region Load Saved Config
	if (g_settings->save_state()) {
		restoreGeometry(g_settings->window_geometry());
		restoreState(g_settings->window_state());
	}
	update_dark_mode(g_settings->dark_mode());
#pragma endregion

#pragma region Window Setup
	setWindowTitle(tr("TagViewer"));
#pragma endregion

#pragma region Show Docks and Window
	show();
	// if they are floating, they won't be shown when the MainWindow is shown.
	for (QDockWidget* const widget : findChildren<QDockWidget*>()) {
		if (widget->isFloating()) {
			widget->show();
		}
	}
#pragma endregion
}

MainWindow::~MainWindow() {
	if (g_settings->save_state()) {
		g_settings->set_window_geometry(saveGeometry());
		g_settings->set_window_state(saveState());
	}
}

void MainWindow::changeEvent(QEvent* const event) {
	switch (event->type()) {
		case QEvent::WindowStateChange:
			emit MAINWINDOW_UPDATE_SIGNAL(fullscreen)(isFullScreen());
			break;
		case QEvent::PaletteChange:
			Icons::helper_palette = palette();
			break;
		default:
			break;
	}
	QMainWindow::changeEvent(event);
}

void MainWindow::MAINWINDOW_UPDATE_SLOT(dark_mode)(bool const is_dark_mode) {
	for (auto* const widget : QApplication::allWidgets()) {
		widget->setPalette(is_dark_mode ? Themes::dark : Themes::light);
	}
	update_action_dark_mode(is_dark_mode);
}

void MainWindow::MAINWINDOW_UPDATE_SLOT(fullscreen)(bool is_fullscreen) {
	auto* const window = this->window();
	window->setWindowState(window->windowState().setFlag(Qt::WindowFullScreen, is_fullscreen));
	// does not emit; see `changeEvent`
}

bool MainWindow::fullscreen() const {
	return isFullScreen();
}

#define UPDATE_SETTING_IMPL(TYPE, NAME) \
	void MainWindow::MAINWINDOW_UPDATE_SLOT(NAME)(TYPE state) { \
		settings.setValue(SETTING(NAME), state); \
		emit MAINWINDOW_UPDATE_SIGNAL(NAME)(state); \
	}

void MainWindow::clear_tagspace_history() {
	if (QMessageBox::warning(this, tr("Are you sure?"), tr("Are you sure you want to clear the TagSpace history?"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
		g_settings->set_tagspace_history(Settings::Defaults::tagspace_history());
	}
}

void MainWindow::show_settings() {
#ifdef QT_NO_DEBUG  // in debug, we want to know if something isn't getting updated properly
	settings_dialog->update_all_settings();
#endif
	settings_dialog->show();
}

void MainWindow::show_about() {
	about_dialog->show();
}

void MainWindow::show_debug_panel() {
	debug_panel->show();
}

void MainWindow::update_action_fullscreen(bool is_fullscreen) {
	action_toggle_fullscreen->setIcon(*(fullscreen_icons[is_fullscreen]));
}

void MainWindow::update_action_dark_mode(bool is_dark_mode) {
	action_toggle_theme->setIcon(*(dark_mode_icons[is_dark_mode]));
}
