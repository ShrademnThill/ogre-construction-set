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

void  InstItem::setRoot(Ogre::SceneNode * node)
{
  m_root = node;
}

void  InstItem::setInstanciationProbability(unsigned int value)
{
  m_instanciationProbability = value;
}
