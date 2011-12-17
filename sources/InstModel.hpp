#ifndef INSTMODEL_HPP
#define INSTMODEL_HPP

#include <Ogre.h>
#include "Model.hpp"

class InstModel
{
public:
  InstModel(Model const & model, Ogre::SceneManager * sceneManager);
  ~InstModel();

  Ogre::SceneNode * getRoot();
  Model const &     getModel() const;

  void  setRoot(Ogre::SceneNode * node);

private:
  Ogre::SceneNode * m_root;
  Model const &     m_model;
};

#endif // INSTMODEL_HPP
