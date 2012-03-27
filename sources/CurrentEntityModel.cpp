#include "CurrentEntityModel.hpp"

CurrentEntityModel::CurrentEntityModel(Entity * entity, QObject *parent) :
  QAbstractTableModel(parent),
  m_entity(entity)
{
}

CurrentEntityModel::~CurrentEntityModel(void)
{
}

int CurrentEntityModel::rowCount(const QModelIndex &) const
{
  return (m_entity->getModelList().size() + m_entity->getEntityList().size());
}

int CurrentEntityModel::columnCount(const QModelIndex &) const
{
  return (1);
}

QVariant  CurrentEntityModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= rowCount(QModelIndex()))
    return (QVariant());
  if (role == Qt::DisplayRole)
    {
      if (index.column() == 0)
        {
          if (index.row() <  m_entity->getEntityList().size())
            return (QString("entity_%2 (%1)").arg(m_entity->getEntityList().at(index.row())->getEntity().getName()).arg(index.row()));
          else
            return (QString("model_%1").arg(index.row() - m_entity->getEntityList().size()));
        }
    }
  return (QVariant());
}

QVariant  CurrentEntityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return (QVariant());
  if (orientation == Qt::Horizontal)
    switch (section)
      {
      case 0:
        return (tr("Name"));
//      case 1:
//        return (tr("Path"));
      default:
        return (QVariant());
      }
  return (QVariant());
}

//Qt::ItemFlags CurrentEntityModel::flags(const QModelIndex &index) const
//{
//  if (!index.isValid())
//    return (Qt::ItemIsEnabled);
//  return (QAbstractTableModel::flags(index) | Qt::ItemIsEditable);
//}

//bool  CurrentEntityModel::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//  if (index.isValid() && role == Qt::EditRole)
//    {
////      if (index.column() == 0)
////        m_list[index.row()].setName(value.toString());
////      else if (index.column() == 1)
////        m_list[index.row()].setPath(value.toString());
////      else
////        return (false);
//      emit(dataChanged(index, index));
//      return (true);
//    }
//  return (false);
//}

//bool  CurrentEntityModel::insertRows(int row, int count, const QModelIndex &)
//{
//  beginInsertRows(QModelIndex(), row, row + count - 1);
//  for (int idx = 0; idx < count; ++idx)
//    //m_list.insert(row, Model(""));
//  endInsertRows();
//  return (true);
//}

//bool  CurrentEntityModel::removeRows(int row, int count, const QModelIndex &)
//{
//  beginRemoveRows(QModelIndex(), row, row + count - 1);
//  for (int idx = 0; idx < count; ++idx)
//    //m_list.removeAt(row);
//  endRemoveRows();
//  return (true);
//}

void  CurrentEntityModel::setEntity(Entity * entity)
{
  m_entity = entity;
}
