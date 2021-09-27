#pragma once

template <typename T1, typename T2>
struct QPair;

#include <QAbstractItemModel>
#include <QColor>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>

#include "DataStreamList.hpp"
#include "TagSpace/Tag.hpp"
#include "TagSpaceConfig/DataBackedModel.hpp"

using TagsList = QVector<Tag>;
Q_DECLARE_METATYPE(TagsList)
// this is required for QVariant to work properly
template QDataStream& operator<<<TagsList>(QDataStream& out, TagsList const& x);
template QDataStream& operator>><TagsList>(QDataStream& in, TagsList& x);

class TagsModel final
	: public QAbstractListModel
	, public DataBackedModel<TagsList> {
	Q_OBJECT
public:
	explicit TagsModel(TagsList const& data = TagsList{});
	~TagsModel();
	// ignores the parent
	QModelIndex index(int row, int column, QModelIndex const& parent = QModelIndex{}) const override;
	int rowCount(QModelIndex const& parent = QModelIndex{}) const override;
	int columnCount(QModelIndex const& parent = QModelIndex{}) const override;
	QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(QModelIndex const& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;
	bool setHeaderData(int section, Qt::Orientation orientation, QVariant const& value, int role = Qt::EditRole) override;
	bool insertRows(int row, int count, QModelIndex const& parent = QModelIndex()) override;

	void add_tag(Tag const& tag);
	void compress_ids();

	void load(TagsList const& new_data) override;
	TagsList const& save() const override;
protected:
	TagsList m_data;
	// the maximum ID within the tags.
	// To get the new ID and set the max at the same time, use `++id_max`.
	long id_max;
	static int constexpr NUM_COLUMNS = 2;
	static QVariant const header_data[TagsModel::NUM_COLUMNS];
	static QVariant const tooltip_data[TagsModel::NUM_COLUMNS];
	static QString const color_annotation;
};
