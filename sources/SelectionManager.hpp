#ifndef SELECTIONMANAGER_HPP
#define SELECTIONMANAGER_HPP

#include <QList>
#include <Ogre.h>
#include "InstItem.hpp"

class SelectionManager
{
public:
  SelectionManager();
  ~SelectionManager();

  Ogre::Vector3             getPosition(void) const;
  Ogre::Quaternion          getOrientation(void) const;
  Ogre::Vector3             getScale(void) const;
  QList<InstItem *> const & getSelection(void) const;
  bool                      isEmpty(void) const;

  void  setPosition(double x, double y, double z);
  void  setOrientation(double w, double x, double y, double z);
  void  setScale(double x, double y, double z);

  void  translate(double x, double y, double z);
  void  pitch(Ogre::Radian const & angle);
  void  yaw(Ogre::Radian const & angle);
  void  roll(Ogre::Radian const & angle);

  void  addItem(InstItem * item);
  void  removeItem(InstItem * item);
  void  clearSelection(bool deleted = false);

private:
  QList<InstItem *> m_selection;
  Ogre::Vector3     m_position;
};

#endif // SELECTIONMANAGER_HPP
