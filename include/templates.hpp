#pragma once

#include <QVariantList>

template <typename T, template <typename> typename List>
QVariantList list_as_variant(List<T> const& list) {
	QVariantList new_list;
	for (T const& item : list) {
		new_list << item;
	}
	return new_list;
}

template <typename T>
QList<T> variant_as_list(QVariantList const& vlist) {
	QList<T> new_list;
	for (QVariant const& item : vlist) {
		if (item.canConvert<T>()) {
			new_list << item.value<T>();
		}
	}
	return new_list;
}
