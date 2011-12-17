#include <QDir>
#include "ModelList.hpp"
#include <QMessageBox>
ModelList::ModelList(QString const & path, QObject *parent) :
  QAbstractTableModel(parent)
{
  build(path);
}

ModelList::~ModelList(void)
{
}

int ModelList::rowCount(const QModelIndex &) const
{
  return (m_list.size());
}

int ModelList::columnCount(const QModelIndex &) const
{
  return (2);
}

QVariant ModelList::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= m_list.size())
    return (QVariant());
  if (role == Qt::DisplayRole)
    {
      if (index.column() == 0)
        return (m_list.at(index.row()).getName());
      else if (index.column() == 1)
        return (m_list.at(index.row()).getPath());
    }
  return (QVariant());
}

QVariant ModelList::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return (QVariant());
  if (orientation == Qt::Horizontal)
    switch (section)
      {
      case 0:
        return (tr("Name"));
      case 1:
        return (tr("path"));
      default:
        return (QVariant());
      }
  return (QVariant());
}

Qt::ItemFlags ModelList::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return (Qt::ItemIsEnabled);
  return (QAbstractTableModel::flags(index) | Qt::ItemIsEditable);
}

bool ModelList::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole)
    {
      if (index.column() == 0)
        m_list[index.row()].setName(value.toString());
      else if (index.column() == 1)
        m_list[index.row()].setPath(value.toString());
      else
        return (false);
      emit(dataChanged(index, index));
      return (true);
    }
  return (false);
}

bool ModelList::insertRows(int row, int count, const QModelIndex &)
{
  beginInsertRows(QModelIndex(), row, row + count - 1);
  for (int idx = 0; idx < count; ++idx)
    m_list.insert(row, Model(""));
  endInsertRows();
  return (true);
}

bool ModelList::removeRows(int row, int count, const QModelIndex &)
{
  beginRemoveRows(QModelIndex(), row, row + count - 1);
  for (int idx = 0; idx < count; ++idx)
    m_list.removeAt(row);
  endRemoveRows();
  return (true);
}

QList<Model> const & ModelList::getList() const
{
  return (m_list);
}

bool ModelList::isModel(QString const & path) const
{
  return (!path.isEmpty());
}

void ModelList::build(QString const & path)
{
  QDir dir(path);

  if (dir.exists())
    {
      QStringList filters("*.mesh");
      QFileInfoList dirInfoList = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::Dirs);
      QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::NoDotAndDotDot|QDir::Files);

      for (int i = 0; i < dirInfoList.size(); ++i)
        {
          build(dirInfoList.at(i).filePath());
        }
      for (int i = 0; i < fileInfoList.size(); ++i)
        {
          if (!fileInfoList.at(i).isDir())
            m_list.insert(0, Model(fileInfoList.at(i).filePath(), fileInfoList.at(i).fileName()));
        }
    }
}
