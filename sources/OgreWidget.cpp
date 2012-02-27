#ifndef Q_WS_WIN
#include <QX11Info>
#endif

#include "OgreWidget.hpp"
#include "InstModel.hpp"

const QPoint     OgreWidget::InvalidMousePoint(-1,-1);

OgreWidget::OgreWidget(QWidget * parent)
  : QFrame(parent),
    m_ogreRoot(0),
    m_sceneManager(0),
    m_renderWindow(0),
    m_viewport(0),
    m_camera(0),
    m_oldPos(InvalidMousePoint),
    m_mouseButtonsPressed(0),
    m_selectionBuffer(0),
    m_currentEntity(0)
{
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_PaintOnScreen);
  setFocusPolicy(Qt::ClickFocus);
}

OgreWidget::~OgreWidget()
{
  if(m_renderWindow)
    m_renderWindow->removeAllViewports();
  if(m_ogreRoot)
    {
      m_ogreRoot->detachRenderTarget(m_renderWindow);
      if(m_sceneManager)
        m_ogreRoot->destroySceneManager(m_sceneManager);
    }
  delete m_ogreRoot;
  delete m_camera;
}

QPaintEngine *  OgreWidget::paintEngine() const
{
  return (0);
}

SelectionManager *  OgreWidget::getSelection(void)
{
  return (&m_selectionManager);
}

void  OgreWidget::setBackgroundColor(QColor c)
{
  if (m_viewport)
    {
      Ogre::ColourValue ogreColour;
      ogreColour.setAsARGB(c.rgba());
      m_viewport->setBackgroundColour(ogreColour);
    }
}

void  OgreWidget::setCurrentEntity(Entity * entity)
{
  if (m_currentEntity)
    m_currentEntity->unload(m_sceneManager->getRootSceneNode());
  m_currentEntity = entity;
  if (m_sceneManager)
    {
      m_sceneManager->clearScene();
      m_currentEntity->load(m_sceneManager->getRootSceneNode());
      update();
    }
}

void  OgreWidget::selectItem(InstItem * item)
{
  if (!item->getRoot())
    return ;
  m_selectionManager.clearSelection();
  m_selectionManager.addItem(item);
  emit itemSelected(true);
  update();
}

void  OgreWidget::unselectItem()
{
  m_selectionManager.clearSelection();
  emit itemSelected(false);
  update();
}

void  OgreWidget::addItem(Model const & model)
{
  m_currentEntity->createModel(model, m_sceneManager);
  update();
}

void  OgreWidget::addItem(Entity & entity)
{
  m_currentEntity->createEntity(entity, m_sceneManager);
  update();
}

void  OgreWidget::paintEvent(QPaintEvent * e)
{
  m_ogreRoot->_fireFrameStarted();
  m_renderWindow->update();
  m_ogreRoot->_fireFrameEnded();

  e->accept();
}

void  OgreWidget::showEvent(QShowEvent * e)
{
  QWidget::showEvent(e);

  if (!m_ogreRoot)
    {
      initOgreSystem();
    }
}

void  OgreWidget::resizeEvent(QResizeEvent * e)
{
  QWidget::resizeEvent(e);

  if (e->isAccepted())
    {
      const QSize &newSize = e->size();
      if(m_renderWindow)
        {
          m_renderWindow->resize(newSize.width(), newSize.height());
          m_renderWindow->windowMovedOrResized();
        }
      if (m_camera)
        {
          Ogre::Real aspectRatio = Ogre::Real(newSize.width()) / Ogre::Real(newSize.height());
          m_camera->getCamera()->setAspectRatio(aspectRatio);
        }
    }
}

void  OgreWidget::moveEvent(QMoveEvent * e)
{
  QWidget::moveEvent(e);

  if(e->isAccepted() && m_renderWindow)
    {
      m_renderWindow->windowMovedOrResized();
      update();
    }
}

void  OgreWidget::mouseMoveEvent(QMouseEvent * e)
{
  if (m_oldPos != InvalidMousePoint)
    {
      Ogre::Real deltaX = e->pos().x() - m_oldPos.x();
      Ogre::Real deltaY = e->pos().y() - m_oldPos.y();

      if (m_mouseButtonsPressed.testFlag(Qt::MiddleButton))
        {
          if(e->modifiers().testFlag(Qt::ControlModifier))
            m_camera->zoom(-deltaY);
          else if (e->modifiers().testFlag(Qt::AltModifier))
            m_camera->rotate(deltaX, -deltaY);
          else
            m_camera->shift(-deltaX, -deltaY);
        }
      else if (m_mouseButtonsPressed.testFlag(Qt::LeftButton) && !m_selectionManager.isEmpty())
        {
          if (e->modifiers().testFlag(Qt::AltModifier))
            {
              if (m_keyState.value(Qt::Key_X))
                m_selectionManager.pitch(Ogre::Degree(deltaX));
              else if (m_keyState.value(Qt::Key_Y))
                m_selectionManager.yaw(Ogre::Degree(deltaX));
              else if (m_keyState.value(Qt::Key_Z))
                m_selectionManager.roll(Ogre::Degree(deltaX));
            }
          else
            {
              Ogre::Plane                 plane(0, 1, 0, m_selectionManager.getPosition().y);
              Ogre::Ray                   oldRay = m_camera->getCamera()->getCameraToViewportRay(e->pos().x() / (float)width(), e->pos().y() / (float)height());
              Ogre::Ray                   ray = m_camera->getCamera()->getCameraToViewportRay(m_oldPos.x() / (float)width(), m_oldPos.y() / (float)height());
              std::pair<bool, Ogre::Real> oldResult = oldRay.intersects(plane);
              std::pair<bool, Ogre::Real> result = ray.intersects(plane);

              if (result.first && oldResult.first)
                {
                  Ogre::Vector3               point;

                  point = oldRay.getPoint(oldResult.second) - ray.getPoint(result.second);
                  m_selectionManager.translate(point.x, point.y, point.z);
                }
            }
          emit itemMoved();
        }
      m_oldPos = e->pos();

      update();
      e->accept();
    }
  else
    {
      e->ignore();
    }
}

