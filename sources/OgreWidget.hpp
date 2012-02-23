#ifndef OGREWIDGET_HPP
#define OGREWIDGET_HPP

#include <QtGui>
#include <Ogre.h>
#include "Camera.hpp"
#include "Model.hpp"
#include "Entity.hpp"
#include "SelectionBuffer.hpp"

class OgreWidget : public QWidget
{
  Q_OBJECT

public:
  OgreWidget(QWidget * parent = 0);
  ~OgreWidget();

  QPaintEngine *  paintEngine() const;
  void            addItem(Model const & model);
  void            addItem(Entity const & entity);
  void            setCurrentEntity(Entity * entity);

  Ogre::SceneNode * getSelectedNode();

public slots:
  void  setBackgroundColor(QColor c);

signals:
  void  itemSelected(bool);
  void  itemMoved();

protected:
  virtual void  moveEvent(QMoveEvent * e);

  virtual void  mouseMoveEvent(QMouseEvent * e);
  virtual void  mousePressEvent(QMouseEvent * e);
  virtual void  mouseReleaseEvent(QMouseEvent * e);

  virtual void  keyPressEvent(QKeyEvent * e);

  virtual void  paintEvent(QPaintEvent * e);
  virtual void  resizeEvent(QResizeEvent * e);
  virtual void  showEvent(QShowEvent * e);

private:
  void  initOgreSystem();
  void  initResources();
  void  initScene();

  void  mouseSelect(QPoint const & pos);
  void  deleteSelection();

  static const QPoint InvalidMousePoint;

  Ogre::Root *          m_ogreRoot;
  Ogre::SceneManager *  m_sceneManager;
  Ogre::RenderWindow *  m_renderWindow;
  Ogre::Viewport *      m_viewport;
  Camera *              m_camera;

  QPoint            m_oldPos;
  Ogre::SceneNode * m_selectedNode;
  Qt::MouseButtons  m_mouseButtonsPressed;
  SelectionBuffer * m_selectionBuffer;

  Entity *  m_currentEntity;
};

#endif // OGREWIDGET_H
