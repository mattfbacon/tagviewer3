#pragma once

#include <QFile>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

enum class ConnectionStatus : unsigned char {
	OK,
	EXISTING_CONNECTION,
	ALREADY_CONNECTED,
	ERROR,
};

class TagSpace final : public QObject {
	Q_OBJECT
public:
	explicit TagSpace(QString const& db_dir, QString const& db_filename, QObject* const parent = nullptr);
	Q_DISABLE_COPY(TagSpace);
	~TagSpace();
	ConnectionStatus connect(bool force = false);
	QString const& get_db_uuid() const;
	bool is_connected() const;
	QSqlError last_db_error() const;
	QString get_db_file_path() const;
	QString get_db_lockfile_path() const;
protected:
	QString const db_file_path;
	QString const db_lockfile_path;
	QFile lockfile;
	QString const db_uuid;
	QSqlDatabase db;
	QSqlQuery query;

	void postconnect();
};
