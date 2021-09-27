#pragma once

class QDataStream;

#include <QObject>
#include <QString>

#include "QVariantType.hpp"

class Prop final : public QObject {
	Q_OBJECT
public:
	Prop() = default;
	Prop(Prop const& other);
	Prop(Prop&& other);
	Prop& operator=(Prop const& other);
	Prop(QString name, QVariantType type);
	QString name;
	QVariantType type;
};

Q_DECLARE_METATYPE(Prop)

QDataStream& operator<<(QDataStream& out, Prop const& x);
QDataStream& operator>>(QDataStream& in, Prop& x);
