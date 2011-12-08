#include "OgreWidget.hpp"
#include "InstModel.hpp"

const QPoint     OgreWidget::invalidMousePoint(-1,-1);
const Ogre::Real OgreWidget::turboModifier(10);

OgreWidget::OgreWidget(QWidget *parent)
  : QWidget(parent),
    m_ogreRoot(0),
    m_sceneManager(0),
    m_renderWindow(0),
    m_viewport(0),
    m_camera(0),
    oldPos(invalidMousePoint),
    selectedNode(0)
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

void OgreWidget::setBackgroundColor(QColor c)
{
  if(m_viewport)
    {
      Ogre::ColourValue ogreColour;
      ogreColour.setAsARGB(c.rgba());
      m_viewport->setBackgroundColour(ogreColour);
    }
}

void OgreWidget::setCameraPosition(const Ogre::Vector3 &pos)
{
  m_camera->setPosition(pos);
  //ogreCamera->lookAt(0,50,0);
  update();
  emit cameraPositionChanged(pos);
}

void OgreWidget::keyPressEvent(QKeyEvent *e)
{
  static QMap<int, Ogre::Vector3> keyCoordModificationMapping;
  static bool mappingInitialised = false;

  if(!mappingInitialised)
    {
      keyCoordModificationMapping[Qt::Key_W] = Ogre::Vector3( 0, 0,-5);
      keyCoordModificationMapping[Qt::Key_S] = Ogre::Vector3( 0, 0, 5);
      keyCoordModificationMapping[Qt::Key_A] = Ogre::Vector3(-5, 0, 0);
      keyCoordModificationMapping[Qt::Key_D] = Ogre::Vector3( 5, 0, 0);
      keyCoordModificationMapping[Qt::Key_PageUp] = Ogre::Vector3( 0, 5, 0);
      keyCoordModificationMapping[Qt::Key_PageDown] = Ogre::Vector3( 0,-5, 0);

      mappingInitialised = true;
    }

  QMap<int, Ogre::Vector3>::iterator keyPressed = keyCoordModificationMapping.find(e->key());
  if(keyPressed != keyCoordModificationMapping.end() && m_camera)
    {
      Ogre::Vector3 newCamPos = m_camera->getPosition() + keyPressed.value();
      setCameraPosition(newCamPos);
      e->accept();
    }
  else
    {
      e->ignore();
    }
}

void OgreWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
  if(e->buttons().testFlag(Qt::LeftButton))
    {
      Ogre::Real x = e->pos().x() / (float)width();
      Ogre::Real y = e->pos().y() / (float)height();

      Ogre::Ray ray = m_camera->getCameraToViewportRay(x, y);
      Ogre::RaySceneQuery * query = m_sceneManager->createRayQuery(ray);
      Ogre::RaySceneQueryResult & queryResult = query->execute();
      Ogre::RaySceneQueryResult::iterator queryResultIterator = queryResult.begin();

      if(queryResultIterator != queryResult.end())
        {
          if(queryResultIterator->movable)
            {
              selectedNode = queryResultIterator->movable->getParentSceneNode();
              selectedNode->showBoundingBox(true);
            }
        }
      else
        {
          selectedNode->showBoundingBox(false);
          selectedNode = 0;
        }

      m_sceneManager->destroyQuery(query);

      update();
      e->accept();
    }
  else
    {
      e->ignore();
    }
}

void OgreWidget::mouseMoveEvent(QMouseEvent *e)
{
  if(e->buttons().testFlag(Qt::LeftButton) && oldPos != invalidMousePoint)
    {
      const QPoint &pos = e->pos();
      Ogre::Real deltaX = pos.x() - oldPos.x();
      Ogre::Real deltaY = pos.y() - oldPos.y();

      if(e->modifiers().testFlag(Qt::ControlModifier))
        {
          deltaX *= turboModifier;
          deltaY *= turboModifier;
        }

      Ogre::Vector3 camTranslation(deltaX, deltaY, 0);
      const Ogre::Vector3 &actualCamPos = m_camera->getPosition();
      setCameraPosition(actualCamPos + camTranslation);

      oldPos = pos;
      e->accept();
    }
  else
    {
      e->ignore();
    }
}