void  OgreWidget::mousePressEvent(QMouseEvent * e)
{
  if (e->button() == Qt::MiddleButton || e->button() == Qt::LeftButton)
    {
      if (e->button() == Qt::LeftButton)
        {
          mouseSelect(e->pos());
          m_mouseButtonsPressed |= Qt::LeftButton;
        }
      if (e->button() == Qt::MiddleButton)
        m_mouseButtonsPressed |= Qt::MiddleButton;
      m_oldPos = e->pos();
      e->accept();
    }
  else
    {
      e->ignore();
    }
}

void  OgreWidget::mouseReleaseEvent(QMouseEvent * e)
{
  if (e->button() == Qt::MiddleButton || e->button() == Qt::LeftButton)
    {

      m_mouseButtonsPressed &= e->buttons();

      m_oldPos = QPoint(InvalidMousePoint);
      e->accept();
    }
  else
    {
      e->ignore();
    }
}

void  OgreWidget::keyPressEvent(QKeyEvent * e)
{
  m_keyState[e->key()] = true;
  if (e->key() == Qt::Key_Delete)
    {
      deleteSelection();
      update();
      e->accept();
    }
  else
    {
      e->ignore();
    }
}

void  OgreWidget::keyReleaseEvent(QKeyEvent * e)
{
  m_keyState[e->key()] = false;
  e->ignore();
}

/*
  TODO:
    -Implementer la selection de multiples objets.
*/

void  OgreWidget::mouseSelect(QPoint const & pos)
{
//  Ogre::Real x = pos.x() / (float)width();
//  Ogre::Real y = pos.y() / (float)height();

//  Ogre::Ray ray = m_camera->getCamera()->getCameraToViewportRay(x, y);
//  Ogre::RaySceneQuery * query = m_sceneManager->createRayQuery(ray);
//  Ogre::RaySceneQueryResult & queryResult = query->execute();
//  Ogre::RaySceneQueryResult::iterator queryResultIterator = queryResult.begin();
//  //Ogre::PlaneBoundedVolume volume = m_camera->getCameraToViewportBoxVolume();
//  //Ogre::PlaneBoundedVolumeListSceneQuery * query = m_sceneManager->createPlaneBoundedVolumeQuery(volume);
//  //Ogre::SceneQueryResult & queryResult = query->execute();

  Ogre::Entity *  selectedEntity = m_selectionBuffer->OnSelectionClick(pos.x(), pos.y());

  if (selectedEntity)
    selectItem(Ogre::any_cast<InstItem *>(selectedEntity->getParentSceneNode()->getUserObjectBindings().getUserAny()));
  else
    unselectItem();
}

void  OgreWidget::deleteSelection()
{
  for (int i = 0; i < m_selectionManager.getSelection().size(); ++i)
    m_currentEntity->deleteItem(m_selectionManager.getSelection()[i]->getRoot());
  m_selectionManager.clearSelection(true);
}

void  OgreWidget::initOgreSystem()
{
  m_ogreRoot = new Ogre::Root();

  Ogre::RenderSystem * renderSystem = m_ogreRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
  m_ogreRoot->setRenderSystem(renderSystem);
  m_ogreRoot->initialise(false);

  m_sceneManager = m_ogreRoot->createSceneManager(Ogre::ST_GENERIC);

  Ogre::NameValuePairList viewConfig;
  Ogre::String widgetHandle;

#ifdef Q_WS_WIN
  widgetHandle = Ogre::StringConverter::toString((size_t)winId());
  viewConfig["externalWindowHandle"] = widgetHandle;
#else
  QX11Info xInfo = x11Info();

  widgetHandle = Ogre::StringConverter::toString(reinterpret_cast<unsigned long>(xInfo.display())) +
      ":" + Ogre::StringConverter::toString(static_cast<unsigned int>(xInfo.screen())) +
      ":" + Ogre::StringConverter::toString(static_cast<unsigned long>(parentWidget()->winId()));
  viewConfig["parentWindowHandle"] = widgetHandle;
#endif

  m_renderWindow = m_ogreRoot->createRenderWindow("Ogre rendering window", width(), height(), false, &viewConfig);

#ifndef Q_WS_WIN
  WId window_id;
  m_renderWindow->getCustomAttribute("WINDOW", &window_id);
  create(window_id);
#endif

  m_camera = new Camera(m_sceneManager->createCamera("myCamera"));

  m_camera->getCamera()->setPosition(0, 0, -500);
  m_camera->getCamera()->lookAt(0, 0, 0);
  m_camera->getCamera()->setAspectRatio(Ogre::Real(width()) / Ogre::Real(height()));
  m_camera->getCamera()->setNearClipDistance(Ogre::Real(0.1));

  m_viewport = m_renderWindow->addViewport(m_camera->getCamera());
  m_viewport->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));

  m_selectionBuffer = new SelectionBuffer(m_sceneManager, m_renderWindow);

  initResources();
  initScene();
}

void  OgreWidget::initResources()
{
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");

  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements())
    {
      secName = seci.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
        {
          typeName = i->first;
          archName = i->second;
          Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void  OgreWidget::initScene()
{
  m_sceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));
}

/*
  TODO:
    -Surcharger la methode addItem pour ajouter des lights.
*/

//void  OgreWidget::addItem(Light const & entity)
//{
//  update();
//}
