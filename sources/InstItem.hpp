#ifndef INSTITEM_HPP
#define INSTITEM_HPP

#include <Ogre.h>

class InstItem
{
public:
  InstItem();
  virtual ~InstItem();

  Ogre::SceneNode * getRoot();
  int               getInstanciationProbability(void) const;

  void  setRoot(Ogre::SceneNode * node);
  void  setInstanciationProbability(unsigned int value);

protected:
  Ogre::SceneNode * m_root;
  Ogre::Vector3     m_position;
  Ogre::Quaternion  m_orientaion;
  Ogre::Vector3     m_scale;
  int               m_instanciationProbability;
};

#endif // INSTITEM_HPP
