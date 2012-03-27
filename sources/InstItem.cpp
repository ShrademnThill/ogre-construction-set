#include "InstItem.hpp"

InstItem::InstItem() :
  m_root(0),
  m_position(0, 0, 0),
  m_scale(1, 1, 1),
  m_instanciationProbability(0)
{
}

InstItem::~InstItem()
{
}

Ogre::SceneNode * InstItem::getRoot()
{
  return (m_root);
}

int  InstItem::getInstanciationProbability(void) const
{
  return (m_instanciationProbability);
}

QStringList const & InstItem::getTags(void) const
{
  return (m_tags);
}

ItemType  InstItem::getType(void) const
{
  return (m_type);
}

Ogre::Vector3 InstItem::getPosition(void) const
{
  return (m_position);
}

Ogre::Vector3 & InstItem::getPosition(void)
{
  return (m_position);
}

Ogre::Quaternion  InstItem::getOrientation(void) const
{
  return (m_orientation);
}

Ogre::Quaternion &  InstItem::getOrientation(void)
{
  return (m_orientation);
}

Ogre::Vector3 InstItem::getScale(void) const
{
  return (m_scale);
}

Ogre::Vector3 & InstItem::getScale(void)
{
  return (m_scale);
}

void  InstItem::setTags(QStringList const & tags)
{
  m_tags = tags;
}

void  InstItem::setRoot(Ogre::SceneNode * node)
{
  m_root = node;
}

void  InstItem::setInstanciationProbability(unsigned int value)
{
  m_instanciationProbability = value;
}
