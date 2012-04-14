#include <QFile>
#include <QDir>
#include <QSettings>
#include "DataManager.hpp"
#include "EntityModelItem.hpp"

DataManager * DataManager::m_instance = 0;

DataManager::DataManager(void)
{
  QSettings settings("config.ini", QSettings::IniFormat);

  m_modelPath = settings.value("model_path", "../data/").toString();
  m_defaultCameraDistance = settings.value("default_camera_distance", 500).toDouble();
  m_cameraLMoveMult = settings.value("camera_left_move_mult", 1).toDouble();
  m_cameraZMoveMult = settings.value("camera_zoom_move_mult", 1).toDouble();
  m_gridSpace = settings.value("grid_space", 128).toDouble();
  m_snapAngle = settings.value("snape_angle", 45).toDouble();

  m_entityModel.setHorizontalHeaderLabels(QStringList() << "Name" << "Composed");
  m_entityModel.setItemPrototype(new EntityModelItem);
}

DataManager::~DataManager(void)
{
  QSettings settings("config.ini", QSettings::IniFormat);

  settings.setValue("model_path", m_modelPath);
  settings.setValue("default_camera_distance", m_defaultCameraDistance);
  settings.setValue("camera_left_move_mult", m_cameraLMoveMult);
  settings.setValue("camera_zoom_move_mult", m_cameraZMoveMult);
  settings.setValue("grid_space", m_gridSpace);
  settings.setValue("snape_angle", m_snapAngle);

  m_instance = 0;
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

QList<Entity *> & DataManager::getEntityList(void)
{
  return (m_entityList);
}

double  DataManager::getDefaultCameraDistance(void) const
{
  return (m_defaultCameraDistance);
}

double  DataManager::getCameraLMoveMult(void) const
{
  return (m_cameraLMoveMult);
}
double  DataManager::getCameraZMoveMult(void) const
{
  return (m_cameraZMoveMult);
}

double  DataManager::getGridSpace(void) const
{
  return (m_gridSpace);
}

double  DataManager::getSnapAngle(void) const
{
  return (m_snapAngle);
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

void  DataManager::setCameraLMoveMult(double value)
{
  m_cameraLMoveMult = value;
}

void  DataManager::setCameraZMoveMult(double value)
{
  m_cameraZMoveMult = value;
}

void  DataManager::setGridSpace(double value)
{
  m_gridSpace = value;
}

void  DataManager::setSnapAngle(double value)
{
  m_snapAngle = value;
}

void  DataManager::readTree(QStandardItem * item, int rowCount, QDataStream & in, int offset)
{
  int type;

  for (int i = 0; i < rowCount; ++i)
    {
      in >> type;
      if (type == 0)
        {
          int idx;

          in >> idx;
          idx += offset;
          item->appendRow(QList<QStandardItem *>() << new EntityModelItem(m_entityList[idx]) << new EntityModelItem(m_entityList[idx]));
        }
      else
        {
          QString name;
          int     count;

          in >> name;
          in >> count;
          item->appendRow(QList<QStandardItem *>() << new EntityModelItem(0, name) << new EntityModelItem(0));
          readTree(item->child(item->rowCount() - 1), count, in, offset);
        }
    }
}

void  DataManager::importProject(QString const & fileName)
{
  QFile file(fileName);

  if (!file.open(QFile::ReadWrite))
    return ;

  QDataStream in(&file);
  int         rowCount;
  int         offset = m_entityList.size();

  in >> rowCount;
  for (int i = 0; i < rowCount; ++i)
    m_entityList.append(new Entity);
  for (int idx = offset; idx < m_entityList.size(); ++idx)
    in >> *m_entityList[idx];
  in >> rowCount;
  readTree(m_entityModel.invisibleRootItem(), rowCount, in, offset);

  file.close();
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
    in >> *entity;
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

  if (!file.open(QFile::ReadWrite|QFile::Truncate))
    return ;

  QDataStream out(&file);

  out << m_entityList.size();
  for (int i = 0; i < m_entityList.size(); ++i)
    out << *m_entityList[i];
  out << m_entityModel.rowCount();
  writeTree(m_entityModel.invisibleRootItem(), out);

  file.close();
}
