#ifndef Q_WS_WIN
#include <QX11Info>
#endif

#include "OgreWidget.hpp"
#include "InstModel.hpp"

const QPoint     OgreWidget::InvalidMousePoint(-1,-1);

OgreWidget::OgreWidget(QWidget * parent)
  : QWidget(parent),
    m_ogreRoot(0),
    m_sceneManager(0),
    m_renderWindow(0),
    m_viewport(0),
    m_camera(0),
    m_oldPos(InvalidMousePoint),
    m_selectedNode(0),
    m_selecting(false)
{
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_PaintOnScreen);
  setFocusPolicy(Qt::ClickFocus);

  //TEST TEMPORAIRE
  //L'entite root devra etre instancie dans le la list des entites, et uniquement agree ici.
  m_currentEntity = new Entity;
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
#else
  QX11Info xInfo = x11Info();

  widgetHandle = Ogre::StringConverter::toString(reinterpret_cast<unsigned long>(xInfo.display())) +
      ":" + Ogre::StringConverter::toString(static_cast<unsigned int>(xInfo.screen())) +
      ":" + Ogre::StringConverter::toString(static_cast<unsigned long>(winId()));
#endif
  viewConfig["externalWindowHandle"] = widgetHandle;
  m_renderWindow = m_ogreRoot->createRenderWindow("Ogre rendering window", width(), height(), false, &viewConfig);

  m_camera = static_cast<Camera *>(m_sceneManager->createCamera("myCamera"));
  m_camera->zoom(-500);

  m_viewport = m_renderWindow->addViewport(m_camera);
  m_viewport->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));
  m_camera->setAspectRatio(Ogre::Real(width()) / Ogre::Real(height()));

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

  Ogre::Entity *    robotEntity = m_sceneManager->createEntity("../data/models/ninja.mesh");
  Ogre::SceneNode * robotNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
  robotNode->pitch(Ogre::Radian(Ogre::Degree(0)));
  robotNode->setPosition(-100, 0, 0);
  robotNode->attachObject(robotEntity);
}

void  OgreWidget::setBackgroundColor(QColor c)
{
  if(m_viewport)
    {
      Ogre::ColourValue ogreColour;
      ogreColour.setAsARGB(c.rgba());
      m_viewport->setBackgroundColour(ogreColour);
    }
}

void  OgreWidget::mouseMoveEvent(QMouseEvent * e)
{
  if(!m_selecting && m_oldPos != InvalidMousePoint)
    {
      Ogre::Real deltaX = e->pos().x() - m_oldPos.x();
      Ogre::Real deltaY = e->pos().y() - m_oldPos.y();

      if(e->modifiers().testFlag(Qt::ControlModifier))
        m_camera->zoom(-deltaY);
      else if (e->modifiers().testFlag(Qt::AltModifier))
        m_camera->rotate(deltaX, deltaY);
      else
        m_camera->shift(-deltaX, -deltaY);
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
        m_selecting = true;
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
      if (e->button() == Qt::LeftButton)
        {
          mouseSelect(e->pos());
          m_selecting = false;
        }
      m_oldPos = QPoint(InvalidMousePoint);
      e->accept();
    }
  else
    {
      e->ignore();
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

void  OgreWidget::keyPressEvent(QKeyEvent * e)
{
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

QPaintEngine *  OgreWidget::paintEngine() const
{
  return NULL;
}

void  OgreWidget::paintEvent(QPaintEvent * e)
{
  m_ogreRoot->_fireFrameStarted();
  m_renderWindow->update();
  m_ogreRoot->_fireFrameEnded();

  e->accept();
}

void  OgreWidget::resizeEvent(QResizeEvent * e)
{
  QWidget::resizeEvent(e);

  if(e->isAccepted())
    {
      const QSize &newSize = e->size();
      if(m_renderWindow)
        {
          m_renderWindow->resize(newSize.width(), newSize.height());
          m_renderWindow->windowMovedOrResized();
        }
      if(m_camera)
        {
          Ogre::Real aspectRatio = Ogre::Real(newSize.width()) / Ogre::Real(newSize.height());
          m_camera->setAspectRatio(aspectRatio);
        }
    }
}

void  OgreWidget::showEvent(QShowEvent * e)
{
  QWidget::showEvent(e);

  if(!m_ogreRoot)
    {
      initOgreSystem();
    }
}

/*
  TODO:
    -Emetre un signal pour debloquer l'info panel.
    -Implementer la selection de multiples objets.
    -L'attribut m_selectedNode devrait se transformer en list de node.
*/

void  OgreWidget::mouseSelect(QPoint const & pos)
{
  Ogre::Real x = pos.x() / (float)width();
  Ogre::Real y = pos.y() / (float)height();

  Ogre::Ray ray = m_camera->getCameraToViewportRay(x, y);
  Ogre::RaySceneQuery * query = m_sceneManager->createRayQuery(ray);
  Ogre::RaySceneQueryResult & queryResult = query->execute();
  Ogre::RaySceneQueryResult::iterator queryResultIterator = queryResult.begin();

  //Ogre::PlaneBoundedVolume volume = m_camera->getCameraToViewportBoxVolume();
  //Ogre::PlaneBoundedVolumeListSceneQuery * query = m_sceneManager->createPlaneBoundedVolumeQuery(volume);
  //Ogre::SceneQueryResult & queryResult = query->execute();

  if(queryResultIterator != queryResult.end() && queryResultIterator->movable)
    {
      m_selectedNode = queryResultIterator->movable->getParentSceneNode();
      m_selectedNode->showBoundingBox(true);
    }
  else if (m_selectedNode)
    {
      m_selectedNode->showBoundingBox(false);
      m_selectedNode = 0;
    }
  m_sceneManager->destroyQuery(query);
  //emit itemsSelected(true);
  update();
}

void  OgreWidget::changeCurrentEntity(Entity * entity)
{
  m_currentEntity->unload(m_sceneManager);
  m_currentEntity = entity;
  m_currentEntity->load(m_sceneManager);
}

void  OgreWidget::addItem(Model const & model)
{
  m_currentEntity->createModel(model, m_sceneManager);
  update();
}

void  OgreWidget::deleteSelection()
{
  if (m_selectedNode)
    {
      m_currentEntity->deleteItem(m_selectedNode);
      m_selectedNode = 0;
      update();
    }
}

/*
  TODO:
    -Surcharger la methode addItem pour ajouter des lights.
    -Surcharger la methode addItem pour ajouter des entities.
    -Ajouter la methode pour deplacer un Item.
*/

//void  OgreWidget::addItem(Light const & entity)
//{
//  update();
//}

//void  OgreWidget::addItem(Entity const & entity)
//{
//  update();
//}

//void  OgreWidget::moveItem(Ogre::Real x, Ogre::Real y, Ogre::Real z)
//{
//  update();
//}
