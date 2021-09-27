#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QItemSelection>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QVariant>
#include <QWidget>

#include "ColorPicker.h"
#include "qfunctionutils.h"

#include "ComboBoxHelpers.hpp"
#include "QVariantType.hpp"
#include "Settings.hpp"
#include "SettingsDialog.hpp"
#include "SettingsTreeView.hpp"
#include "TagSpaceConfig/TagSpaceConfig.hpp"

#include "icons.hpp"
#include "initptr.hpp"
#include "pragmas.hpp"
#include "templates.hpp"

int constexpr BIG_NUMBER = 1000;  // for the purpose of calculating desired tab width

using sneaky_pointer_t = quintptr;

static QString const embolden_str = QStringLiteral("<b>%0</b>");
#define embolden(STR) (embolden_str.arg(STR))

#define INIT_PAGE(NAME) INIT_PTR(PAGE(NAME))
#define ADD_CUSTOM_PAGE(NAME, DISPLAY_NAME, ADD_WIDGET_METHOD) \
	PAGE_SURROGATE(NAME).setLayout(PAGE(NAME)); \
	section_stack->addWidget(&PAGE_SURROGATE(NAME)); \
	QStandardItem* const ITEM(NAME) = new QStandardItem{ tr(DISPLAY_NAME) }; \
	ITEM(NAME)->setData(QVariant::fromValue(reinterpret_cast<sneaky_pointer_t>(&PAGE_SURROGATE(NAME))), Qt::UserRole + 1); \
	PAGE(NAME)->ADD_WIDGET_METHOD(new QLabel{ embolden(tr(DISPLAY_NAME)) })
#define ADD_PAGE(NAME, DISPLAY_NAME) \
	ADD_CUSTOM_PAGE(NAME, DISPLAY_NAME, addRow); \
	PAGE(NAME)->setRowWrapPolicy(QFormLayout::WrapLongRows);

#define INIT_INPUT(INPUT_NAME, ...) INIT_PTR(INPUT(INPUT_NAME), __VA_ARGS__)
#define ADD_INPUT(PAGE_NAME, INPUT_NAME, ...) PAGE(PAGE_NAME)->addRow(tr(__VA_ARGS__), INPUT(INPUT_NAME))

#define SUBPAGE(PARENT, CHILD) item_##PARENT->appendRow(item_##CHILD)
#define TOPLEVEL_PAGE(PAGE) model.appendRow(item_##PAGE)

