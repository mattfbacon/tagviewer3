#pragma once

#include <QListView>
#include <QObject>
#include <QStringList>
#include <QStringListModel>

class FileList final : public QListView {
	Q_OBJECT
public:
	explicit FileList();
	~FileList();
protected:
	QStringList const strings;
	QStringListModel model;
};
