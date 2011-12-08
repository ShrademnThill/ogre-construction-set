#include "MainWindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ogreWidget = new OgreWidget;
  ui->ogreLayout->addWidget(ogreWidget);

  tabifyDockWidget(ui->lightWidget, ui->modelWidget);
  tabifyDockWidget(ui->modelWidget, ui->entityWidget);

  confWidget = new ConfWidget(this);
  m_modelList = new ModelList("media", this);
  ui->modelTableView->setModel(m_modelList);
}

MainWindow::~MainWindow()
{
  delete ui;
  delete ogreWidget;
  delete confWidget;
}

void MainWindow::on_actionOptions_triggered()
{
  ogreWidget->addModel(m_modelList->getList().at(27));
  if (confWidget->exec() == QDialog::Accepted)
    {
      ui->statusBar->showMessage("Accepted");
    }
  else
    {
      ui->statusBar->showMessage("Rejected");
    }
}

void MainWindow::on_actionLock_triggered(bool locked)
{
  static bool isAllocated = false;
  QWidget *   entityTitle;
  QWidget *   modelTitle;
  QWidget *   lightTitle;

  if (isAllocated == false)
    {
      entityTitle = new QWidget;
      modelTitle = new QWidget;
      lightTitle = new QWidget;
      isAllocated = false;
    }
  if (locked == true)
    {
      ui->entityWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
      ui->entityWidget->setTitleBarWidget(entityTitle);
      ui->modelWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
      ui->modelWidget->setTitleBarWidget(modelTitle);
      ui->lightWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
      ui->lightWidget->setTitleBarWidget(lightTitle);
    }
  else
    {
      ui->entityWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
      ui->entityWidget->setTitleBarWidget(0);
      ui->modelWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
      ui->modelWidget->setTitleBarWidget(0);
      ui->lightWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
      ui->lightWidget->setTitleBarWidget(0);
    }
}

QMenu * MainWindow::createPopupMenu()
{
  QMenu * customMenu = QMainWindow::createPopupMenu();
  //customMenu->addSeparator();
  //customMenu->addAction(ui->actionLock);
  return (customMenu);
}
