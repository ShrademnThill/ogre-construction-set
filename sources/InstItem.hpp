#ifndef INSTITEM_HPP
#define INSTITEM_HPP

#include <Ogre.h>
#include <QStringList>

enum ItemType
{
  EntityType,
  ModelType,
  LightType
};

class InstItem
{
public:
  InstItem();
  virtual ~InstItem();

  Ogre::SceneNode *   getRoot();
  int                 getInstanciationProbability(void) const;
  QStringList const & getTags(void) const;
  ItemType            getType(void) const;
  Ogre::Vector3       getPosition(void) const;
  Ogre::Vector3 &     getPosition(void);
  Ogre::Quaternion    getOrientation(void) const;
  Ogre::Quaternion &  getOrientation(void);
  Ogre::Vector3       getScale(void) const;
  Ogre::Vector3 &     getScale(void);

  void  setTags(QStringList const & tags);
  void  setRoot(Ogre::SceneNode * node);
  void  setInstanciationProbability(unsigned int value);

protected:
  Ogre::SceneNode * m_root;
  Ogre::Vector3     m_position;
  Ogre::Quaternion  m_orientation;
  Ogre::Vector3     m_scale;
  int               m_instanciationProbability;
  ItemType          m_type;

private:
  QStringList m_tags;
};

#endif // INSTITEM_HPP
