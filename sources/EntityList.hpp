#ifndef ENTITYLIST_HPP
#define ENTITYLIST_HPP

#include <QAbstractTableModel>
#include <QList>
#include "Entity.hpp"

class EntityList : public QAbstractTableModel
{
public:
  EntityList(QObject * parent = 0);
  ~EntityList(void);

  int           rowCount(const QModelIndex &parent) const;
  int           columnCount(const QModelIndex &parent) const;
  QVariant      data(const QModelIndex &index, int role) const;
  QVariant      headerData(int section, Qt::Orientation orientation, int role) const;
  bool          setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  bool          insertRows(int row, int count, const QModelIndex &index = QModelIndex());
  bool          removeRows(int row, int count, const QModelIndex &index = QModelIndex());

  QList<Entity> & getList(void);

  void  addEntity(Entity const &);

private:
  QList<Entity> m_list;
};

#endif // ENTITYLIST_HPP
