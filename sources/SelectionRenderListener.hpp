#ifndef SELECTIONRENDERLISTENER_HPP
#define SELECTIONRENDERLISTENER_HPP

#include <Ogre.h>

class MaterialSwitcher;

/**
 We need this attached to the depth target, otherwise we get problems with the compositor
 MaterialSwitcher should NOT be running all the time - rather only when we're
 specifically rendering the target that needs it
*/
class SelectionRenderListener : public Ogre::RenderTargetListener
{
private:
  MaterialSwitcher *  mMaterialListener;

public:
  SelectionRenderListener(MaterialSwitcher * switcher);
  ~SelectionRenderListener();

  virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent & evt);
  virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent & evt);
};

#endif // SELECTIONRENDERLISTENER_HPP
