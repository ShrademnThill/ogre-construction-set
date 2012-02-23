#ifndef INSTMODEL_HPP
#define INSTMODEL_HPP

#include <Ogre.h>
#include "Model.hpp"

class InstModel
{
public:
  InstModel(Model const & model, Ogre::SceneNode * node);
  ~InstModel();

  Ogre::SceneNode * getRoot();
  Model const &     getModel() const;

  void  setRoot(Ogre::SceneNode * node);

  void  load(Ogre::SceneNode * node);
  void  unload(void);

private:
  Ogre::SceneNode * m_root;
  Model const &     m_model;
  Ogre::Vector3     m_position;
  Ogre::Quaternion  m_orientaion;
};

#endif // INSTMODEL_HPP
