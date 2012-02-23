#include "MaterialSwitcher.hpp"

MaterialSwitcher::MaterialSwitcher() : mLastTechnique(0)
{
  mCurrentColor = Ogre::ColourValue(0.0, 0.0, 0.0);
}

MaterialSwitcher::~MaterialSwitcher()
{
}

Ogre::Technique * MaterialSwitcher::handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String & schemeName,
                                                  Ogre::Material *originalMaterial, unsigned short lodIndex,
                                                  const Ogre::Renderable *rend)
{
  if(rend)
    {
      if(typeid(*rend) == typeid(Ogre::SubEntity))
        {
          const Ogre::SubEntity *subEntity = static_cast<const Ogre::SubEntity *>(rend);
          if(mLastEntity == subEntity->getParent()->getName())
            {
              const_cast<Ogre::SubEntity *>(subEntity)->setCustomParameter(1, Ogre::Vector4(mCurrentColor.r, mCurrentColor.g, mCurrentColor.b, 1.0));
            }
          else
            {
              Ogre::ResourcePtr res = Ogre::MaterialManager::getSingleton().load("PlainColor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
              mLastTechnique = static_cast<Ogre::MaterialPtr>(res)->getTechnique(0);

              getNextColor();
              const_cast<Ogre::SubEntity *>(subEntity)->setCustomParameter(1, Ogre::Vector4(mCurrentColor.r, mCurrentColor.g, mCurrentColor.b, 1.0));

              mLastEntity = subEntity->getParent()->getName();
              mColorDict[mCurrentColor] = mLastEntity;
            }
          return mLastTechnique;
        }
    }
  return 0;
}

const Ogre::String &  MaterialSwitcher::getEntityName(const Ogre::ColourValue &color) const
{
  ColorMapConstIter iter = mColorDict.find(color);
  if(iter != mColorDict.end())
    return (*iter).second;
  else
    return mEmptyString;
}

void MaterialSwitcher::getNextColor()
{
  Ogre::ARGB color = mCurrentColor.getAsARGB();
  color++;
  mCurrentColor.setAsARGB(color);
  // Alternatively, for an easier to interpret debug overlay with an
  // increased risk of dictionary collision:
  // mCurrentColor.setAsARGB(Ogre::Math::UnitRandom()*0x00FFFFFF + 0xFF000000);
}

void MaterialSwitcher::reset()
{
  mCurrentColor = Ogre::ColourValue(0.0, 0.0, 0.0);
  mLastEntity.clear();
  mColorDict.clear();
}
