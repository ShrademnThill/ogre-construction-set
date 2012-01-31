#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtGui/QMainWindow>
#include "OgreWidget.hpp"
#include "ConfWidget.hpp"
#include "ModelList.hpp"

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

private slots:
  void  on_actionSettings_triggered();
  void  on_actionLock_triggered(bool);
  void  on_actionAddModel_triggered();

protected:
  virtual QMenu * createPopupMenu();

private:
  void  initData();
  void  initWidget();

  Ui::MainWindow *  ui;
  OgreWidget *      m_ogreWidget;
  ConfWidget *      m_confWidget;

  ModelList *     m_modelList;
  //LightList *     m_lightList;
  //EntityList *    m_entityList;
};

#endif // MAINWINDOW_HPP
