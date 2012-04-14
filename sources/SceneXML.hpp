#ifndef SCENEXML_HPP
#define SCENEXML_HPP

#include <QDomDocument>
#include <QTextStream>
#include <Ogre.h>

class SceneXML
{
public:
  SceneXML(Ogre::SceneManager * scene);

  void  save(QTextStream & out);

private:
  void  createDom(Ogre::Node * node, QDomElement & parent);

  Ogre::SceneManager * m_scene;
  QDomDocument         m_doc;
};

#endif // SCENEXML_HPP
