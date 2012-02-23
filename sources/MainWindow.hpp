#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtGui/QMainWindow>
#include "OgreWidget.hpp"
#include "ConfWidget.hpp"
#include "ModelList.hpp"
#include "EntityList.hpp"
#include "CurrentEntityModel.hpp"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void  refreshData();

private slots:
  void  on_actionSettings_triggered();
  void  on_actionLock_triggered(bool);
  void  on_actionInstModel_triggered();
  void  on_actionInstEntity_triggered();
  void  on_actionRefresh_triggered();

  void  itemSelected(bool);
  void  itemMoved();
  void  updateItem();

  void  on_actionAddEntity_triggered();
  void  on_actionLoadEntity_triggered();

protected:
  virtual QMenu * createPopupMenu();
  virtual void    closeEvent(QCloseEvent * event);

private:
  void  initWidget();

  Ui::MainWindow *  ui;
  OgreWidget *      m_ogreWidget;
  ConfWidget *      m_confWidget;

  ModelList *       m_modelList;
  EntityList *      m_entityList;
  Entity *          m_currentEntity;
};

#endif // MAINWINDOW_HPP
