#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <QString>
#include "Ogre.h"

class Camera
{
public:
  Camera(Ogre::Camera * camera);
  ~Camera();

  Ogre::Real      getZoom() const;
  Ogre::Camera *  getCamera();

  void        zoom(Ogre::Real delta);
  void        shift(Ogre::Real deltaX, Ogre::Real deltaY);
  void        rotate(Ogre::Real deltaX, Ogre::Real deltaY);
  void        reset(Ogre::Real x = 0, Ogre::Real y = 0, Ogre::Real z = 0);

private:
  Ogre::Camera *  m_camera;
  Ogre::Real      m_zoom;
};

#endif // CAMERA_HPP
