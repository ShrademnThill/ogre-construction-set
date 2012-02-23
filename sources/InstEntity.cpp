#include "InstEntity.hpp"

InstEntity::InstEntity(Entity const & entity, Ogre::SceneNode * node) :
  m_entity(entity),
  m_position(0, 0, 0)
{
  load(node);
}

InstEntity::~InstEntity()
{
  if (m_root)
    {
      Ogre::MovableObject * ent = m_root->getAttachedObject(0);
      Ogre::SceneManager *  sceneManager = m_root->getCreator();

      sceneManager->destroyMovableObject(ent);
      sceneManager->destroySceneNode(m_root);
    }
}

Ogre::SceneNode * InstEntity::getRoot()
{
  return (m_root);
}

Entity const &  InstEntity::getEntity() const
{
  return (m_entity);
}

void  InstEntity::setRoot(Ogre::SceneNode * node)
{
  m_root = node;
}

void  InstEntity::load(Ogre::SceneNode * node)
{
  m_root = node->createChildSceneNode();
  m_entity.load(m_root);
  m_root->setPosition(m_position);
  m_root->setOrientation(m_orientaion);
  m_root->showBoundingBox(true);
}

void  InstEntity::unload(void)
{
  m_entity.unload(m_root);
  m_position = m_root->getPosition();
  m_orientaion = m_root->getOrientation();
}
