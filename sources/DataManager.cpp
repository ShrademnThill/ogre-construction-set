#include <QFile>
#include <QDir>
#include "DataManager.hpp"
#include "EntityModelItem.hpp"

DataManager * DataManager::m_instance = 0;

DataManager::DataManager(void)
{
  m_modelPath = "../data/";
  m_entityModel.setHorizontalHeaderLabels(QStringList() << "Name" << "Composed");
  m_entityModel.setItemPrototype(new EntityModelItem);
  m_defaultCameraDistance = 500;
  m_gridSpace = 100;
}

DataManager * DataManager::getSingleton(void)
{
  if (!m_instance)
    m_instance = new DataManager;
  return (m_instance);
}

QList<RessourcesPath> const & DataManager::getRessourcesPathList(void) const
{
  return (m_ressourcesPathList);
}

QString DataManager::getModelPath(void) const
{
  return (m_modelPath);
}

QUndoStack const &  DataManager::getUndoStack(void)
{
  return (m_undoStack);
}

QStandardItemModel *  DataManager::getEntityModel(void)
{
  return (&m_entityModel);
}

QList<Entity *> * DataManager::getEntityList(void)
{
  return (&m_entityList);
}

double  DataManager::getDefaultCameraDistance(void) const
{
  return (m_defaultCameraDistance);
}
double  DataManager::getGridSpace(void) const
{
  return (m_gridSpace);
}

void  DataManager::setRessourcesPathList(QList<RessourcesPath> const & ressourcesPathList)
{
  m_ressourcesPathList = ressourcesPathList;
}

void  DataManager::setModelPath(QString const & modelPath)
{
  m_modelPath = modelPath;
}

void  DataManager::setDefaultCameraDistance(double value)
{
  m_defaultCameraDistance = value;
}

void  DataManager::setGridSpace(double value)
{
  m_gridSpace = value;
}

void  DataManager::readTree(QStandardItem * item, int rowCount, QDataStream & in)
{
  int type;

  for (int i = 0; i < rowCount; ++i)
    {
      in >> type;
      if (type == 0)
        {
          int idx;

          in >> idx;
          item->appendRow(QList<QStandardItem *>() << new EntityModelItem(m_entityList[idx]) << new EntityModelItem(m_entityList[idx]));
        }
      else
        {
          QString name;
          int     count;

          in >> name;
          in >> count;
          item->appendRow(QList<QStandardItem *>() << new EntityModelItem(0, name) << new EntityModelItem(0));
          readTree(item->child(item->rowCount() - 1), count, in);
        }
    }
}

void  DataManager::openProject(QString const & fileName)
{
  QFile file(fileName);

  if (!file.open(QFile::ReadWrite))
    return ;

  QDataStream in(&file);
  int         rowCount;

  in >> rowCount;
  m_entityModel.removeRows(0, m_entityModel.rowCount());
  m_entityList.clear();
  for (int i = 0; i < rowCount; ++i)
    m_entityList.append(new Entity);
  foreach (Entity * entity, m_entityList)
    {
      QString     strTemp;
      QStringList strListTemp;
      int         intTemp;

      in >> strTemp;
      entity->setName(strTemp);
      in >> intTemp;
      entity->setInstNothingProbability(intTemp);
      in >> strListTemp;
      entity->setTags(strListTemp);
      in >> intTemp;
      for (int i = 0; i < intTemp; ++i)
        {
          int           temp;

          in >> temp;
          entity->createEntity(*m_entityList[temp], 0);
          in >> temp;
          entity->getEntityList().last()->setInstanciationProbability(temp);
          in >> strListTemp;
          entity->getEntityList().last()->setTags(strListTemp);
          in >> entity->getEntityList().last()->getPosition().x;
          in >> entity->getEntityList().last()->getPosition().y;
          in >> entity->getEntityList().last()->getPosition().z;
          in >> entity->getEntityList().last()->getOrientation().w;
          in >> entity->getEntityList().last()->getOrientation().x;
          in >> entity->getEntityList().last()->getOrientation().y;
          in >> entity->getEntityList().last()->getOrientation().z;
          in >> entity->getEntityList().last()->getScale().x;
          in >> entity->getEntityList().last()->getScale().y;
          in >> entity->getEntityList().last()->getScale().z;
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
          entity->createModel(model, 0);
          in >> temp;
          entity->getModelList().last()->setInstanciationProbability(temp);
          in >> strListTemp;
          entity->getModelList().last()->setTags(strListTemp);
          in >> entity->getModelList().last()->getPosition().x;
          in >> entity->getModelList().last()->getPosition().y;
          in >> entity->getModelList().last()->getPosition().z;
          in >> entity->getModelList().last()->getOrientation().w;
          in >> entity->getModelList().last()->getOrientation().x;
          in >> entity->getModelList().last()->getOrientation().y;
          in >> entity->getModelList().last()->getOrientation().z;
          in >> entity->getModelList().last()->getScale().x;
          in >> entity->getModelList().last()->getScale().y;
          in >> entity->getModelList().last()->getScale().z;
        }
    }
  in >> rowCount;
  readTree(m_entityModel.invisibleRootItem(), rowCount, in);

  file.close();
}

void  DataManager::writeTree(QStandardItem * item, QDataStream & out)
{
  for (int i = 0; i < item->rowCount(); ++i)
    {
      EntityModelItem * current;

      current = static_cast<EntityModelItem *>(item->child(i));
      if (!current->hasChildren())
        out << 0 << m_entityList.indexOf(current->getEntity());
      else
        {
          out << 1 << current->getName() << current->rowCount();
          writeTree(current, out);
        }
    }
}

void  DataManager::saveProject(QString const & fileName)
{
  QFile file(fileName);

  if (!file.open(QFile::ReadWrite))
    return ;

  QDataStream out(&file);

  out << m_entityList.size();
  for (int i = 0; i < m_entityList.size(); ++i)
    {
      out << m_entityList[i]->getName();
      out << m_entityList[i]->getInstNothingProbability();
      out << m_entityList[i]->getTags();
      out << m_entityList[i]->getEntityList().size();
      foreach (InstEntity * instEntity, m_entityList[i]->getEntityList())
        {
          out << m_entityList.indexOf(&instEntity->getEntity());
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
      out << m_entityList[i]->getModelList().size();
      foreach (InstModel * instModel,  m_entityList[i]->getModelList())
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
    }
  out << m_entityModel.rowCount();
  writeTree(m_entityModel.invisibleRootItem(), out);

  file.close();
}
