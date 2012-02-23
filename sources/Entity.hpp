#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <QList>
#include <QString>
#include "Model.hpp"
#include "InstModel.hpp"
#include "InstEntity.hpp"

class InstEntity;

class Entity
{
public:
  Entity(QString const & name = "");
  ~Entity();

  void  createModel(Model const & model, Ogre::SceneManager * sceneManager);
  void  createEntity(Entity const & entity, Ogre::SceneManager * sceneManager);
  void  deleteItem(Ogre::SceneNode * node);
  void  load(Ogre::SceneNode * node) const;
  void  unload(Ogre::SceneNode * node) const;
  void  setName(QString const &);
  void  setComposed(bool);

  QString const &             getName(void) const;
  bool                        isComposed(void) const;
  QList<InstModel *> const &  getModelList(void) const;
  QList<InstEntity *> const & getEntityList(void) const;

private:
  QString             m_name;
  bool                m_composed;
  QList<InstModel *>  m_modelList;
  QList<InstEntity *> m_entityList;
};

#endif // ENTITY_HPP
