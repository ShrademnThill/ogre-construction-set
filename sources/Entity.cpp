#include "Entity.hpp"

Entity::Entity(QString const & name) :
  m_name(name),
  m_composed(true)
{
}

Entity::~Entity()
{
}

void  Entity::createModel(Model const & model, Ogre::SceneManager * sceneManager)
{
  m_modelList.append(new InstModel(model, sceneManager->getRootSceneNode()));
}

void  Entity::createEntity(Entity const & entity, Ogre::SceneManager * sceneManager)
{
  m_entityList.append(new InstEntity(entity, sceneManager->getRootSceneNode()));
}

void  Entity::deleteItem(Ogre::SceneNode * node)
{
  for (int i = 0; i < m_modelList.size(); ++i)
    if (m_modelList[i]->getRoot() == node)
      delete m_modelList.takeAt(i);
}

void  Entity::load(Ogre::SceneNode * node) const
{
  for (int i = 0; i < m_modelList.size(); ++i)
    m_modelList[i]->load(node);
  for (int i = 0; i < m_entityList.size(); ++i)
    m_entityList[i]->load(node);
}

void  Entity::unload(Ogre::SceneNode * node) const
{
  for (int i = 0; i < m_modelList.size(); ++i)
    m_modelList[i]->unload();
  for (int i = 0; i < m_entityList.size(); ++i)
    m_entityList[i]->unload();
  node->removeAllChildren();
}

void  Entity::setName(QString const & name)
{
  m_name = name;
}

void  Entity::setComposed(bool composed)
{
  m_composed = composed;
}

QString const & Entity::getName(void) const
{
  return (m_name);
}

bool  Entity::isComposed(void) const
{
  return (m_composed);
}

QList<InstModel *> const &  Entity::getModelList(void) const
{
  return (m_modelList);
}

QList<InstEntity *> const & Entity::getEntityList(void) const
{
  return (m_entityList);
}