SettingsDialog::SettingsDialog(QWidget*const parent)
	: QDialog{ parent, }
	, INIT_PTR(layout)
	, INIT_PTR(section_selection, &model)
	, INIT_PTR(section_stack)
	, INIT_PAGE(behavior)
	, INIT_PAGE(history)
	, INIT_PAGE(slideshow)
	, INIT_PAGE(tagspace_defaults)
	, INIT_PAGE(tag_defaults)
	, INIT_PAGE(prop_defaults)
	, INIT_PAGE(interface)
	, INIT_PAGE(custom_css)
	, INIT_INPUT(save_state)
	, INIT_INPUT(reset_layout, tr("Reset"))
	, INIT_INPUT(dark_mode)
	, INIT_INPUT(save_tagspace_history)
	, INIT_INPUT(clear_tagspace_history, tr("Clear"))
	, INIT_INPUT(auto_reopen)
	, INIT_INPUT(slideshow_interval)
	, INIT_INPUT(slideshow_end_on_fullscreen_exit)
	, INIT_INPUT(slideshow_wrap_around)
	, INIT_INPUT(custom_css)
	, INIT_INPUT(new_tag_name)
	, INIT_INPUT(new_tag_color)
	, INIT_INPUT(new_prop_name)
	, INIT_INPUT(new_prop_type)
	// , INIT_INPUT(tagspace_defaults)
{
	ADD_PAGE(behavior, "Behavior");
	ADD_PAGE(history, "History");
	ADD_PAGE(slideshow, "Slideshow");
	ADD_CUSTOM_PAGE(tagspace_defaults, "TagSpace Defaults", addWidget);
	ADD_PAGE(tag_defaults, "Default Tags");
	ADD_PAGE(prop_defaults, "Default Props");
	ADD_PAGE(interface, "Interface");
	ADD_CUSTOM_PAGE(custom_css, "Custom CSS", addWidget);

	INPUT(slideshow_interval)->setRange(1, std::numeric_limits<int>::max());

	INPUT(slideshow_wrap_around)->addItems(QStringList{ tr("End slideshow"), tr("Wrap around") });
	INPUT(slideshow_wrap_around)->setInsertPolicy(QComboBox::NoInsert);

	INPUT(custom_css)->setSizePolicy(QSizePolicy{ QSizePolicy::Expanding, QSizePolicy::Expanding, QSizePolicy::Frame });
	INPUT(custom_css)->setLineWrapMode(QPlainTextEdit::NoWrap);
	INPUT(custom_css)->setPlaceholderText(tr("Enter CSS here"));
	INPUT(custom_css)->setFont(QFont{ QStringLiteral("monospace") });
	INPUT(custom_css)->setTabStopDistance(static_cast<qreal>(INPUT(custom_css)->fontMetrics().horizontalAdvance(QStringLiteral("  ").repeated(BIG_NUMBER))) / static_cast<qreal>(BIG_NUMBER));
	INPUT(custom_css)->setTextInteractionFlags(Qt::TextEditorInteraction);
	INPUT(custom_css)->setUndoRedoEnabled(true);

	ADD_INPUT(interface, save_state, "Persist the window layout?");
	ADD_INPUT(interface, reset_layout, "Reset window layout");
	ADD_INPUT(interface, dark_mode, "Dark mode");
	ADD_INPUT(history, save_tagspace_history, "Save TagSpace history?");
	ADD_INPUT(history, clear_tagspace_history, "Clear TagSpace history");
	ADD_INPUT(history, auto_reopen, "Reopen on startup?");
	ADD_INPUT(slideshow, slideshow_interval, "Item duration (milliseconds)");
	ADD_INPUT(slideshow, slideshow_end_on_fullscreen_exit, "End on fullscreen exit?");
	ADD_INPUT(slideshow, slideshow_wrap_around, "Action at end");
	PAGE(custom_css)->addWidget(INPUT(custom_css));
	// PAGE(tagspace_defaults)->addWidget(INPUT(tagspace_defaults));
	ADD_INPUT(tag_defaults, new_tag_name, "Name", "tag default");
	ADD_INPUT(tag_defaults, new_tag_color, "Color", "tag default");
	ADD_INPUT(prop_defaults, new_prop_name, "Name", "prop default");
	ADD_INPUT(prop_defaults, new_prop_type, "Type", "prop default");

	update_all_settings();

#define SETTING_SIGNAL_IMPL_CHECKBOX(NAME) \
	QObject::connect(INPUT(NAME), &QCheckBox::stateChanged, [](int const state) { \
		g_settings->set_##NAME(checkstate_as_bool(state)); \
	}); \
	QObject::connect(g_settings.get(), &Settings::NAME##_changed, [this](bool const state) { \
		INPUT(NAME)->setCheckState(bool_as_checkstate(state)); \
	})
