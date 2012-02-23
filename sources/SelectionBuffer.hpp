#ifndef SELECTIONBUFFER_HPP
#define SELECTIONBUFFER_HPP

#include <Ogre.h>

class MaterialSwitcher;
class SelectionRenderListener;

class SelectionBuffer
{
private:
  Ogre::SceneManager *  mSceneMgr;
  Ogre::Camera *        mCamera;
  Ogre::RenderTarget *  mRenderTarget;

  // This is the material listener - Note: it is controlled by a separate
  // RenderTargetListener, not applied globally to all targets
  MaterialSwitcher *  mMaterialSwitchListener;

  SelectionRenderListener * mSelectionTargetListener;

  Ogre::TexturePtr      mTexture;
  Ogre::RenderTexture * mRenderTexture;
  Ogre::uint8 *         mBuffer;
  Ogre::PixelBox *      mPixelBox;
  Ogre::Overlay *       mSelectionDebugOverlay;

  void  CreateRTTOverlays();
  void  Update();
  void  UpdateBufferSize();

public:
  SelectionBuffer(Ogre::SceneManager * sceneMgr, Ogre::RenderTarget * renderTarget);
  ~SelectionBuffer();

  Ogre::Entity *  OnSelectionClick(int x, int y);
  void            ShowOverlay(bool show);
};

#endif // SELECTIONBUFFER_HPP
