#include "SelectionManager.hpp"

SelectionManager::SelectionManager() :
  m_position(0, 0, 0)
{
}

SelectionManager::~SelectionManager()
{
}

Ogre::Vector3 SelectionManager::getPosition(void) const
{
  if (m_selection.size() == 1)
    return (m_selection[0]->getRoot()->getPosition());
  return (m_position);
}

Ogre::Quaternion  SelectionManager::getOrientation(void) const
{
  if (m_selection.size() == 1)
    return (m_selection[0]->getRoot()->getOrientation());
  return (Ogre::Quaternion());
}

Ogre::Vector3 SelectionManager::getScale(void) const
{
  if (m_selection.size() == 1)
    return (m_selection[0]->getRoot()->getScale());
  return (Ogre::Vector3(1, 1, 1));
}

QList<InstItem *> const & SelectionManager::getSelection(void) const
{
  return (m_selection);
}

bool  SelectionManager::isEmpty(void) const
{
  return (m_selection.isEmpty());
}

void  SelectionManager::setPosition(double x, double y, double z)
{
  if (m_selection.size() == 1)
    m_selection[0]->getRoot()->setPosition(x, y, z);
  else
    for (int i = 0; i < m_selection.size(); ++i)
      m_selection[i]->getRoot()->translate(x, y, z);
}

void  SelectionManager::setOrientation(double w, double x, double y, double z)
{
  if (m_selection.size() == 1)
    m_selection[0]->getRoot()->setOrientation(w, x, y, z);
}

void  SelectionManager::setScale(double x, double y, double z)
{
  if (m_selection.size() == 1)
    m_selection[0]->getRoot()->setScale(x, y, z);
  else
    for (int i = 0; i < m_selection.size(); ++i)
      m_selection[i]->getRoot()->scale(x, y, z);
}

void  SelectionManager::translate(double x, double y, double z)
{
  if (m_selection.size() == 1)
    m_selection[0]->getRoot()->translate(x, y, z);
  else
    for (int i = 0; i < m_selection.size(); ++i)
      m_selection[i]->getRoot()->translate(x, y, z);
}

void  SelectionManager::pitch(Ogre::Radian const & angle)
{
  if (m_selection.size() == 1)
    m_selection[0]->getRoot()->pitch(angle);
}

void  SelectionManager::yaw(Ogre::Radian const & angle)
{
  if (m_selection.size() == 1)
    m_selection[0]->getRoot()->yaw(angle);
}

void  SelectionManager::roll(Ogre::Radian const & angle)
{
  if (m_selection.size() == 1)
    m_selection[0]->getRoot()->roll(angle);
}

void  SelectionManager::addItem(InstItem * item)
{
  item->getRoot()->showBoundingBox(true);
  m_selection.append(item);
}

void  SelectionManager::removeItem(InstItem * item)
{
  item->getRoot()->showBoundingBox(false);
  m_selection.removeOne(item);
}

void  SelectionManager::clearSelection(bool deleted)
{
  if (!deleted)
    for (int i = 0; i < m_selection.size(); ++i)
      m_selection[i]->getRoot()->showBoundingBox(false);
  m_selection.clear();
}
