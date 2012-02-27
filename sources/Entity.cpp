#include "Entity.hpp"

Entity::Entity(QString const & name) :
  m_name(name),
  m_composed(true),
  m_instNothingProbability(100)
{
}

Entity::~Entity()
{
}

void  Entity::createModel(Model const & model, Ogre::SceneManager * sceneManager)
{
  m_modelList.append(new InstModel(model, sceneManager->getRootSceneNode()));
}

void  Entity::createEntity(Entity & entity, Ogre::SceneManager * sceneManager)
{
  m_entityList.append(new InstEntity(entity, sceneManager->getRootSceneNode()));
}

void  Entity::deleteItem(Ogre::SceneNode * node)
{
  for (int i = 0; i < m_modelList.size(); ++i)
    if (m_modelList[i]->getRoot() == node)
      delete m_modelList.takeAt(i);
  for (int i = 0; i < m_entityList.size(); ++i)
    if (m_entityList[i]->getRoot() == node)
      delete m_entityList.takeAt(i);
}

void  Entity::load(Ogre::SceneNode * node)
{
  if (m_composed)
    {
      for (int i = 0; i < m_modelList.size(); ++i)
        m_modelList[i]->load(node);
      for (int i = 0; i < m_entityList.size(); ++i)
        m_entityList[i]->load(node);
    }
  else
    {
      int temp = 0;
      int proba = qrand() % 100;

      m_instModel = 0;
      for (int i = 0; i < m_modelList.size() && !m_instModel; ++i)
        {
          if (m_modelList[i]->getInstanciationProbability() + temp > proba)
            m_instModel = m_modelList[i];
          temp += m_modelList[i]->getInstanciationProbability();
        }
      if (m_instModel)
        m_instModel->load(node);
      else
        {
          m_instEntity = 0;
          for (int i = 0; i < m_entityList.size() && !m_instEntity; ++i)
            {
              if (m_entityList[i]->getInstanciationProbability() + temp > proba)
                m_instEntity = m_entityList[i];
              temp += m_entityList[i]->getInstanciationProbability();
            }
          if (m_instEntity)
            m_instEntity->load(node);
        }
    }
}

void  Entity::unload(Ogre::SceneNode *)
{
  for (int i = 0; i < m_modelList.size(); ++i)
    m_modelList[i]->unload();
  for (int i = 0; i < m_entityList.size(); ++i)
    m_entityList[i]->unload();
}

void  Entity::setName(QString const & name)
{
  m_name = name;
}

void  Entity::setComposed(bool composed)
{
  m_composed = composed;
}

void  Entity::setInstNothingProbability(int value)
{
  m_instNothingProbability = value;
}

QString const & Entity::getName(void) const
{
  return (m_name);
}

bool  Entity::isComposed(void) const
{
  return (m_composed);
}

QList<InstModel *> const &  Entity::getModelList(void) const
{
  return (m_modelList);
}

QList<InstEntity *> const & Entity::getEntityList(void) const
{
  return (m_entityList);
}

InstItem *  Entity::getItem(int idx)
{
  if (idx < m_modelList.size())
    return (m_modelList[idx]);
  else
    return (m_entityList[idx]);
}

int Entity::getInstNothingProbability(void) const
{
  return (m_instNothingProbability);
}
