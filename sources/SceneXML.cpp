#include "SceneXML.hpp"

SceneXML::SceneXML(Ogre::SceneManager * scene) :
  m_scene(scene)
{
  QDomElement elem;

  elem = m_doc.createElement("scene");
  m_doc.appendChild(elem);
  elem.setAttribute("formatVersion", "1.0");

  elem = m_doc.createElement("nodes");
  m_doc.lastChild().appendChild(elem);

  createDom(scene->getRootSceneNode()->getChild(0), elem);
}

void  SceneXML::save(QTextStream & out)
{
  m_doc.save(out, 2);
}

void  SceneXML::createDom(Ogre::Node * node, QDomElement & parent)
{
  QDomElement elem;

  elem = m_doc.createElement("node");
  parent.appendChild(elem);
  elem.setAttribute("name", node->getName().c_str());

  elem = m_doc.createElement("position");
  elem.setAttribute("x", node->getPosition().x);
  elem.setAttribute("y", node->getPosition().y);
  elem.setAttribute("z", node->getPosition().z);
  parent.lastChild().appendChild(elem);

  elem = m_doc.createElement("rotation");
  elem.setAttribute("qx", node->getOrientation().x);
  elem.setAttribute("qy", node->getOrientation().y);
  elem.setAttribute("qz", node->getOrientation().z);
  elem.setAttribute("qw", node->getOrientation().w);
  parent.lastChild().appendChild(elem);

  elem = m_doc.createElement("scale");
  elem.setAttribute("x", node->getScale().x);
  elem.setAttribute("y", node->getScale().y);
  elem.setAttribute("z", node->getScale().z);
  parent.lastChild().appendChild(elem);

  Ogre::MovableObject * object = 0;

  if (static_cast<Ogre::SceneNode *>(node)->numAttachedObjects() > 0)
    object = static_cast<Ogre::SceneNode *>(node)->getAttachedObject(0);

  if (object)
    {
      elem = m_doc.createElement("entity");
      elem.setAttribute("name", object->getName().c_str());
      elem.setAttribute("meshFile", QString(object->getName().c_str()) + ".mesh");
      elem.setAttribute("static", node->getScale().z);
      parent.lastChild().appendChild(elem);
    }

  for (int i = 0; i < node->numChildren(); ++i)
    if (node->getChild(i)->getName() != "grid")
      createDom(node->getChild(i), parent.lastChildElement());
}
