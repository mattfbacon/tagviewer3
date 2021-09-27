#pragma once

class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QTreeView;
class QStackedWidget;
class QSettings;
class QShowEvent;
class QCheckBox;
class QPushButton;
class QSpinBox;
class QComboBox;
class QLineEdit;
class QString;
class QColor;
class QPlainTextEdit;
class SettingsTreeView;
class TagSpaceConfig;
class ColorPicker;

#include <QDialog>
#include <QFormLayout>
#include <QObject>
#include <QStandardItemModel>

#include "ComboBoxHelpers.hpp"
#include "Settings.hpp"
#include "TagSpaceConfig/TagSpaceConfig.hpp"

#define PAGE(NAME) page_##NAME
#define PAGE_SURROGATE(NAME) page_surrogate_##NAME
#define INPUT(NAME) input_##NAME
#define ITEM(NAME) item_##NAME

#define DECLARE_CUSTOM_PAGE(NAME, TYPE) \
protected: \
	TYPE* const PAGE(NAME); \
	QWidget PAGE_SURROGATE(NAME)
#define DECLARE_PAGE(NAME) DECLARE_CUSTOM_PAGE(NAME, QFormLayout)
#define DECLARE_INPUT(NAME, TYPE) \
protected: \
	TYPE* const INPUT(NAME)

#define SETTING_SLOT(NAME) update_##NAME
#define ACTION_SIGNAL(NAME) NAME##_triggered

#define DECLARE_SETTING_SLOT(NAME, ...) \
protected: \
	Q_SLOT void SETTING_SLOT(NAME)(__VA_ARGS__)
#define DECLARE_ACTION_SIGNAL(NAME) \
public: \
	Q_SIGNAL void ACTION_SIGNAL(NAME)()

#define DECLARE_SETTING_AND_INPUT(NAME, INPUT_TYPE, ...) \
	DECLARE_SETTING_SLOT(NAME, __VA_ARGS__); \
	DECLARE_INPUT(NAME, INPUT_TYPE)
#define DECLARE_ACTION_AND_INPUT(NAME, INPUT_TYPE) \
	DECLARE_ACTION_SIGNAL(NAME); \
	DECLARE_INPUT(NAME, INPUT_TYPE)

class SettingsDialog final : public QDialog {
	Q_OBJECT
public:
	explicit SettingsDialog(QWidget* parent = nullptr);
	~SettingsDialog();
public slots:
	void show();  // override of non-virtual
	void update_all_settings();
protected slots:
	void select_page(QModelIndex const& index);
protected:
	QHBoxLayout* const layout;

	QStandardItemModel model;

	SettingsTreeView* const section_selection;
	QStackedWidget* const section_stack;

	DECLARE_PAGE(behavior);
	DECLARE_PAGE(history);
	DECLARE_PAGE(slideshow);
	DECLARE_CUSTOM_PAGE(tagspace_defaults, QVBoxLayout);
	DECLARE_PAGE(tag_defaults);
	DECLARE_PAGE(prop_defaults);
	DECLARE_PAGE(interface);
	DECLARE_CUSTOM_PAGE(custom_css, QVBoxLayout);

	static inline bool checkstate_as_bool(int const state) {
		return state == Qt::Checked;
	}
	static inline Qt::CheckState bool_as_checkstate(bool const state) {
		return state ? Qt::Checked : Qt::Unchecked;
	}
	static int constexpr DEBOUNCE_TIME = 500;
protected:  // settings
	DECLARE_SETTING_AND_INPUT(save_state, QCheckBox, bool);
	DECLARE_ACTION_AND_INPUT(reset_layout, QPushButton);
	DECLARE_SETTING_AND_INPUT(dark_mode, QCheckBox, bool);
	DECLARE_SETTING_AND_INPUT(save_tagspace_history, QCheckBox, bool);
	DECLARE_ACTION_AND_INPUT(clear_tagspace_history, QPushButton);
	DECLARE_SETTING_AND_INPUT(auto_reopen, QCheckBox, bool);
	DECLARE_SETTING_AND_INPUT(slideshow_interval, QSpinBox, int);
	DECLARE_SETTING_AND_INPUT(slideshow_end_on_fullscreen_exit, QCheckBox, bool);
	DECLARE_SETTING_AND_INPUT(slideshow_wrap_around, QComboBox, bool);
	DECLARE_SETTING_AND_INPUT(custom_css, QPlainTextEdit, QString const&);
	DECLARE_SETTING_AND_INPUT(new_tag_name, QLineEdit, QString const&);
	DECLARE_SETTING_AND_INPUT(new_tag_color, ColorPicker, QColor const&);
	DECLARE_SETTING_AND_INPUT(new_prop_name, QLineEdit, QString const&);
	DECLARE_SETTING_AND_INPUT(new_prop_type, ComboBoxHelpers::NewPropType::ComboBox, QVariantType);

	// DECLARE_INPUT(tagspace_defaults, TagSpaceConfig);
	// DECLARE_SETTING_SLOT(tagspace_default_tags, TagsList const&);
	// DECLARE_SETTING_SLOT(tagspace_default_props, PropsList const&);
};
