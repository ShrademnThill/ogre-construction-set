#include "GridObject.hpp"

GridObject::GridObject()
{
}

Ogre::ManualObject *  GridObject::createGrid(Ogre::SceneManager * scene, Ogre::Real)
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

//void  AxisObject::addBox(ManualObject * obj, Vector3 dim, Vector3 pos, ColourValue color, short boxMask)
//{
//  if(!obj)
//    return ;

//  obj->begin("Grid", Ogre::RenderOperation::OT_TRIANGLE_LIST);

//  dim /= 2;

//  Ogre::Real l = dim.x;
//  Ogre::Real h = dim.y;
//  Ogre::Real w = dim.z;

//  obj->position(Ogre::Vector3(-l, h, w) + pos);
//  obj->colour(color);
//  obj->position(Ogre::Vector3(-l, -h, w) + pos);
//  obj->colour(color);
//  obj->position(Ogre::Vector3(l, -h, w) + pos);
//  obj->colour(color);
//  obj->position(Ogre::Vector3(l, h, w) + pos);

//  obj->position(Ogre::Vector3(-l, h, -w) + pos);
//  obj->colour(color);
//  obj->position(Ogre::Vector3(-l, -h, -w) + pos);
//  obj->colour(color);
//  obj->position(Ogre::Vector3(l, -h, -w) + pos);
//  obj->colour(color);
//  obj->position(Ogre::Vector3(l, h, -w) + pos);

//  // front back
//  if(boxMask & BOX_FRONT)
//    obj->quad(0, 1, 2, 3);
//  if(boxMask & BOX_BACK)
//    obj->quad(7, 6, 5, 4);

//  // top bottom
//  if(boxMask & BOX_TOP)
//    obj->quad(0, 3, 7, 4);
//  if(boxMask & BOX_BOT)
//    obj->quad(2, 1, 5, 6);

//  // end caps
//  if(boxMask & BOX_RIGHT)
//    obj->quad(1, 0, 4, 5);
//  if(boxMask & BOX_LEFT)
//    obj->quad(3, 2, 6, 7);

//  obj->end();
//}

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
