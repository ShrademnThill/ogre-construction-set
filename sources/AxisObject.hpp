#ifndef AXISOBJECT_HPP
#define AXISOBJECT_HPP

#include <Ogre.h>

class AxisObject
{
public:
  enum BoxParts
  {
    BOX_NONE = 0x00,
    BOX_TOP  = 0x01,
    BOX_BOT = 0x02,
    BOX_FRONT = 0x04,
    BOX_BACK = 0x08,
    BOX_LEFT = 0x10,
    BOX_RIGHT = 0x20,
    BOX_ALL = 0xFF
  };

  AxisObject();

  static Ogre::ManualObject *  createAxis(Ogre::SceneManager * scene, Ogre::String const & name, Ogre::Real scale);

private:
  static void  addMaterial(Ogre::String const & mat, Ogre::ColourValue const & clr, Ogre::SceneBlendType sbt);
  static void  addBox(Ogre::ManualObject * obj, Ogre::Vector3 dim, Ogre::Vector3 pos, Ogre::ColourValue color, short boxMask);
};

#endif // AXISOBJECT_HPP
