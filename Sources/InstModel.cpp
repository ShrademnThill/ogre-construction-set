#include "InstModel.hpp"

InstModel::InstModel(Model const &)
{
}

InstModel::~InstModel()
{
}

void InstModel::setRoot(Ogre::SceneNode * node)
{
  m_root = node;
}

Ogre::SceneNode * InstModel::getRoot(void)
{
  return (m_root);
}
