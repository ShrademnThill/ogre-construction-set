#include "Camera.hpp"

Camera::Camera(Ogre::String const & name, Ogre::SceneManager * sm) :
  Ogre::Camera(name, sm)
{
}

Ogre::Real  Camera::getZoom(void) const
{
  return (m_zoom);
}


void        Camera::zoom(Ogre::Real delta)
{
  m_zoom -= delta;
  moveRelative(Ogre::Vector3(0, 0, -delta));
}

void        Camera::shift(Ogre::Real deltaX, Ogre::Real deltaY)
{
  moveRelative(Ogre::Vector3(deltaX, -deltaY, 0));
}

void        Camera::rotate(Ogre::Real deltaX, Ogre::Real deltaY)
{
  moveRelative(Ogre::Vector3(0, 0, -m_zoom));
  yaw(Ogre::Degree(deltaX));
  pitch(Ogre::Degree(deltaY));
  moveRelative(Ogre::Vector3(0, 0, m_zoom));
}
