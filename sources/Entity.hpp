#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <QList>
#include <QString>
#include "Model.hpp"
#include "InstModel.hpp"

class Entity
{
public:
  Entity(QString const & name = "");
  ~Entity();

  InstModel *       createModel(Model const & model, Ogre::SceneManager * sceneManager);
  void              deleteItem(Ogre::SceneNode * node);
  void              load(Ogre::SceneManager * sceneManager) const;
  void              unload(Ogre::SceneManager * sceneManager) const;
  void              setName(QString const &);

  QString const &   getName(void) const;

private:
  QString             m_name;
  QList<InstModel *>  m_modelList;
};

#endif // ENTITY_HPP
