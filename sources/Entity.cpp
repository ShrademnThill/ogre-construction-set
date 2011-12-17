#include "Entity.hpp"

Entity::Entity()
{
}

Entity::~Entity()
{
}

InstModel *   Entity::createModel(const Model & model, Ogre::SceneManager * sceneManager)
{
  InstModel * instModel = new InstModel(model, sceneManager);

  m_modelList.append(instModel);
  return (instModel);
}

void  Entity::deleteItem(Ogre::SceneNode * node)
{
  for (int i = 0; i < m_modelList.size(); ++i)
    if (m_modelList[i]->getRoot() == node)
      delete m_modelList.takeAt(i);
}

void  Entity::load(Ogre::SceneManager * sceneManager)
{
  for (int i = 0; i < m_modelList.size(); ++i)
    sceneManager->getRootSceneNode()->addChild(m_modelList[i]->getRoot());
}

void  Entity::unload(Ogre::SceneManager * sceneManager)
{
  sceneManager->getRootSceneNode()->removeAllChildren();
}
