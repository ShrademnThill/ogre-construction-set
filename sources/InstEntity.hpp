#ifndef INSTENTITY_HPP
#define INSTENTITY_HPP

#include <Ogre.h>
#include "Entity.hpp"

class Entity;

class InstEntity
{
public:
  InstEntity(Entity const & entity, Ogre::SceneNode * node);
  ~InstEntity();

  Ogre::SceneNode * getRoot();
  Entity const &    getEntity() const;

  void  setRoot(Ogre::SceneNode * node);

  void  load(Ogre::SceneNode * node);
  void  unload(void);

private:
  Ogre::SceneNode * m_root;
  Entity const &    m_entity;
  Ogre::Vector3     m_position;
  Ogre::Quaternion  m_orientaion;
};

#endif // INSTENTITY_HPP
