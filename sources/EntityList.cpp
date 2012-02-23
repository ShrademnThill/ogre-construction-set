#include "EntityList.hpp"

EntityList::EntityList(QObject * parent) :
  QAbstractTableModel(parent)
{
}

EntityList::~EntityList()
{
}

int EntityList::rowCount(const QModelIndex &) const
{
  return (m_list.size());
}

int EntityList::columnCount(const QModelIndex &) const
{
  return (2);
}

QVariant  EntityList::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= m_list.size())
    return (QVariant());
  if (role == Qt::DisplayRole)
    {
      if (index.column() == 0)
        return (m_list.at(index.row()).getName());
      else if (index.column() == 1)
        return (m_list.at(index.row()).isComposed());
    }
  return (QVariant());
}

QVariant  EntityList::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return (QVariant());
  if (orientation == Qt::Horizontal)
    switch (section)
      {
      case 0:
        return (tr("Name"));
      case 1:
        return (tr("Composed"));
      default:
        return (QVariant());
      }
  return (QVariant());
}

Qt::ItemFlags EntityList::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return (Qt::ItemIsEnabled);
  return (QAbstractTableModel::flags(index) | Qt::ItemIsEditable);
}

bool  EntityList::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole)
    {
      if (index.column() == 0)
        m_list[index.row()].setName(value.toString());
      else if (index.column() == 1)
        m_list[index.row()].setComposed(value.toBool());
      else
        return (false);
      emit(dataChanged(index, index));
      return (true);
    }
  return (false);
}

bool  EntityList::insertRows(int row, int count, const QModelIndex &)
{
  beginInsertRows(QModelIndex(), row, row + count - 1);
  for (int idx = 0; idx < count; ++idx)
    m_list.insert(row, Entity());
  endInsertRows();
  return (true);
}

bool  EntityList::removeRows(int row, int count, const QModelIndex &)
{
  beginRemoveRows(QModelIndex(), row, row + count - 1);
  for (int idx = 0; idx < count; ++idx)
    m_list.removeAt(row);
  endRemoveRows();
  return (true);
}

QList<Entity> & EntityList::getList(void)
{
  return (m_list);
}

void  EntityList::addEntity(Entity const & entity)
{
  insertRow(0);
  setData(index(0, 0), entity.getName(), Qt::EditRole);
}
