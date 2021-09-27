#include <QDataStream>

#include "TagSpace/Prop.hpp"

Prop::Prop(QString name, QVariantType type)
	: name(name)
	, type(type) {}

Prop::Prop(Prop const& other)
	: name(other.name)
	, type(other.type) {}
Prop::Prop(Prop&& other)
	: name(std::move(other.name))
	, type(std::move(other.type)) {}
Prop& Prop::operator=(Prop const& other) {
	Prop copy(other);
	std::swap(*this, copy);
	return *this;
}

QDataStream& operator<<(QDataStream& out, Prop const& x) {
	out << x.name << x.type;
	return out;
}

QDataStream& operator>>(QDataStream& in, Prop& x) {
	in >> x.name >> x.type;
	return in;
}
