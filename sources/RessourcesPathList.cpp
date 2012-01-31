#include "RessourcesPathList.hpp"

RessourcesPathList::RessourcesPathList(QObject * parent) :
  QAbstractTableModel(parent)
{
}

RessourcesPathList::~RessourcesPathList(void)
{
}

int RessourcesPathList::rowCount(const QModelIndex &) const
{
  return (m_list.size());
}

int RessourcesPathList::columnCount(const QModelIndex &) const
{
  return (2);
}

QVariant RessourcesPathList::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= m_list.size())
    return (QVariant());
  if (role == Qt::DisplayRole)
    {
      if (index.column() == 0)
        return (m_list.at(index.row()).path);
      else if (index.column() == 1)
        return (m_list.at(index.row()).rec);
    }
  return (QVariant());
}

QVariant RessourcesPathList::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return (QVariant());
  if (orientation == Qt::Horizontal)
    switch (section)
      {
      case 0:
        return (tr("Path"));
      case 1:
        return (tr("Recurisve"));
      default:
        return (QVariant());
      }
  return (QVariant());
}

Qt::ItemFlags RessourcesPathList::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return (Qt::ItemIsEnabled);
  return (QAbstractTableModel::flags(index) | Qt::ItemIsEditable);
}

bool RessourcesPathList::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole)
    {
      if (index.column() == 0)
        m_list[index.row()].path = value.toString();
      else if (index.column() == 1)
        m_list[index.row()].rec = value.toBool();
      else
        return (false);
      emit(dataChanged(index, index));
      return (true);
    }
  return (false);
}

bool RessourcesPathList::insertRows(int row, int count, const QModelIndex &)
{
  beginInsertRows(QModelIndex(), row, row + count - 1);
  for (int idx = 0; idx < count; ++idx)
    m_list.insert(row, RessourcesPath());
  endInsertRows();
  return (true);
}

bool RessourcesPathList::removeRows(int row, int count, const QModelIndex &)
{
  beginRemoveRows(QModelIndex(), row, row + count - 1);
  for (int idx = 0; idx < count; ++idx)
    m_list.removeAt(row);
  endRemoveRows();
  return (true);
}

QList<RessourcesPath> const & RessourcesPathList::getList(void) const
{
  return (m_list);
}
