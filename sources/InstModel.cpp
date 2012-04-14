#include <QMessageBox>
#include "InstModel.hpp"

InstModel::InstModel(Model const & model) :
  m_model(model)
{
  m_type = ModelType;
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
  try
  {
    Ogre::Entity * entity = node->getCreator()->createEntity(m_model.getPath().toStdString());

    m_root = node->createChildSceneNode();
    m_root->getUserObjectBindings().setUserAny(Ogre::Any(static_cast<InstItem *>(this)));
    m_root->attachObject(entity);
    m_root->setPosition(m_position);
    m_root->setOrientation(m_orientation);
    m_root->setScale(m_scale);
  }
  catch (Ogre::Exception &)
  {
    QMessageBox::warning(0, "Instanciation Denied", m_model.getPath() + " not found.");
    return ;
  }
}

void  InstModel::unload(void)
{
  if (!m_root)
    return ;
  m_position = m_root->getPosition();
  m_orientation = m_root->getOrientation();
  m_scale = m_root->getScale();
  m_root = 0;
}
