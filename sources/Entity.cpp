#include <QMessageBox>
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

bool  Entity::containEntity(Entity const * entity)
{
  if (entity == this)
    return (true);
  for (int i = 0; i < entity->getEntityList().size(); ++i)
    if (containEntity(&entity->getEntityList().at(i)->getEntity()))
      return (true);
  return (false);
}

void  Entity::createModel(Model const & model, Ogre::SceneManager * sceneManager)
{
  InstModel * instModel;

  try
  {
    if (sceneManager)
      instModel = new InstModel(model, sceneManager->getRootSceneNode());
    else
      instModel = new InstModel(model, 0);
  }
  catch (Ogre::Exception &)
  {
    QMessageBox::warning(0, "Instanciation Denied", "You have to add the path to the model's directory in your \"ressources.cfg\" file.");
    return ;
  }
  m_modelList.append(instModel);
}

void  Entity::createEntity(Entity & entity, Ogre::SceneManager * sceneManager)
{
  if (sceneManager)
    m_entityList.append(new InstEntity(entity, sceneManager->getRootSceneNode()));
  else
    m_entityList.append(new InstEntity(entity, 0));
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
      foreach (InstModel * instModel, m_modelList)
        instModel->load(node);
      foreach (InstEntity * instEntity, m_entityList)
        if (m_tags.isEmpty())
          instEntity->load(node);
        else
          for (int i = 0; i < m_tags.size(); ++i)
            if (instEntity->getTags().contains(m_tags[i]))
              {
                instEntity->load(node);
                break ;
              }
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
            if (m_tags.isEmpty())
              m_instEntity->load(node);
            else
              foreach (QString const & str, m_tags)
                if (m_instEntity->getTags().contains(str))
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

void  Entity::setTags(QStringList const & tags)
{
  m_tags = tags;
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
  if (idx < m_entityList.size())
    return (m_entityList[idx]);
  else
    return (m_modelList[idx - m_entityList.size()]);
}

int Entity::getInstNothingProbability(void) const
{
  return (m_instNothingProbability);
}

QStringList const & Entity::getTags(void) const
{
  return (m_tags);
}
