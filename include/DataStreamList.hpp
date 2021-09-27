#pragma once

#include <QDataStream>

template <typename T>
QDataStream& operator<<(QDataStream& out, T const& x) {
	out << x.size();
	for (auto const& item : x) {
		out << item;
	}
	return out;
}
template <typename T>
QDataStream& operator>>(QDataStream& in, T& x) {
	int size;
	in >> size;
	x.reserve(size);
	for (int i = 0; i < size; i++) {
		in >> x[i];
	}
	return in;
}
