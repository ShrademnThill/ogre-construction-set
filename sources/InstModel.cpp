#include "InstModel.hpp"

InstModel::InstModel(Model const & model, Ogre::SceneNode * node) :
  m_root(0),
  m_model(model),
  m_position(0, 0, 0)
{
  load(node);
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

void  InstModel::load(Ogre::SceneNode * node)
{
  Ogre::Entity * entity = node->getCreator()->createEntity(m_model.getPath().toStdString());

  m_root = node->createChildSceneNode();
  m_root->setPosition(m_position);
  m_root->setOrientation(m_orientaion);
  m_root->attachObject(entity);
}

void  InstModel::unload(void)
{
  m_position = m_root->getPosition();
  m_orientaion = m_root->getOrientation();
}
