#include <QFile>
#include <QSqlError>
#include <QString>
#include <QUuid>

#include "TagSpace/TagSpace.hpp"

static QString const DB_TYPE = QStringLiteral("QSQLITE");

#define TABLE_NAME_MEDIA "media"
#define TABLE_NAME_KEYWORDS "keywords"
#define TABLE_NAME_JOIN_MEDIA_KEYWORDS "join_media_keywords"

#define TABLE_SCHEMA_MEDIA "media_id INTEGER PRIMARY KEY AUTOINCREMENT, "

static QString const LOCK_PREFIX = QStringLiteral("~");

TagSpace::TagSpace(QString const& db_dir, QString const& db_filename, QObject*const parent)
	: QObject{ parent, }
	, db_file_path(QStringList{ db_dir, db_filename, }.join(QChar::fromLatin1('/')))
	, db_lockfile_path(QStringList{ db_dir, LOCK_PREFIX + db_filename, }.join(QChar::fromLatin1('/')))
	, lockfile{ db_lockfile_path, }
	, db_uuid(QUuid::createUuid().toString(QUuid::WithoutBraces))
	, db(QSqlDatabase::addDatabase(DB_TYPE, db_uuid)) // protect with UUID in case of multiple connections
	, query{ db, }
{
	db.setDatabaseName(QStringList{ db_dir, db_filename }.join(QChar::fromLatin1('/')));
	db.setNumericalPrecisionPolicy(QSql::LowPrecisionDouble);  // ints as ints, others as doubles
}

TagSpace::~TagSpace() {
	// `db.close()` called by QSqlDatabase destructor
}

ConnectionStatus TagSpace::connect(bool force) {
	// we assume that the directory exists and is RWX for the user
	if (!lockfile.open(QIODevice::WriteOnly | (force ? static_cast<QIODevice::OpenModeFlag>(0) : QIODevice::NewOnly))) {
		return ConnectionStatus::EXISTING_CONNECTION;
	}
	if (is_connected()) {
		return ConnectionStatus::ALREADY_CONNECTED;
	}
	if (!db.open()) {
		return ConnectionStatus::ERROR;
	}
	postconnect();
	return ConnectionStatus::OK;
}

QString const& TagSpace::get_db_uuid() const {
	return db_uuid;
}

bool TagSpace::is_connected() const {
	return db.isValid() && db.isOpen() && lockfile.isOpen();
}

QSqlError TagSpace::last_db_error() const {
	return db.lastError();
}

QString TagSpace::get_db_file_path() const {
	return db_file_path;
}

QString TagSpace::get_db_lockfile_path() const {
	return db_lockfile_path;
}

static QString const INIT_TABLE_MEDIA = QStringLiteral("create table if not exists " TABLE_NAME_MEDIA " (" TABLE_SCHEMA_MEDIA ")");

void TagSpace::postconnect() {
	query.exec(INIT_TABLE_MEDIA);
}
