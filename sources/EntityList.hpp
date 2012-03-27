#ifndef ENTITYLIST_HPP
#define ENTITYLIST_HPP

#include <QAbstractTableModel>
#include <QList>
#include "Entity.hpp"

//class Node
//{
//public:
//  Node(Node * parent = 0, Entity * entity = 0);
//  ~Node();

//  Node *          getParent() const;
//  Entity *        getEntity();
//  QList<Node *> & getChildren();

//private:
//  Node *        m_parent;
//  QList<Node *> m_children;
//  Entity *      m_entity;
//};

//class EntityList : public QAbstractItemModel
//{
//public:
//  EntityList(QObject * parent = 0);
//  ~EntityList(void);

//  QModelIndex   index(int row, int column, QModelIndex const & parent = QModelIndex()) const;
//  QModelIndex   parent(QModelIndex const & child) const;
//  int           rowCount(QModelIndex const & parent) const;
//  int           columnCount(QModelIndex const & parent) const;
//  QVariant      data(QModelIndex const & index, int role) const;
//  QVariant      headerData(int section, Qt::Orientation orientation, int role) const;

//  Qt::ItemFlags flags(QModelIndex const & index) const;

//  bool          setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
//  bool          insertRows(int row, int count, const QModelIndex &index = QModelIndex());
//  bool          removeRows(int row, int count, const QModelIndex &index = QModelIndex());
//};

class EntityList : public QAbstractTableModel
{
public:
  EntityList(QObject * parent = 0);
  ~EntityList(void);

  int           rowCount(QModelIndex const & parent) const;
  int           columnCount(QModelIndex const & parent) const;
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
