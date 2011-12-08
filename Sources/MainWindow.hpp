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
  void on_actionOptions_triggered();
  void on_actionLock_triggered(bool);

protected:
  virtual QMenu * createPopupMenu();

private:
  Ui::MainWindow *ui;
  OgreWidget *ogreWidget;
  ConfWidget *confWidget;
  ModelList *m_modelList;
};

#endif // MAINWINDOW_HPP
