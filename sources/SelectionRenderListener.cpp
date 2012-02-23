#include "SelectionRenderListener.hpp"
#include "MaterialSwitcher.hpp"

SelectionRenderListener::SelectionRenderListener(MaterialSwitcher *switcher) :
  mMaterialListener(switcher)
{
}

SelectionRenderListener::~SelectionRenderListener()
{
  delete mMaterialListener;
}

void SelectionRenderListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent &evt)
{
  Ogre::MaterialManager::getSingleton().addListener(mMaterialListener);
}

void SelectionRenderListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent &evt)
{
  Ogre::MaterialManager::getSingleton().removeListener(mMaterialListener);
}
