#include "DataManager.hpp"
#include "EntityModelItem.hpp"

EntityModelItem::EntityModelItem(Entity * entity, QString const & name) :
  m_entity(entity),
  m_name(name)
{
  if (!m_entity)
    {
      setBackground(QColor(0xBBFFBB));
    }
  else
    {
      setDropEnabled(false);
    }
}

QVariant  EntityModelItem::data(int role) const
{
  if (m_entity && (role == Qt::EditRole || role == Qt::DisplayRole))
    {
      if (column() == 0)
        return (m_entity->getName());
      else if (column() == 1)
        return (m_entity->isComposed());
    }
  else if ((role == Qt::EditRole || role == Qt::DisplayRole) && column() == 0)
    return (m_name);
  return (QStandardItem::data(role));
}

void      EntityModelItem::setData(QVariant const & value, int role)
{
  if (m_entity && role == Qt::EditRole)
    {
      if (column() == 0)
        m_entity->setName(value.toString());
      else if (column() == 1)
        m_entity->setComposed(value.toBool());
    }
  else if (role == Qt::EditRole)
    m_name = value.toString();
  else
    QStandardItem::setData(value, role);
}

QStandardItem * EntityModelItem::clone() const
{
  return (new EntityModelItem);
}

int EntityModelItem::type() const
{
  return (QStandardItem::UserType + 1);
}

void  EntityModelItem::read(QDataStream & in)
{
  QVariant v;

  m_name.clear();
  m_entity = 0;
  in >> v;
  if (v.toInt() != -1)
    m_entity = DataManager::getSingleton()->getEntityList()->at(v.toInt());
  in >> v;
  m_name = v.toString();
  QStandardItem::read(in);
}

void  EntityModelItem::write(QDataStream & out) const
{
  QVariant v;
  int idx = -1;

  if (m_entity)
    idx = DataManager::getSingleton()->getEntityList()->indexOf(m_entity);

  v.setValue<int>(idx);
  out << v;
  v.setValue<QString>(m_name);
  out << v;
  QStandardItem::write(out);
}

Entity *  EntityModelItem::getEntity(void)
{
  return (m_entity);
}

QString   EntityModelItem::getName(void) const
{
  return (m_name);
}
