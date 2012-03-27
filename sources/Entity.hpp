#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <QList>
#include <QString>
#include <QStringList>
#include "Item.hpp"
#include "Model.hpp"
#include "InstModel.hpp"
#include "InstEntity.hpp"

class InstEntity;

class Entity : public Item
{
public:
  Entity(QString const & name = "");
  ~Entity();

  bool  containEntity(Entity const * entity);

  void  createModel(Model const & model, Ogre::SceneManager * sceneManager);
  void  createEntity(Entity & entity, Ogre::SceneManager * sceneManager);
  void  deleteItem(Ogre::SceneNode * node);
  void  load(Ogre::SceneNode * node);
  void  unload(Ogre::SceneNode *);
  void  setName(QString const &);
  void  setComposed(bool);
  void  setInstNothingProbability(int value);
  void  setTags(QStringList const & tags);

  QString const &             getName(void) const;
  bool                        isComposed(void) const;
  QList<InstModel *> const &  getModelList(void) const;
  QList<InstEntity *> const & getEntityList(void) const;
  InstItem *                  getItem(int idx);
  int                         getInstNothingProbability(void) const;
  QStringList const &         getTags(void) const;

private:
  QString             m_name;
  bool                m_composed;
  QList<InstModel *>  m_modelList;
  QList<InstEntity *> m_entityList;
  InstModel *         m_instModel;
  InstEntity *        m_instEntity;
  int                 m_instNothingProbability;
  QStringList         m_tags;
};

#endif // ENTITY_HPP
