#ifndef OGREWIDGET_HPP
#define OGREWIDGET_HPP

#include <QtGui>
#include <Ogre.h>
#include "Model.hpp"
#include "Entity.hpp"

class OgreWidget : public QWidget
{
  Q_OBJECT

public:
  OgreWidget(QWidget *parent = 0);
  ~OgreWidget();

  QPaintEngine* paintEngine() const;

  void          addModel(Model const & model);

public slots:
  void setBackgroundColor(QColor c);
  void setCameraPosition(const Ogre::Vector3 &pos);

signals:
  void cameraPositionChanged(const Ogre::Vector3 &pos);

protected:
  virtual void keyPressEvent(QKeyEvent * e);
  virtual void moveEvent(QMoveEvent * e);
  virtual void mouseDoubleClickEvent(QMouseEvent * e);
  virtual void mouseMoveEvent(QMouseEvent * e);
  virtual void mousePressEvent(QMouseEvent * e);
  virtual void mouseReleaseEvent(QMouseEvent * e);
  virtual void paintEvent(QPaintEvent * e);
  virtual void resizeEvent(QResizeEvent * e);
  virtual void showEvent(QShowEvent * e);
  virtual void wheelEvent(QWheelEvent * e);

private:
  void initOgreSystem();
  void initResources();
  void initScene();

private:
  static const Ogre::Real turboModifier;
  static const QPoint invalidMousePoint;

  Ogre::Root          *m_ogreRoot;
  Ogre::SceneManager  *m_sceneManager;
  Ogre::RenderWindow  *m_renderWindow;
  Ogre::Viewport      *m_viewport;
  Ogre::Camera        *m_camera;

  QPoint          oldPos;
  Ogre::SceneNode *selectedNode;

  Entity  * m_currentEntity;
};

#endif // OGREWIDGET_H
