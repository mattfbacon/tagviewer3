#pragma once

#include <optional>

#include <QColor>
#include <QMutex>
#include <QObject>
#include <QSettings>

#include "TagSpaceConfig/TagSpaceConfig.hpp"

#define SETTINGS_MEMBER_PREFIX(NAME) m_##NAME
#define SETTINGS_SET_PREFIX(NAME) set_##NAME
#define SETTINGS_NOTIFY_SUFFIX(NAME) NAME##_changed

namespace SettingsImpl {
template <typename Unit, Unit A, Unit B>
using is_less_or_equal = std::bool_constant<A <= B>;
template <typename T>
using would_value_be_better = std::conjunction<std::is_trivially_copyable<T>, is_less_or_equal<size_t, sizeof(std::decay_t<T>), sizeof(void*)>>;
template <typename T>
using auto_value_or_cref = std::conditional_t<would_value_be_better<T>::value, T, T const&>;
}  // namespace SettingsImpl

#define SETTINGS_BASIC_PROP(NAME, TYPE, SHARED_TYPE) \
public: \
	Q_PROPERTY(TYPE NAME READ NAME WRITE SETTINGS_SET_PREFIX(NAME) NOTIFY SETTINGS_NOTIFY_SUFFIX(NAME) DESIGNABLE false) \
public: \
	SHARED_TYPE NAME(); \
	Q_SLOT void SETTINGS_SET_PREFIX(NAME)(SHARED_TYPE); \
	Q_SIGNAL void SETTINGS_NOTIFY_SUFFIX(NAME)(SHARED_TYPE); \
protected: \
	std::optional<TYPE> SETTINGS_MEMBER_PREFIX(NAME)

class Settings : public QObject {
	Q_OBJECT
public:
#define PROP(NAME, STORED_TYPE, SHARED_TYPE, DEFAULT_VALUE, STORED_NAME) SETTINGS_BASIC_PROP(NAME, STORED_TYPE, SHARED_TYPE)
#include "settings_prop_list.hpp"
public:
	explicit Settings(QObject* parent = nullptr);
	~Settings();
public slots:
	void load();
	void commit();
protected:
	QSettings proxied_settings;
	QMutex group_entry_mutex;

public:
	struct Names {
#undef PROP
#define PROP(NAME, STORED_TYPE, SHARED_TYPE, DEFAULT_VALUE, STORED_NAME) static QString const NAME
#include "settings_prop_list.hpp"
	};
	struct Defaults {
#undef PROP
#define PROP(NAME, STORED_TYPE, SHARED_TYPE, DEFAULT_VALUE, STORED_NAME) static STORED_TYPE NAME()
#include "settings_prop_list.hpp"
	};
};

extern std::unique_ptr<Settings> g_settings;
