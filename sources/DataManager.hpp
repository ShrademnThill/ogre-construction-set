#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <QList>
#include <QUndoStack>
#include <QStandardItemModel>
#include "RessourcesPathList.hpp"
#include "Entity.hpp"

class DataManager
{
public:
  ~DataManager(void);

  static DataManager * getSingleton(void);

  QList<RessourcesPath> const & getRessourcesPathList(void) const;
  QString                       getModelPath(void) const;
  QUndoStack const &            getUndoStack(void);
  QStandardItemModel *          getEntityModel(void);
  QList<Entity *> &             getEntityList(void);
  double                        getDefaultCameraDistance(void) const;
  double                        getCameraLMoveMult(void) const;
  double                        getCameraZMoveMult(void) const;
  double                        getGridSpace(void) const;
  double                        getSnapAngle(void) const;

  void  setRessourcesPathList(QList<RessourcesPath> const &);
  void  setModelPath(QString const & modelPath);
  void  setDefaultCameraDistance(double value);
  void  setCameraLMoveMult(double value);
  void  setCameraZMoveMult(double value);
  void  setGridSpace(double value);
  void  setSnapAngle(double value);

  void  importProject(QString const & fileName);
  void  openProject(QString const & fileName);
  void  saveProject(QString const & fileName);

private:
  DataManager(void);
  DataManager(DataManager const &);
  DataManager &  operator=(DataManager const &);

  void  readTree(QStandardItem * item, int rowCount, QDataStream & in, int offset = 0);
  void  writeTree(QStandardItem * item, QDataStream & out);

  static DataManager *  m_instance;

  QList<RessourcesPath> m_ressourcesPathList;
  QString               m_modelPath;
  QUndoStack            m_undoStack;

  QStandardItemModel    m_entityModel;
  QList<Entity *>       m_entityList;

  double  m_defaultCameraDistance;
  double  m_cameraLMoveMult;
  double  m_cameraZMoveMult;
  double  m_gridSpace;
  double  m_snapAngle;
};

#endif // DATAMANAGER_HPP
