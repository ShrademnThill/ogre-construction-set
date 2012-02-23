#ifndef MATERIALSWITCHER_HPP
#define MATERIALSWITCHER_HPP

#include <Ogre.h>

class SelectionBuffer;

struct cmp_ColourValue
{
  bool operator()(const Ogre::ColourValue & a, const Ogre::ColourValue & b) const
  {
    return a.getAsBGRA() < b.getAsBGRA();
  }
};

class MaterialSwitcher : public Ogre::MaterialManager::Listener
{
private:
  typedef std::map<Ogre::ColourValue, Ogre::String, cmp_ColourValue> ColorMap;
  typedef std::map<Ogre::ColourValue, Ogre::String, cmp_ColourValue>::const_iterator ColorMapConstIter;

  Ogre::String                mEmptyString;
  Ogre::ColourValue           mCurrentColor;
  Ogre::String                mLastEntity;
  Ogre::Technique *           mLastTechnique;
  MaterialSwitcher::ColorMap  mColorDict;

  void getNextColor();

  friend class SelectionBuffer;

public:
  MaterialSwitcher();
  ~MaterialSwitcher();

  virtual Ogre::Technique * handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String & schemeName,
                                                 Ogre::Material * originalMaterial, unsigned short lodIndex,
                                                 const Ogre::Renderable * rend);

  const Ogre::String &  getEntityName(const Ogre::ColourValue & color) const;

  void  reset();
};

#endif // MATERIALSWITCHER_HPP
