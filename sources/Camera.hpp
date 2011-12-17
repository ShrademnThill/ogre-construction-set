#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <QString>
#include "Ogre.h"

class Camera : public Ogre::Camera
{
public:
  Camera(Ogre::String const & name, Ogre::SceneManager * sm);

  Ogre::Real  getZoom(void) const;
  void        zoom(Ogre::Real delta);
  void        shift(Ogre::Real deltaX, Ogre::Real deltaY);
  void        rotate(Ogre::Real deltaX, Ogre::Real deltaY);

private:
  Ogre::Real m_zoom;
};

#endif // CAMERA_HPP
