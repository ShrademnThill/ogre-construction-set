#include "InstEntity.hpp"

InstEntity::InstEntity(Entity & entity, Ogre::SceneNode * node) :
  m_entity(entity)
{
  load(node);
}

InstEntity::~InstEntity()
{
  if (m_root)
    {
      m_root->showBoundingBox(false);
      m_root->setVisible(false);
    }
}

Entity const &  InstEntity::getEntity() const
{
  return (m_entity);
}

void  InstEntity::load(Ogre::SceneNode * node)
{
  m_root = node->createChildSceneNode();
  m_root->getUserObjectBindings().setUserAny(Ogre::Any(static_cast<InstItem *>(this)));
  m_entity.load(m_root);
  m_root->setPosition(m_position);
  m_root->setOrientation(m_orientaion);
  m_root->setScale(m_scale);
}

void  InstEntity::unload(void)
{
  m_entity.unload(m_root);
  if (!m_root)
    return ;
  m_position = m_root->getPosition();
  m_orientaion = m_root->getOrientation();
  m_scale = m_root->getScale();
  m_root = 0;
}