void OgreWidget::mousePressEvent(QMouseEvent *e)
{
  if(e->buttons().testFlag(Qt::LeftButton))
    {
      oldPos = e->pos();
      e->accept();
    }
  else
    {
      e->ignore();
    }
}

void OgreWidget::mouseReleaseEvent(QMouseEvent *e)
{
  if(!e->buttons().testFlag(Qt::LeftButton))
    {
      oldPos = QPoint(invalidMousePoint);
      e->accept();
    }
  else
    {
      e->ignore();
    }
}

void OgreWidget::moveEvent(QMoveEvent *e)
{
  QWidget::moveEvent(e);

  if(e->isAccepted() && m_renderWindow)
    {
      m_renderWindow->windowMovedOrResized();
      update();
    }
}

QPaintEngine * OgreWidget::paintEngine() const
{
  return NULL;
}

void OgreWidget::paintEvent(QPaintEvent *e)
{
  m_ogreRoot->_fireFrameStarted();
  m_renderWindow->update();
  m_ogreRoot->_fireFrameEnded();

  e->accept();
}

void OgreWidget::resizeEvent(QResizeEvent *e)
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

void OgreWidget::showEvent(QShowEvent *e)
{
  QWidget::showEvent(e);

  if(!m_ogreRoot)
    {
      initOgreSystem();
    }
}

void OgreWidget::wheelEvent(QWheelEvent *e)
{
  Ogre::Vector3 zTranslation(0,0, -e->delta() / 60);

  if(e->modifiers().testFlag(Qt::ControlModifier))
    {
      zTranslation.z *= turboModifier;
    }

  const Ogre::Vector3 &actualCamPos = m_camera->getPosition();
  setCameraPosition(actualCamPos + zTranslation);

  e->accept();
}

void OgreWidget::initOgreSystem()
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

  widgetHandle = Ogre::StringConverter::toString(static_cast<unsigned long>(xInfo.display())) +
      ":" + Ogre::StringConverter::toString(static_cast<unsigned int>(xInfo.screen())) +
      ":" + Ogre::StringConverter::toString(static_cast<unsigned long>(parentWidget()->winId()));
#endif
  viewConfig["externalWindowHandle"] = widgetHandle;
  m_renderWindow = m_ogreRoot->createRenderWindow("Ogre rendering window", width(), height(), false, &viewConfig);

  m_camera = m_sceneManager->createCamera("myCamera");
  Ogre::Vector3 camPos(300, 600, 300);
  m_camera->setPosition(camPos);
  m_camera->lookAt(0,0,0);
  emit cameraPositionChanged(camPos);

  m_viewport = m_renderWindow->addViewport(m_camera);
  m_viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
  m_camera->setAspectRatio(Ogre::Real(width()) / Ogre::Real(height()));

  initResources();
  initScene();
}

void OgreWidget::initResources()
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

void OgreWidget::initScene()
{
  m_sceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));

//  Ogre::Entity * robotEntity = m_sceneManager->createEntity("media/models/ninja.mesh");
//  Ogre::SceneNode * robotNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
//  robotNode->pitch(Ogre::Radian(Ogre::Degree(0)));
//  robotNode->setPosition(-100, 0, 0);
//  robotNode->attachObject(robotEntity);
}

//void OgreWidget::changeCurrentEntity(Entity const * entity)
//{
//  m_sceneManager->getRootSceneNode()->removeChild(m_currentEntity->getRoot());
//  m_sceneManager->getRootSceneNode()->addChild(entity->getRoot());
//}

void OgreWidget::addModel(Model const & model)
{
  InstModel * instance = m_currentEntity->createModel(model);
  Ogre::Entity * entity = m_sceneManager->createEntity("../../" + model.getPath().toStdString());
  Ogre::SceneNode * node = m_sceneManager->getRootSceneNode()->createChildSceneNode();

  node->attachObject(entity);
  instance->setRoot(node);
}
