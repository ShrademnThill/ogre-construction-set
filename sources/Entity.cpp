#include "DataManager.hpp"
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

  instModel = new InstModel(model);
  if (sceneManager)
    instModel->load(sceneManager->getRootSceneNode());
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

QDataStream & operator<<(QDataStream & out, Entity const & entity)
{
  QList<Entity *> entityList = DataManager::getSingleton()->getEntityList();

  out << entity.getName();
  out << entity.getInstNothingProbability();
  out << entity.getTags();
  out << entity.isComposed();
  out << entity.isDeleted();
  out << entity.getEntityList().size();
  foreach (InstEntity * instEntity, entity.getEntityList())
    {
      out << entityList.indexOf(&instEntity->getEntity());
      out << instEntity->getInstanciationProbability();
      out << instEntity->getTags();
      out << instEntity->getPosition().x;
      out << instEntity->getPosition().y;
      out << instEntity->getPosition().z;
      out << instEntity->getOrientation().w;
      out << instEntity->getOrientation().x;
      out << instEntity->getOrientation().y;
      out << instEntity->getOrientation().z;
      out << instEntity->getScale().x;
      out << instEntity->getScale().y;
      out << instEntity->getScale().z;
    }
  out << entity.getModelList().size();
  foreach (InstModel * instModel,  entity.getModelList())
    {
      out << instModel->getModel().getName();
      out << instModel->getModel().getPath();
      out << instModel->getInstanciationProbability();
      out << instModel->getTags();
      out << instModel->getPosition().x;
      out << instModel->getPosition().y;
      out << instModel->getPosition().z;
      out << instModel->getOrientation().w;
      out << instModel->getOrientation().x;
      out << instModel->getOrientation().y;
      out << instModel->getOrientation().z;
      out << instModel->getScale().x;
      out << instModel->getScale().y;
      out << instModel->getScale().z;
    }
  return (out);
}

QDataStream & operator>>(QDataStream & in, Entity & entity)
{
  QList<Entity *> entityList = DataManager::getSingleton()->getEntityList();
  QString         strTemp;
  QStringList     strListTemp;
  int             intTemp;
  bool            boolTemp;

  in >> strTemp;
  entity.setName(strTemp);
  in >> intTemp;
  entity.setInstNothingProbability(intTemp);
  in >> strListTemp;
  entity.setTags(strListTemp);
  in >> boolTemp;
  entity.setComposed(boolTemp);
  in >> boolTemp;
  entity.setDeleted(boolTemp);
  in >> intTemp;
  for (int i = 0; i < intTemp; ++i)
    {
      int           temp;

      in >> temp;
      if (temp < 0 || temp >= entityList.size())
        temp = 0;
      entity.createEntity(*entityList[temp], 0);
      in >> temp;
      entity.getEntityList().last()->setInstanciationProbability(temp);
      in >> strListTemp;
      entity.getEntityList().last()->setTags(strListTemp);
      in >> entity.getEntityList().last()->getPosition().x;
      in >> entity.getEntityList().last()->getPosition().y;
      in >> entity.getEntityList().last()->getPosition().z;
      in >> entity.getEntityList().last()->getOrientation().w;
      in >> entity.getEntityList().last()->getOrientation().x;
      in >> entity.getEntityList().last()->getOrientation().y;
      in >> entity.getEntityList().last()->getOrientation().z;
      in >> entity.getEntityList().last()->getScale().x;
      in >> entity.getEntityList().last()->getScale().y;
      in >> entity.getEntityList().last()->getScale().z;
    }
  in >> intTemp;
  for (int i = 0; i < intTemp; ++i)
    {
      int     temp;
      Model   model;

      in >> strTemp;
      model.setName(strTemp);
      in >> strTemp;
      model.setPath(strTemp);
      entity.createModel(model, 0);
      in >> temp;
      entity.getModelList().last()->setInstanciationProbability(temp);
      in >> strListTemp;
      entity.getModelList().last()->setTags(strListTemp);
      in >> entity.getModelList().last()->getPosition().x;
      in >> entity.getModelList().last()->getPosition().y;
      in >> entity.getModelList().last()->getPosition().z;
      in >> entity.getModelList().last()->getOrientation().w;
      in >> entity.getModelList().last()->getOrientation().x;
      in >> entity.getModelList().last()->getOrientation().y;
      in >> entity.getModelList().last()->getOrientation().z;
      in >> entity.getModelList().last()->getScale().x;
      in >> entity.getModelList().last()->getScale().y;
      in >> entity.getModelList().last()->getScale().z;
    }
  return (in);
}
