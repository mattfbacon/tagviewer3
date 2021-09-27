#pragma once

class QDataStream;

#include <QColor>
#include <QObject>
#include <QString>

class Tag final : public QObject {
	Q_OBJECT
public:
	Tag() = default;
	Tag(Tag const& other);
	Tag(Tag&& other);
	Tag& operator=(Tag const& other);
	Tag(QString name, QColor color, qint64 id = -1);
	QString name;
	QColor color;
	// -1 = no ID
	qint64 id = -1;
};

Q_DECLARE_METATYPE(Tag)

QDataStream& operator<<(QDataStream& out, Tag const& x);
QDataStream& operator>>(QDataStream& in, Tag& x);
