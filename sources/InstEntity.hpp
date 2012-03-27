#ifndef INSTENTITY_HPP
#define INSTENTITY_HPP

#include <QStringList>
#include "InstItem.hpp"
#include "Entity.hpp"

class Entity;

class InstEntity : public InstItem
{
public:
  InstEntity(Entity &entity, Ogre::SceneNode * node);
  ~InstEntity();

  Entity &  getEntity();

  void  load(Ogre::SceneNode * node);
  void  unload(void);

private:
  Entity &    m_entity;
};

#endif // INSTENTITY_HPP
