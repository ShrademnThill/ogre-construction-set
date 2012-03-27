#ifndef OGREWIDGET_HPP
#define OGREWIDGET_HPP

#include <QtGui>
#include <Ogre.h>
#include "Camera.hpp"
#include "Model.hpp"
#include "Entity.hpp"
#include "InstItem.hpp"
#include "SelectionBuffer.hpp"
#include "SelectionManager.hpp"
#include "ConstraintManager.hpp"

class OgreWidget : public QFrame
{
  Q_OBJECT

public:
  OgreWidget(QWidget * parent = 0);
  ~OgreWidget();

  QPaintEngine *  paintEngine() const;

  SelectionManager *  getSelection(void);

public slots:
  void  setBackgroundColor(QColor c);
  void  setCurrentEntity(Entity * entity);
  void  selectItem(InstItem * item);
  void  unselectItem(void);
  void  addItem(Model const & model);
  void  addItem(Entity & entity);

  void  constraintX(bool value);
  void  constraintY(bool value);
  void  constraintZ(bool value);
  void  activeGrid(bool value);

  void  resetCamera(void);

signals:
  void  itemSelected();
  void  itemUnselected();
  void  itemMoved();
  void  itemDoubleClicked();

protected:
  virtual void  paintEvent(QPaintEvent * e);
  virtual void  showEvent(QShowEvent * e);
  virtual void  resizeEvent(QResizeEvent * e);
  virtual void  moveEvent(QMoveEvent * e);

  virtual void  mouseMoveEvent(QMouseEvent * e);
  virtual void  mousePressEvent(QMouseEvent * e);
  virtual void  mouseReleaseEvent(QMouseEvent * e);
  virtual void  mouseDoubleClickEvent(QMouseEvent * e);

  virtual void  keyPressEvent(QKeyEvent * e);
  virtual void  keyReleaseEvent(QKeyEvent * e);

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

  QPoint              m_oldPos;
  Qt::MouseButtons    m_mouseButtonsPressed;
  QMap<int, bool>     m_keyState;

  SelectionBuffer * m_selectionBuffer;
  SelectionManager  m_selectionManager;

  bool  m_constraintedX;
  bool  m_constraintedY;
  bool  m_constraintedZ;
  bool  m_gridActivated;

  Entity *  m_currentEntity;
};

#endif // OGREWIDGET_H
