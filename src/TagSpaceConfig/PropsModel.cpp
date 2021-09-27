#include <QSettings>

#include "ComboBoxHelpers.hpp"
#include "Settings.hpp"
#include "TagSpaceConfig/PropsModel.hpp"

QVariant const PropsModel::header_data[PropsModel::NUM_COLUMNS] = {
	QVariant::fromValue(tr("Name", "prop data")),
	QVariant::fromValue(tr("Type", "prop data")),
};
QVariant const PropsModel::tooltip_data[PropsModel::NUM_COLUMNS] = {
	QVariant::fromValue(tr("Name of the prop")),
	QVariant::fromValue(tr("Type for the prop")),
};

PropsModel::PropsModel(PropsList const& data)
	: QAbstractListModel{}
	, m_data(data) {
	//
}

PropsModel::~PropsModel() {
	//
}

QModelIndex PropsModel::index(int const row, int const column, QModelIndex const& parent) const {
	if (hasIndex(row, column, parent)) {
		auto const& row_data = m_data[row];
		return createIndex(row, column, column == 0 ? reinterpret_cast<quintptr>(&row_data.name) : reinterpret_cast<quintptr>(&row_data.type));
	} else {
		return QModelIndex{};
	}
}

int PropsModel::rowCount(QModelIndex const& parent) const {
	if (parent.isValid()) {  // not root; items don't have children
		return 0;
	} else {
		return m_data.size();
	}
}

int PropsModel::columnCount(QModelIndex const& parent) const {
	if (parent.isValid()) {
		return 0;
	} else {
		return NUM_COLUMNS;
	}
}

QVariant PropsModel::data(QModelIndex const& index, int const role) const {
	if (!index.isValid()) {
		return QVariant{};
	}
	switch (role) {
		case Qt::EditRole: {
			auto const& row_data = m_data[index.row()];
			switch (index.column()) {
				case 0:
					return QVariant::fromValue(row_data.name);
				case 1:
					return QVariant::fromValue(row_data.type);
				default:
					Q_ASSERT(false);  // already checked if the index is valid
			}
		}
		case Qt::ToolTipRole:
		case Qt::AccessibleDescriptionRole: {
			return tooltip_data[index.column()];
		}
		case Qt::DisplayRole:
		case Qt::AccessibleTextRole: {
			auto const& row_data = m_data[index.row()];
			switch (index.column()) {
				case 0:
					return QVariant::fromValue(row_data.name);
				case 1:
					return QVariant::fromValue(ComboBoxHelpers::NewPropType::names[ComboBoxHelpers::NewPropType::from_saved(row_data.type)]);
				default:
					Q_ASSERT(false);  // already checked if the index is valid
			}
		}
		default:
			return QVariant{};
	}
}

Qt::ItemFlags PropsModel::flags(QModelIndex const& index) const {
	if (!index.isValid()) {
		return {};
	}
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant PropsModel::headerData(int const section, Qt::Orientation const orientation, int const role) const {
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

bool PropsModel::setData(QModelIndex const& index, QVariant const& value, int const role) {
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
			if (!value.canConvert(QMetaType::UInt)) {
				return false;
			}
			row_data.type = value.value<unsigned int>();
			emit dataChanged(index, index, ROLES_THAT_CHANGE);
			return true;
		}
		default:
			return false;
	}
}

bool PropsModel::setHeaderData(int const, Qt::Orientation const, QVariant const&, int const) {
	return false;  // header data is immutable
}

bool PropsModel::insertRows(int const row, int const count, QModelIndex const& parent) {
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
	m_data.insert(row, count, Prop{ g_settings->new_prop_name(), g_settings->new_prop_type() });
	endInsertRows();
	return true;
}

void PropsModel::add_prop(Prop const& prop) {
	QModelIndex const parent;
	auto const size = m_data.size();
	beginInsertRows(parent, size, size);
	m_data.append(prop);
	endInsertRows();
}

void PropsModel::load(PropsList const& new_data) {
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

PropsList const& PropsModel::save() const {
	return m_data;
}