#define SETTING_SIGNAL_IMPL_SPINBOX(NAME) \
	QObject::connect( \
		INPUT(NAME), \
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), \
		QFunctionUtils::Debounce( \
			[](int const value) { \
				g_settings->set_##NAME(value); \
			}, \
			DEBOUNCE_TIME)); \
	QObject::connect(g_settings.get(), &Settings::NAME##_changed, INPUT(NAME), &QSpinBox::setValue)
#define SETTING_SIGNAL_IMPL_PUSHBUTTON(NAME) QObject::connect(INPUT(NAME), &QPushButton::clicked, this, &SettingsDialog::ACTION_SIGNAL(NAME))
#define SETTING_SIGNAL_IMPL_LINEEDIT(NAME) \
	QObject::connect(INPUT(NAME), &QLineEdit::textChanged, g_settings.get(), &Settings::set_##NAME); \
	QObject::connect(g_settings.get(), &Settings::NAME##_changed, INPUT(NAME), &QLineEdit::setText)
#define SETTING_SIGNAL_IMPL_COMBOBOX(NAME, FROM_SAVED, TO_SAVED) \
	QObject::connect(INPUT(NAME), static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [](int const index) { \
		g_settings->set_##NAME(TO_SAVED(index)); \
	}); \
	QObject::connect(g_settings.get(), &Settings::NAME##_changed, [this](std::invoke_result_t<decltype(TO_SAVED), int> const state) { \
		INPUT(NAME)->setCurrentIndex(FROM_SAVED(state)); \
	})
#define SETTING_SIGNAL_IMPL_COLORPICKER(NAME) \
	QObject::connect(INPUT(NAME), &ColorPicker::color_changed, g_settings.get(), &Settings::set_##NAME); \
	QObject::connect(g_settings.get(), &Settings::NAME##_changed, INPUT(NAME), &ColorPicker::set_color);

	SETTING_SIGNAL_IMPL_CHECKBOX(save_state);
	SETTING_SIGNAL_IMPL_CHECKBOX(dark_mode);
	SETTING_SIGNAL_IMPL_CHECKBOX(save_tagspace_history);
	SETTING_SIGNAL_IMPL_PUSHBUTTON(clear_tagspace_history);
	SETTING_SIGNAL_IMPL_CHECKBOX(auto_reopen);
	SETTING_SIGNAL_IMPL_SPINBOX(slideshow_interval);
	SETTING_SIGNAL_IMPL_CHECKBOX(slideshow_end_on_fullscreen_exit);
	SETTING_SIGNAL_IMPL_COMBOBOX(slideshow_wrap_around, ComboBoxHelpers::SlideshowWrapAround::from_saved, ComboBoxHelpers::SlideshowWrapAround::to_saved);
	QObject::connect(
		INPUT(custom_css),
		&QPlainTextEdit::textChanged,
		QFunctionUtils::Debounce(
			[this]() {
				g_settings->set_custom_css(INPUT(custom_css)->toPlainText());
			},
			1000));
	QObject::connect(g_settings.get(), &Settings::custom_css_changed, INPUT(custom_css), &QPlainTextEdit::setPlainText);
	SETTING_SIGNAL_IMPL_LINEEDIT(new_tag_name);
	SETTING_SIGNAL_IMPL_COLORPICKER(new_tag_color);
	SETTING_SIGNAL_IMPL_LINEEDIT(new_prop_name);
	SETTING_SIGNAL_IMPL_COMBOBOX(new_prop_type, ComboBoxHelpers::NewPropType::from_saved, ComboBoxHelpers::NewPropType::to_saved);
	// QObject::connect(INPUT(tagspace_defaults), &TagSpaceConfig::tags_changed, [this]() {
	// 	g_settings->set_tagspace_default_tags(INPUT(tagspace_defaults)->get_tags());
	// });
	// QObject::connect(INPUT(tagspace_defaults), &TagSpaceConfig::props_changed, [this]() {
	// 	g_settings->set_tagspace_default_props(INPUT(tagspace_defaults)->get_props());
	// });

	SUBPAGE(tagspace_defaults, tag_defaults);
	SUBPAGE(tagspace_defaults, prop_defaults);
	SUBPAGE(behavior, history);
	SUBPAGE(behavior, slideshow);
	SUBPAGE(behavior, tagspace_defaults);
	SUBPAGE(interface, custom_css);
	TOPLEVEL_PAGE(behavior);
	TOPLEVEL_PAGE(interface);

	QObject::connect(section_selection, &SettingsTreeView::section_selected, this, &SettingsDialog::select_page);
	section_selection->expandAll();

	layout->addWidget(section_selection, 0);
	layout->addWidget(section_stack, 1);
	setLayout(layout);
	setAttribute(Qt::WA_DeleteOnClose, false);
	setWindowTitle(tr("Settings"));
}

SettingsDialog::~SettingsDialog() {
	//
}

#define USE_SETTING_SLOT_IMPL(NAME) SETTING_SLOT(NAME)(g_settings->NAME())

void SettingsDialog::update_all_settings() {
	USE_SETTING_SLOT_IMPL(dark_mode);
	USE_SETTING_SLOT_IMPL(save_state);
	USE_SETTING_SLOT_IMPL(save_tagspace_history);
	USE_SETTING_SLOT_IMPL(auto_reopen);
	USE_SETTING_SLOT_IMPL(slideshow_interval);
	USE_SETTING_SLOT_IMPL(slideshow_end_on_fullscreen_exit);
	USE_SETTING_SLOT_IMPL(slideshow_wrap_around);
	USE_SETTING_SLOT_IMPL(custom_css);
	USE_SETTING_SLOT_IMPL(new_tag_name);
	USE_SETTING_SLOT_IMPL(new_tag_color);
	USE_SETTING_SLOT_IMPL(new_prop_name);
	USE_SETTING_SLOT_IMPL(new_prop_type);
}

#define ASSERT_SETTING_CONVERTER(NAME, GETTER, TO_SAVED) Q_ASSERT(TO_SAVED(INPUT(NAME)->GETTER()) == g_settings->NAME())
#define ASSERT_SETTING_GENERIC(NAME, GETTER) ASSERT_SETTING_CONVERTER(NAME, GETTER, )
#define ASSERT_SETTING_CHECKBOX(NAME) ASSERT_SETTING_CONVERTER(NAME, checkState, checkstate_as_bool)
#define ASSERT_SETTING_COMBOBOX(NAME, TO_SAVED) ASSERT_SETTING_CONVERTER(NAME, currentIndex, TO_SAVED)
#define ASSERT_SETTING_SPINBOX(NAME) ASSERT_SETTING_GENERIC(NAME, value)
#define ASSERT_SETTING_PLAINTEXTEDIT(NAME) ASSERT_SETTING_GENERIC(NAME, toPlainText)
#define ASSERT_SETTING_LINEEDIT(NAME) ASSERT_SETTING_GENERIC(NAME, text)
#define ASSERT_SETTING_COLORPICKER(NAME) ASSERT_SETTING_GENERIC(NAME, color)

void SettingsDialog::show() {
	ASSERT_SETTING_CHECKBOX(dark_mode);
	ASSERT_SETTING_CHECKBOX(save_state);
	ASSERT_SETTING_CHECKBOX(save_tagspace_history);
	ASSERT_SETTING_CHECKBOX(auto_reopen);
	ASSERT_SETTING_SPINBOX(slideshow_interval);
	ASSERT_SETTING_CHECKBOX(slideshow_end_on_fullscreen_exit);
	ASSERT_SETTING_COMBOBOX(slideshow_wrap_around, ComboBoxHelpers::SlideshowWrapAround::to_saved);
	ASSERT_SETTING_PLAINTEXTEDIT(custom_css);
	ASSERT_SETTING_LINEEDIT(new_tag_name);
	ASSERT_SETTING_COLORPICKER(new_tag_color);
	ASSERT_SETTING_LINEEDIT(new_prop_name);
	ASSERT_SETTING_COMBOBOX(new_prop_type, ComboBoxHelpers::NewPropType::to_saved);
	// Q_ASSERT(INPUT(tagspace_defaults)->get_tags() == g_settings->tagspace_default_tags());
	// Q_ASSERT(INPUT(tagspace_defaults)->get_props() == g_settings->tagspace_default_props());
	QDialog::show();
}

#define SETTING_SLOT_IMPL_CONVERTER(NAME, TYPE, GETTER, FROM_SAVED, SETTER, TO_SAVED) \
	void SettingsDialog::SETTING_SLOT(NAME)(TYPE value) { \
		Q_ASSERT(value == g_settings->NAME()); \
		if (value == TO_SAVED(INPUT(NAME)->GETTER())) { \
			return; \
		} \
		INPUT(NAME)->SETTER(FROM_SAVED(value)); \
	}
#define SETTING_SLOT_IMPL_GENERIC(NAME, TYPE, GETTER, SETTER) SETTING_SLOT_IMPL_CONVERTER(NAME, TYPE, GETTER, , SETTER, )
#define SETTING_SLOT_IMPL_CHECKBOX(NAME) SETTING_SLOT_IMPL_CONVERTER(NAME, bool, checkState, bool_as_checkstate, setCheckState, checkstate_as_bool)
#define SETTING_SLOT_IMPL_SPINBOX(NAME) SETTING_SLOT_IMPL_GENERIC(NAME, int, value, setValue)
#define SETTING_SLOT_IMPL_LINEEDIT(NAME) SETTING_SLOT_IMPL_GENERIC(NAME, QString const&, text, setText)
#define SETTING_SLOT_IMPL_PLAINTEXTEDIT(NAME) SETTING_SLOT_IMPL_GENERIC(NAME, QString const&, toPlainText, setPlainText)
#define SETTING_SLOT_IMPL_COLORPICKER(NAME) SETTING_SLOT_IMPL_GENERIC(NAME, QColor const&, color, set_color)
#define SETTING_SLOT_IMPL_COMBOBOX_WITH_CONVERTER(NAME, SAVED_TYPE, FROM_SAVED, TO_SAVED) \
	SETTING_SLOT_IMPL_CONVERTER(NAME, SAVED_TYPE, currentIndex, FROM_SAVED, setCurrentIndex, TO_SAVED);

#define SETTING_SLOT_IMPL_STUB(NAME, ...) \
	DO_PRAGMA(message("stubbing " #NAME)); \
	void SettingsDialog::SETTING_SLOT(NAME)(__VA_ARGS__) {}

SETTING_SLOT_IMPL_CHECKBOX(dark_mode);
SETTING_SLOT_IMPL_CHECKBOX(save_state);
SETTING_SLOT_IMPL_CHECKBOX(save_tagspace_history);
SETTING_SLOT_IMPL_CHECKBOX(auto_reopen);
SETTING_SLOT_IMPL_SPINBOX(slideshow_interval);
SETTING_SLOT_IMPL_CHECKBOX(slideshow_end_on_fullscreen_exit);
SETTING_SLOT_IMPL_COMBOBOX_WITH_CONVERTER(slideshow_wrap_around, bool, ComboBoxHelpers::SlideshowWrapAround::from_saved, ComboBoxHelpers::SlideshowWrapAround::to_saved);
SETTING_SLOT_IMPL_PLAINTEXTEDIT(custom_css);
SETTING_SLOT_IMPL_LINEEDIT(new_tag_name);
SETTING_SLOT_IMPL_COLORPICKER(new_tag_color);
SETTING_SLOT_IMPL_LINEEDIT(new_prop_name);
SETTING_SLOT_IMPL_COMBOBOX_WITH_CONVERTER(new_prop_type, QVariantType, ComboBoxHelpers::NewPropType::from_saved, ComboBoxHelpers::NewPropType::to_saved);

void SettingsDialog::select_page(QModelIndex const& index) {
	section_stack->setCurrentWidget(reinterpret_cast<QWidget*>(index.data(Qt::UserRole + 1).value<sneaky_pointer_t>()));
}
