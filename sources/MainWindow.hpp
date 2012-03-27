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
  void  on_actionRefresh_triggered();
  void  on_actionChangeBackgroundColor_triggered();

  void  on_actionInstModel_triggered();
  void  on_actionInstEntity_triggered();
  void  on_actionAddEntity_triggered();
  void  on_actionLoadEntity_triggered();
  void  on_actionEditEntity_triggered();
  void  on_actionDeleteEntity_triggered();
  void  on_actionAddGroup_triggered();
  void  on_actionRenameGroup_triggered();
  void  on_actionDeleteGroup_triggered();

  void  on_actionActiveGrid_triggered(bool checked);
  void  on_actionConstraintX_triggered(void);
  void  on_actionConstraintY_triggered(void);
  void  on_actionConstraintZ_triggered(void);

  void  on_actionSave_triggered();

  void  on_currentEntityListView_doubleClicked(QModelIndex const & index);

  void  on_modelTreeView_customContextMenuRequested(QPoint const &);
  void  on_entityTreeView_customContextMenuRequested(QPoint const &);

  void  on_nextToolButton_clicked();
  void  on_prevToolButton_clicked();
  void  on_currentEntityListView_clicked(const QModelIndex &index);

  void  on_ogreWidget_itemSelected();
  void  on_ogreWidget_itemUnselected();
  void  on_ogreWidget_itemMoved();
  void  on_ogreWidget_itemDoubleClicked();

  void  updateItem();

  void  on_actionNewProject_triggered();

  void  on_actionSaveAs_triggered();

  void  on_actionOpen_triggered();

  void  on_actionResetCamera_triggered();

protected:
  virtual QMenu * createPopupMenu();
  virtual void    closeEvent(QCloseEvent * event);

private:
  void  initWidget();

  Ui::MainWindow *  ui;
  OgreWidget *      m_ogreWidget;
  ConfWidget *      m_confWidget;

  QFileSystemModel *    m_fileSystemModel;
  QStandardItemModel *  m_entityModel;
  Entity *              m_currentEntity;
};

#endif // MAINWINDOW_HPP
