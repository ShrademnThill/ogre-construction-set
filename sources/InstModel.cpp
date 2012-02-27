#include "InstModel.hpp"

InstModel::InstModel(Model const & model, Ogre::SceneNode * node) :
  m_model(model)
{
  load(node);
}

InstModel::~InstModel()
{
  if (m_root)
    {
      m_root->showBoundingBox(false);
      m_root->setVisible(false);
    }
}

Model const &     InstModel::getModel() const
{
  return (m_model);
}

void  InstModel::load(Ogre::SceneNode * node)
{
  Ogre::Entity * entity = node->getCreator()->createEntity(m_model.getPath().toStdString());

  m_root = node->createChildSceneNode();
  m_root->getUserObjectBindings().setUserAny(Ogre::Any(static_cast<InstItem *>(this)));
  m_root->attachObject(entity);
  m_root->setPosition(m_position);
  m_root->setOrientation(m_orientaion);
  m_root->setScale(m_scale);
}

void  InstModel::unload(void)
{
  if (!m_root)
    return ;
  m_position = m_root->getPosition();
  m_orientaion = m_root->getOrientation();
  m_scale = m_root->getScale();
  m_root = 0;
}
