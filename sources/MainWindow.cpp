#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "Data.hpp"
#include "EditEntityDialog.hpp"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  initWidget();
}

MainWindow::~MainWindow()
{
  delete ui;
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
  //Ajout du widget de rendu
  m_ogreWidget = new OgreWidget(this);
  setCentralWidget(m_ogreWidget);

  //Ajout du widget de settings
  m_confWidget = new ConfWidget(this);

  //Connection des signaux
  connect(m_ogreWidget, SIGNAL(itemSelected(bool)), this, SLOT(itemSelected(bool)));
  connect(m_ogreWidget, SIGNAL(itemMoved()), this, SLOT(itemMoved()));
  connect(ui->positionXDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->positionYDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->positionZDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->rotationXDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->rotationYDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->rotationZDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));

//  connect(ui->entityTableView, SIGNAL(activated(QModelIndex)), this, SLOT(selectItem))

//  tabifyDockWidget(ui->lightWidget, ui->modelWidget);
//  tabifyDockWidget(ui->modelWidget, ui->entityWidget);
//  setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

  QSettings settings("config.ini", QSettings::IniFormat);

  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());

  //Initialisation des listes d'items
  m_modelList = new ModelList(this);
  m_entityList = new EntityList(this);
  ui->modelTableView->setModel(m_modelList);
  ui->entityTableView->setModel(m_entityList);

  m_entityList->addEntity(Entity("root entity"));
  m_ogreWidget->setCurrentEntity(&m_entityList->getList()[0]);

  refreshData();
}

void  MainWindow::refreshData()
{
  QList<RessourcesPath> ressourcesPathList = Data::getSingleton()->getRessourcesPathList();

  m_modelList->clearList();
  for (int i = 0; i < ressourcesPathList.size(); ++i)
    {
      m_modelList->build(ressourcesPathList[i].path, ressourcesPathList[i].rec);
    }

  ui->modelTableView->setModel(0);
  ui->modelTableView->setModel(m_modelList);
}

void  MainWindow::on_actionSettings_triggered()
{
  if (m_confWidget->exec() == QDialog::Accepted)
    {
      ui->statusBar->showMessage("Accepted");
      refreshData();
    }
  else
    {
      ui->statusBar->showMessage("Rejected");
    }
}

void  MainWindow::on_actionLock_triggered(bool locked)
{
  QList<QDockWidget *>  widgets = findChildren<QDockWidget *>();

  if (locked == true)
    {
      for (int i = 0; i < widgets.size(); ++i)
        {
          widgets[i]->setFeatures(QDockWidget::NoDockWidgetFeatures);
          widgets[i]->setTitleBarWidget(new QWidget(this));
        }
      ui->mainToolBar->setMovable(false);
    }
  else
    {
      for (int i = 0; i < widgets.size(); ++i)
        {
          widgets[i]->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
          widgets[i]->setTitleBarWidget(0);
        }
      ui->mainToolBar->setMovable(true);
    }
}

void  MainWindow::on_actionInstModel_triggered()
{
  QModelIndex idx = ui->modelTableView->selectionModel()->currentIndex();

  if (idx.isValid())
    m_ogreWidget->addItem(m_modelList->getList().at(idx.row()));
}

void  MainWindow::on_actionInstEntity_triggered()
{
  QModelIndex idx = ui->entityTableView->selectionModel()->currentIndex();

  if (idx.isValid())
    m_ogreWidget->addItem(m_entityList->getList().at(idx.row()));
}

void  MainWindow::on_actionAddEntity_triggered()
{
  EditEntityDialog  dialog(this);

  if (dialog.exec() == QDialog::Accepted)
    {
      m_entityList->insertRow(0);
      m_entityList->setData(m_entityList->index(0, 0), dialog.getEntityName(), Qt::EditRole);
    }
}

void  MainWindow::on_actionLoadEntity_triggered()
{
  QModelIndex     idx = ui->entityTableView->selectionModel()->currentIndex();

  if (idx.isValid())
    {
      m_currentEntity = &m_entityList->getList()[idx.row()];
      m_ogreWidget->setCurrentEntity(m_currentEntity);

      delete ui->currentEntityTableView->model();
      ui->currentEntityTableView->setModel(new CurrentEntityModel(m_currentEntity, this));
    }
}

void  MainWindow::on_actionRefresh_triggered()
{
  refreshData();
}

void  MainWindow::itemSelected(bool selected)
{
  Ogre::SceneNode * node = m_ogreWidget->getSelectedNode();
  ui->infoWidgetContents->setEnabled(selected);

  if (node)
    {
      ui->positionXDoubleSpinBox->setValue(node->getPosition().x);
      ui->positionYDoubleSpinBox->setValue(node->getPosition().y);
      ui->positionZDoubleSpinBox->setValue(node->getPosition().z);
      ui->rotationXDoubleSpinBox->setValue(node->getOrientation().x);
      ui->rotationYDoubleSpinBox->setValue(node->getOrientation().y);
      ui->rotationZDoubleSpinBox->setValue(node->getOrientation().z);
    }
}

void  MainWindow::itemMoved()
{
  Ogre::SceneNode * node = m_ogreWidget->getSelectedNode();

  if (node)
    {
      ui->positionXDoubleSpinBox->setValue(node->getPosition().x);
      ui->positionYDoubleSpinBox->setValue(node->getPosition().y);
      ui->positionZDoubleSpinBox->setValue(node->getPosition().z);
      ui->rotationXDoubleSpinBox->setValue(node->getOrientation().x);
      ui->rotationYDoubleSpinBox->setValue(node->getOrientation().y);
      ui->rotationZDoubleSpinBox->setValue(node->getOrientation().z);
    }
}

void  MainWindow::updateItem()
{
  Ogre::SceneNode * node = m_ogreWidget->getSelectedNode();

  if (node)
    {
      node->setPosition(ui->positionXDoubleSpinBox->value(), ui->positionYDoubleSpinBox->value(), ui->positionZDoubleSpinBox->value());
      node->setOrientation(10, ui->rotationXDoubleSpinBox->value(), ui->rotationYDoubleSpinBox->value(), ui->rotationZDoubleSpinBox->value());
      m_ogreWidget->update();
    }
}

void    MainWindow::closeEvent(QCloseEvent * event)
{
  QSettings settings("config.ini", QSettings::IniFormat);

  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());

  QMainWindow::closeEvent(event);
}

QMenu * MainWindow::createPopupMenu()
{
  QMenu * customMenu = QMainWindow::createPopupMenu();
  customMenu->addSeparator();
  customMenu->addAction(ui->actionLock);
  return (customMenu);
}
