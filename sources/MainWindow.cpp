#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "Data.hpp"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  initData();
  initWidget();
}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_ogreWidget;
  delete m_confWidget;
}

void  MainWindow::initData()
{
  m_modelList = new ModelList(Data::getSingleton()->getModelsPath());
}

void  MainWindow::initWidget()
{
  /*
  TODO:
    -Recuperer les donnes de la derniere session a l'aide d'un QSetting.
    -Restaurer la position des widgets et autre changement cosmetiques.
    -Charger les paths correct pour les models 3d.
  */
  ui->setupUi(this);
  m_ogreWidget = new OgreWidget;
  setCentralWidget(m_ogreWidget);
  tabifyDockWidget(ui->lightWidget, ui->modelWidget);
  tabifyDockWidget(ui->modelWidget, ui->entityWidget);
  setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

  m_confWidget = new ConfWidget(this);
  ui->modelTableView->setModel(m_modelList);
}

void  MainWindow::on_actionSettings_triggered()
{
  if (m_confWidget->exec() == QDialog::Accepted)
    {
      ui->statusBar->showMessage("Accepted");
    }
  else
    {
      ui->statusBar->showMessage("Rejected");
    }
}

void  MainWindow::on_actionLock_triggered(bool locked)
{
  static QWidget *   entityTitle = new QWidget;
  static QWidget *   modelTitle = new QWidget;
  static QWidget *   lightTitle = new QWidget;
  static QWidget *   infoTitle = new QWidget;

  if (locked == true)
    {
      ui->entityWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
      ui->entityWidget->setTitleBarWidget(entityTitle);
      ui->modelWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
      ui->modelWidget->setTitleBarWidget(modelTitle);
      ui->lightWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
      ui->lightWidget->setTitleBarWidget(lightTitle);
      ui->infoWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
      ui->infoWidget->setTitleBarWidget(infoTitle);
      ui->mainToolBar->setMovable(false);
    }
  else
    {
      ui->entityWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
      ui->entityWidget->setTitleBarWidget(0);
      ui->modelWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
      ui->modelWidget->setTitleBarWidget(0);
      ui->lightWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
      ui->lightWidget->setTitleBarWidget(0);
      ui->infoWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
      ui->infoWidget->setTitleBarWidget(0);
      ui->mainToolBar->setMovable(true);
    }
}

void  MainWindow::on_actionAddModel_triggered()
{
  QModelIndex idx = ui->modelTableView->selectionModel()->currentIndex();

  m_ogreWidget->addItem(m_modelList->getList().at(idx.row()));
}

QMenu * MainWindow::createPopupMenu()
{
  QMenu * customMenu = QMainWindow::createPopupMenu();
  //customMenu->addSeparator();
  //customMenu->addAction(ui->actionLock);
  return (customMenu);
}
