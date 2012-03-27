#include "DataManager.hpp"
#include "Camera.hpp"

Camera::Camera(Ogre::Camera * camera) :
  m_camera(camera),
  m_zoom(0)
{
}

Camera::~Camera()
{
}

Ogre::Real  Camera::getZoom() const
{
  return (m_zoom);
}

Ogre::Camera *  Camera::getCamera()
{
  return (m_camera);
}

void  Camera::zoom(Ogre::Real delta)
{
  m_zoom -= delta;// * m_zoom / 500.f;
  m_camera->moveRelative(Ogre::Vector3(0, 0, -delta));
}

void  Camera::shift(Ogre::Real deltaX, Ogre::Real deltaY)
{
  m_camera->moveRelative(Ogre::Vector3(deltaX, -deltaY, 0));
}

void  Camera::rotate(Ogre::Real deltaX, Ogre::Real deltaY)
{
  m_camera->moveRelative(Ogre::Vector3(0, 0, -m_zoom));
  m_camera->yaw(Ogre::Degree(deltaX));
  m_camera->pitch(Ogre::Degree(deltaY));
  m_camera->moveRelative(Ogre::Vector3(0, 0, m_zoom));
}

void  Camera::reset(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
  m_camera->setPosition(x + 1, y + 1, z - 1);
  m_camera->lookAt(x, y, z);
  m_camera->setPosition(x, y, z);
  m_zoom = DataManager::getSingleton()->getDefaultCameraDistance();
  m_camera->moveRelative(Ogre::Vector3(0, 0, m_zoom));
}
