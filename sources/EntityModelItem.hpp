#ifndef ENTITYMODELITEM_HPP
#define ENTITYMODELITEM_HPP

#include <QStandardItem>
#include "Entity.hpp"

class EntityModelItem : public QStandardItem
{
public:
  EntityModelItem(Entity * entity = 0, QString const & name = "");

  QVariant        data(int role) const;
  void            setData(QVariant const & value, int role);
  QStandardItem * clone() const;
  int             type() const;
  void            read(QDataStream & in);
  void            write(QDataStream & out) const;

  Entity *  getEntity(void);
  QString   getName(void) const;

private:
  Entity *  m_entity;
  QString   m_name;
};

#endif // ENTITYMODELITEM_HPP
