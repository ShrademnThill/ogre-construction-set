#include "InstModel.hpp"

InstModel::InstModel(Model const & model, Ogre::SceneManager * sceneManager) :
  m_root(0),
  m_model(model)
{
  Ogre::Entity * entity = sceneManager->createEntity(m_model.getPath().toStdString());
  Ogre::SceneNode * node = sceneManager->getRootSceneNode()->createChildSceneNode();

  node->attachObject(entity);
  m_root = node;
}

InstModel::~InstModel()
{
  if (m_root)
    {
      Ogre::MovableObject * ent = m_root->getAttachedObject(0);
      Ogre::SceneManager *  sceneManager = m_root->getCreator();

      sceneManager->destroyMovableObject(ent);
      sceneManager->destroySceneNode(m_root);
    }
}

Ogre::SceneNode * InstModel::getRoot()
{
  return (m_root);
}

Model const &     InstModel::getModel() const
{
  return (m_model);
}

void InstModel::setRoot(Ogre::SceneNode * node)
{
  m_root = node;
}
