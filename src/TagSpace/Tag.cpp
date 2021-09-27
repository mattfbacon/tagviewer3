#include <QDataStream>

#include "TagSpace/Tag.hpp"

Tag::Tag(QString name, QColor color, qint64 id /* = -1 */)
	: name(name)
	, color(color)
	, id(id) {}

Tag::Tag(Tag const& other)
	: name(other.name)
	, color(other.color)
	, id(other.id) {}
Tag::Tag(Tag&& other)
	: name(std::move(other.name))
	, color(std::move(other.color))
	, id(std::move(other.id)) {}
Tag& Tag::operator=(Tag const& other) {
	Tag copy(other);
	std::swap(*this, copy);
	return *this;
}

QDataStream& operator<<(QDataStream& out, Tag const& x) {
	out << x.name << x.color << x.id;
	return out;
}

QDataStream& operator>>(QDataStream& in, Tag& x) {
	in >> x.name >> x.color >> x.id;
	return in;
}
