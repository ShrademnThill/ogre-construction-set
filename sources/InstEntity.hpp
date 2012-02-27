#ifndef INSTENTITY_HPP
#define INSTENTITY_HPP

#include "InstItem.hpp"
#include "Entity.hpp"

class Entity;

class InstEntity : public InstItem
{
public:
  InstEntity(Entity &entity, Ogre::SceneNode * node);
  ~InstEntity();

  Entity const &  getEntity() const;

  void  load(Ogre::SceneNode * node);
  void  unload(void);

private:
  Entity &    m_entity;
};

#endif // INSTENTITY_HPP
