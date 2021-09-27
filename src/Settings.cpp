#include "Settings.hpp"

#define LOAD_SIMPLE_SETTING(NAME) \
	if (proxied_settings.contains(Settings::Names::NAME)) { \
		SETTINGS_MEMBER_PREFIX(NAME) = proxied_settings.value(Settings::Names::NAME).value<std::invoke_result_t<decltype(Settings::Defaults::NAME)>>(); \
	}
#define LOAD_ARRAY_SETTING(NAME) \
	{ \
		proxied_settings.beginGroup(Settings::Names::NAME); \
		bool const key_exists = proxied_settings.contains(QStringLiteral("size")); \
		if (key_exists) { \
			auto const num_items = proxied_settings.value(QStringLiteral("size")).value<size_t>(); \
			SETTINGS_MEMBER_PREFIX(NAME) = {}; \
			SETTINGS_MEMBER_PREFIX(NAME).reserve(num_items); \
			QString item_name; \
			for (std::remove_const_t<decltype(num_items)> i = 0; i < num_items; i++) { \
				item_name.setNum(i); \
				SETTINGS_MEMBER_PREFIX(NAME) << proxied_settings.value(item_name).value<std::invoke_result_t<decltype(Settings::Defaults::NAME)>>(); \
			} \
		} \
		proxied_settings.endGroup(); \
	}

Settings::Settings(QObject* const parent)
	: QObject{ parent } {
	load();
}

// defaults are lazy-loaded to avoid putting the default into the user's settings unless it's necessary.
#define GETTER_IMPL(NAME, SHARED_TYPE) \
	SHARED_TYPE Settings::NAME() { \
		if (!SETTINGS_MEMBER_PREFIX(NAME).has_value()) { \
			SETTINGS_MEMBER_PREFIX(NAME) = std::optional{ Settings::Defaults::NAME() }; \
		} \
		return *SETTINGS_MEMBER_PREFIX(NAME); \
	}
#define SETTER_IMPL(NAME, SHARED_TYPE) \
	void Settings::SETTINGS_SET_PREFIX(NAME)(SHARED_TYPE value) { \
		SETTINGS_MEMBER_PREFIX(NAME) = std::optional{ value }; \
		emit SETTINGS_NOTIFY_SUFFIX(NAME)(*SETTINGS_MEMBER_PREFIX(NAME)); \
	}
#define GETSET_IMPL(NAME, SHARED_TYPE) GETTER_IMPL(NAME, SHARED_TYPE) SETTER_IMPL(NAME, SHARED_TYPE)
#undef PROP
#define PROP(NAME, STORED_TYPE, SHARED_TYPE, DEFAULT_VALUE, STORED_NAME) GETSET_IMPL(NAME, SHARED_TYPE)
#include "settings_prop_list.hpp"

void Settings::load() {
	QMutexLocker group_entry_lock{ &group_entry_mutex };

#undef PROP
#define PROP(NAME, STORED_TYPE, SHARED_TYPE, DEFAULT_VALUE, STORED_NAME) LOAD_SIMPLE_SETTING(NAME)
#include "settings_prop_list.hpp"
}

Settings::~Settings() {
	commit();
}

#define SAVER_IMPL_SIMPLE(NAME) \
	if (SETTINGS_MEMBER_PREFIX(NAME).has_value()) { \
		proxied_settings.setValue(Settings::Names::NAME, QVariant::fromValue(*SETTINGS_MEMBER_PREFIX(NAME))); \
	} else { \
		proxied_settings.remove(Settings::Names::NAME); /* jic */ \
	}
void Settings::commit() {
	QMutexLocker group_entry_lock{ &group_entry_mutex };

#undef PROP
#define PROP(NAME, STORED_TYPE, SHARED_TYPE, DEFAULT_VALUE, STORED_NAME) SAVER_IMPL_SIMPLE(NAME)
#include "settings_prop_list.hpp"
}

#undef PROP
#define PROP(NAME, TYPE, SHARED_TYPE, DEFAULT_VALUE, STORED_NAME) QString const Settings::Names::NAME = QStringLiteral(STORED_NAME);
#include "settings_prop_list.hpp"

#undef PROP
#define PROP(NAME, TYPE, SHARED_TYPE, DEFAULT_VALUE, STORED_NAME) \
	TYPE Settings::Defaults::NAME() { \
		return DEFAULT_VALUE; \
	}
#include "settings_prop_list.hpp"
