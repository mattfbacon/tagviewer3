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
#include "TagSpace/Prop.hpp"
#include "TagSpaceConfig/DataBackedModel.hpp"

using PropsList = QVector<Prop>;
Q_DECLARE_METATYPE(PropsList)
template QDataStream& operator<<<PropsList>(QDataStream& out, PropsList const& x);
template QDataStream& operator>><PropsList>(QDataStream& in, PropsList& x);

class PropsModel final
	: public QAbstractListModel
	, public DataBackedModel<PropsList> {
	Q_OBJECT
public:
	explicit PropsModel(PropsList const& data = PropsList{});
	~PropsModel();
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

	void add_prop(Prop const& prop);

	void load(PropsList const& new_data) override;
	PropsList const& save() const override;
protected:
	PropsList m_data;

	static int constexpr NUM_COLUMNS = 2;
	static QVariant const header_data[PropsModel::NUM_COLUMNS];
	static QVariant const tooltip_data[PropsModel::NUM_COLUMNS];
};
