#ifndef GRIDOBJECT_HPP
#define GRIDOBJECT_HPP

#include <Ogre.h>

class GridObject
{
public:
  GridObject();

  static Ogre::ManualObject *  createGrid(Ogre::SceneManager * scene);

private:
  static void  addMaterial(Ogre::String const & mat, Ogre::ColourValue const & clr, Ogre::SceneBlendType sbt);

};

#endif // GRIDOBJECT_HPP
