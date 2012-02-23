#ifndef CURRENTENTITYMODEL_HPP
#define CURRENTENTITYMODEL_HPP

#include <QAbstractTableModel>
#include "Entity.hpp"

class CurrentEntityModel : public QAbstractTableModel
{
public:
  CurrentEntityModel(Entity * entity, QObject * parent = 0);
  ~CurrentEntityModel(void);

  int           rowCount(const QModelIndex &parent) const;
  int           columnCount(const QModelIndex &parent) const;
  QVariant      data(const QModelIndex &index, int role) const;
  QVariant      headerData(int section, Qt::Orientation orientation, int role) const;
//  Qt::ItemFlags flags(const QModelIndex &index) const;
//  bool          setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
//  bool          insertRows(int row, int count, const QModelIndex &index = QModelIndex());
//  bool          removeRows(int row, int count, const QModelIndex &index = QModelIndex());

  void  setEntity(Entity * entity);

private:
  Entity *  m_entity;
};

#endif // CURRENTENTITYMODEL_HPP
