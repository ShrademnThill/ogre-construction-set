#include "MaterialSwitcher.hpp"
#include "SelectionRenderListener.hpp"
#include "SelectionBuffer.hpp"

SelectionBuffer::SelectionBuffer(Ogre::SceneManager *sceneMgr, Ogre::RenderTarget *renderTarget) :
  mSceneMgr(sceneMgr),
  mRenderTarget(renderTarget),
  mBuffer(0),
  mPixelBox(0)
{
  mCamera = mSceneMgr->getCamera("myCamera");

  mMaterialSwitchListener = new MaterialSwitcher();

  mSelectionTargetListener = new SelectionRenderListener(mMaterialSwitchListener);

  unsigned int width = mRenderTarget->getWidth();
  unsigned int height = mRenderTarget->getHeight();

  mTexture = Ogre::TextureManager::getSingleton().createManual("SelectionPassTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                               Ogre::TEX_TYPE_2D, width, height, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

  mRenderTexture = mTexture->getBuffer()->getRenderTarget();
  mRenderTexture->setAutoUpdated(false);
  mRenderTexture->setPriority(0);
  mRenderTexture->addViewport(mCamera);
  mRenderTexture->getViewport(0)->setOverlaysEnabled(false);
  mRenderTexture->getViewport(0)->setClearEveryFrame(true);
  mRenderTexture->addListener(mSelectionTargetListener);
  mRenderTexture->getViewport(0)->setMaterialScheme("aa");

  Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mTexture->getBuffer();
  size_t bufferSize = pixelBuffer->getSizeInBytes();

  mBuffer = new Ogre::uint8[bufferSize];
  mPixelBox = new Ogre::PixelBox(pixelBuffer->getWidth(), pixelBuffer->getHeight(),
                                 pixelBuffer->getDepth(), pixelBuffer->getFormat(), mBuffer);

  //CreateRTTOverlays();
}

SelectionBuffer::~SelectionBuffer()
{
  Ogre::TextureManager::getSingleton().unload("SelectionPassTex");
  delete mPixelBox;
  delete[] mBuffer;
  delete mSelectionTargetListener;
  delete mMaterialSwitchListener;
}

void SelectionBuffer::Update()
{
  UpdateBufferSize();

  mMaterialSwitchListener->reset();
  mRenderTexture->update();

  mRenderTexture->copyContentsToMemory(*mPixelBox, Ogre::RenderTarget::FB_FRONT);
}

void SelectionBuffer::UpdateBufferSize()
{
  unsigned int width = mRenderTarget->getWidth();
  unsigned int height = mRenderTarget->getHeight();

  if(width != mRenderTexture->getWidth() || height != mRenderTexture->getHeight())
    {
      Ogre::TextureManager::getSingleton().unload("SelectionPassTex");
      delete[] mBuffer;
      delete mPixelBox;

      mTexture = Ogre::TextureManager::getSingleton().createManual("SelectionPassTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                   Ogre::TEX_TYPE_2D, width, height, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

      mRenderTexture = mTexture->getBuffer()->getRenderTarget();
      mRenderTexture->setAutoUpdated(false);
      mRenderTexture->setPriority(0);
      mRenderTexture->addViewport(mCamera);
      mRenderTexture->getViewport(0)->setOverlaysEnabled(false);
      mRenderTexture->getViewport(0)->setClearEveryFrame(true);
      mRenderTexture->addListener(mSelectionTargetListener);
      mRenderTexture->getViewport(0)->setMaterialScheme("aa");

      Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mTexture->getBuffer();
      size_t bufferSize = pixelBuffer->getSizeInBytes();

      mBuffer = new Ogre::uint8[bufferSize];
      mPixelBox = new Ogre::PixelBox(pixelBuffer->getWidth(), pixelBuffer->getHeight(),
                                     pixelBuffer->getDepth(), pixelBuffer->getFormat(), mBuffer);
    }
}

Ogre::Entity *  SelectionBuffer::OnSelectionClick(int x, int y)
{
  Update();

  size_t posInStream = (mPixelBox->getWidth() * y) * 4;
  posInStream += x * 4;

  Ogre::BGRA color(0);
  memcpy((void *)&color, mBuffer+posInStream, 4);
  Ogre::ColourValue cv;
  cv.setAsARGB(color);
  cv.a = 1.0f;
  const Ogre::String &entName = mMaterialSwitchListener->getEntityName(cv);
  if(entName.empty())
    return 0;
  else
    return mSceneMgr->getEntity(entName);
}

void SelectionBuffer::CreateRTTOverlays()
{
  Ogre::MaterialPtr baseWhite = Ogre::MaterialManager::getSingleton().getDefaultSettings();
  Ogre::MaterialPtr selectionBufferTexture = baseWhite->clone("SelectionDebugMaterial");
  Ogre::TextureUnitState *textureUnit = selectionBufferTexture->getTechnique(0)->getPass(0)->createTextureUnitState();

  textureUnit->setTextureName("SelectionPassTex");

  Ogre::OverlayManager *mgr = Ogre::OverlayManager::getSingletonPtr();
  mSelectionDebugOverlay = mgr->create("SelectionDebugOverlay");

  Ogre::OverlayContainer *panel = static_cast<Ogre::OverlayContainer *>(mgr->createOverlayElement("Panel", "SelectionDebugPanel"));
  panel->setMetricsMode(Ogre::GMM_PIXELS);
  panel->setPosition(10, 10);
  panel->setDimensions(400, 280);
  panel->setMaterialName("SelectionDebugMaterial");
  mSelectionDebugOverlay->add2D(panel);

  mSelectionDebugOverlay->show();
}

void SelectionBuffer::ShowOverlay(bool show)
{
  if(show)
    mSelectionDebugOverlay->show();
  else
    mSelectionDebugOverlay->hide();
}
