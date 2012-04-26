#include "InstModel.hpp"
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

  for (int i = 0; i < scene->getRootSceneNode()->numChildren(); ++i)
    createDom(scene->getRootSceneNode()->getChild(i), elem);
}

void  SceneXML::save(QTextStream & out)
{
  m_doc.save(out, 2);
}

void  SceneXML::createDom(Ogre::Node * node, QDomElement & parent)
{
  QDomElement elem;

  if (node->getName() != "grid")
    {
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

      InstModel *  model = 0;

      if (static_cast<Ogre::SceneNode *>(node)->numAttachedObjects() > 0)
        model = dynamic_cast<InstModel *>(Ogre::any_cast<InstItem *>(node->getUserObjectBindings().getUserAny()));

      if (model)
        {
          elem = m_doc.createElement("entity");
          elem.setAttribute("name", model->getModel().getName());
          elem.setAttribute("meshFile", model->getModel().getPath());
          parent.lastChild().appendChild(elem);
        }
    }

  for (int i = 0; i < node->numChildren(); ++i)
    createDom(node->getChild(i), parent.lastChildElement());
}
