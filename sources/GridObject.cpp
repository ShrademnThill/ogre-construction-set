#include "GridObject.hpp"

GridObject::GridObject()
{
}

Ogre::ManualObject *  GridObject::createGrid(Ogre::SceneManager * scene)
{
  addMaterial("Grid", Ogre::ColourValue(1, 1, 1, 0.75), Ogre::SBT_TRANSPARENT_ALPHA);

  Ogre::ManualObject * grid = scene->createManualObject();

  grid->begin("Grid", Ogre::RenderOperation::OT_LINE_LIST);
  for (Ogre::Real i = -10; i <= 10; ++i)
    {
      grid->position(-10, 0, i);
      grid->position(10, 0, i);
      grid->position(i, 0, -10);
      grid->position(i, 0, 10);
    }
  grid->end();
  grid->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
  return (grid);
}

void  GridObject::addMaterial(Ogre::String const & mat, Ogre::ColourValue const & clr, Ogre::SceneBlendType sbt)
{
  static int init = false;

  if(init)
    return ;
  else
    init = true;

  Ogre::MaterialPtr matptr = Ogre::MaterialManager::getSingleton().create(mat, "General");

  matptr->setReceiveShadows(false);
  matptr->getTechnique(0)->setLightingEnabled(true);
  matptr->getTechnique(0)->getPass(0)->setDiffuse(clr);
  matptr->getTechnique(0)->getPass(0)->setAmbient(clr);
  matptr->getTechnique(0)->getPass(0)->setSelfIllumination(clr);
  matptr->getTechnique(0)->getPass(0)->setSceneBlending(sbt);
  matptr->getTechnique(0)->getPass(0)->setLightingEnabled(false);
  matptr->getTechnique(0)->getPass(0)->setVertexColourTracking(Ogre::TVC_DIFFUSE);
}
