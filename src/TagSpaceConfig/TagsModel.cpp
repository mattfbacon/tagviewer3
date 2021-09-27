#include <QDataStream>
#include <QSettings>

#include "Settings.hpp"
#include "TagSpaceConfig/TagsModel.hpp"

QVariant const TagsModel::header_data[TagsModel::NUM_COLUMNS] = {
	QVariant::fromValue(tr("Name")),
	QVariant::fromValue(tr("Color")),
};
QVariant const TagsModel::tooltip_data[TagsModel::NUM_COLUMNS] = {
	QVariant::fromValue(tr("Name of the tag")),
	QVariant::fromValue(tr("Color for the tag")),
};
QString const TagsModel::color_annotation = tr("Hex color %1");

inline bool compare_ids(Tag const& tag1, Tag const& tag2) {
	return tag1.id < tag2.id;
}

TagsModel::TagsModel(TagsList const& data)
	: QAbstractListModel{}
	, m_data(data)
	, id_max(data.empty() ? -1 : std::max_element(std::cbegin(data), std::cend(data), compare_ids)->id) {
	//
}

TagsModel::~TagsModel() {
	//
}

QModelIndex TagsModel::index(int const row, int const column, QModelIndex const& parent) const {
	if (hasIndex(row, column, parent)) {
		auto const& row_data = m_data[row];
		return createIndex(row, column, column == 0 ? reinterpret_cast<quintptr>(&row_data.name) : reinterpret_cast<quintptr>(&row_data.color));
	} else {
		return QModelIndex{};
	}
}

int TagsModel::rowCount(QModelIndex const& parent) const {
	if (parent.isValid()) {  // not root; items don't have children
		return 0;
	} else {
		return m_data.size();
	}
}

int TagsModel::columnCount(QModelIndex const& parent) const {
	if (parent.isValid()) {
		return 0;
	} else {
		return NUM_COLUMNS;
	}
}

QVariant TagsModel::data(QModelIndex const& index, int const role) const {
	if (!index.isValid()) {
		return QVariant{};
	}
	switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole: {
			auto const& row_data = m_data[index.row()];
			switch (index.column()) {
				case 0:
					return QVariant::fromValue(row_data.name);
				case 1:
					return QVariant::fromValue(row_data.color);
				default:
					Q_ASSERT(false);  // already checked if the index is valid
			}
		}
		case Qt::ToolTipRole:
		case Qt::AccessibleDescriptionRole: {
			return tooltip_data[index.column()];
		}
		case Qt::AccessibleTextRole: {
			auto const& row_data = m_data[index.row()];
			switch (index.column()) {
				case 0:
					return QVariant::fromValue(row_data.name);
				case 1:
					return QVariant::fromValue(color_annotation.arg(row_data.color.name()));
				default:
					Q_ASSERT(false);  // already checked if the index is valid
			}
		}
		default:
			return QVariant{};
	}
}

Qt::ItemFlags TagsModel::flags(QModelIndex const& index) const {
	if (!index.isValid()) {
		return {};
	}
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant TagsModel::headerData(int const section, Qt::Orientation const orientation, int const role) const {
	switch (orientation) {
		case Qt::Horizontal: {
			if (section >= NUM_COLUMNS) {
				return QVariant{};
			}
			switch (role) {
				case Qt::DisplayRole:
				case Qt::AccessibleTextRole:
					return header_data[section];
				case Qt::ToolTipRole:
				case Qt::AccessibleDescriptionRole:
					return tooltip_data[section];
				default:
					return QVariant{};
			}
		}
		default:
			Q_ASSERT(false);
			// fall through
		case Qt::Vertical:
			return QVariant{};
	}
}

static QVector<int> const ROLES_THAT_CHANGE = {
	Qt::EditRole,
	Qt::DisplayRole,
	Qt::AccessibleTextRole,
};

bool TagsModel::setData(QModelIndex const& index, QVariant const& value, int const role) {
	if (!index.isValid()) {
		return false;
	}
	if (role != Qt::EditRole) {
		return false;
	}
	auto& row_data = m_data[index.row()];
	switch (index.column()) {
		case 0: {
			if (!value.canConvert(QMetaType::QString)) {
				return false;
			}
			row_data.name = value.value<QString>();
			emit dataChanged(index, index, ROLES_THAT_CHANGE);
			return true;
		}
		case 1: {
			// if (value.type() != QMetaType::QColor) {
			// 	return false;
			// }
			if (!value.canConvert(QMetaType::QColor)) {
				return false;
			}
			row_data.color = value.value<QColor>();
			emit dataChanged(index, index, ROLES_THAT_CHANGE);
			return true;
		}
		default:
			return false;
	}
}

bool TagsModel::setHeaderData(int const, Qt::Orientation const, QVariant const&, int const) {
	return false;  // header data is immutable
}

bool TagsModel::insertRows(int const row, int const count, QModelIndex const& parent) {
	if (parent.isValid()) {
		return false;
	}  // not root; no children
	if (row > m_data.size() || row < 0 || count < 0) {
		return false;
	}  // invalid
	if (count == 0) {
		return true;
	}  // no items
	beginInsertRows(parent, row, row + count - 1);
	QSettings const settings;
	m_data.insert(row, count, Tag{ g_settings->new_tag_name(), g_settings->new_tag_color() });
	for (int i = row; i < row + count; ++i) {
		m_data[i].id = ++id_max;
	}
	endInsertRows();
	return true;
}

void TagsModel::add_tag(Tag const& tag) {
	QModelIndex const parent;
	auto const size = m_data.size();
	beginInsertRows(parent, size, size);
	m_data.append(tag);
	// set ID based on what it should be within the model
	m_data[size].id = ++id_max;
	endInsertRows();
}

void TagsModel::compress_ids() {
	long i = 0;
	for (Tag& tag : m_data) {
		tag.id = i;
		++i;
	}
	id_max = i;
}

void TagsModel::load(TagsList const& new_data) {
	auto const new_size = new_data.size();
	auto const old_size = m_data.size();

	{
		QModelIndex const parent;
		if (new_size < old_size) {
			beginRemoveRows(parent, new_size, old_size - 1);
		} else if (new_size > old_size) {
			beginInsertRows(parent, old_size, new_size - 1);
		}
	}

	m_data = new_data;

	if (new_size < old_size) {
		endRemoveRows();
	} else if (new_size > old_size) {
		endInsertRows();
	}

	emit dataChanged(index(0, 0), index(std::min(new_size, old_size), NUM_COLUMNS - 1), ROLES_THAT_CHANGE);
}

TagsList const& TagsModel::save() const {
	return m_data;
}
