#include <QColorDialog>
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "Data.hpp"
#include "EditEntityDialog.hpp"
#include "EditInstItemDialog.hpp"

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
  connect(ui->pXDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->pYDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->pZDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->oWDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->oXDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->oYDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->oZDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->sXDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->sYDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));
  connect(ui->sZDoubleSpinBox, SIGNAL(editingFinished()), this, SLOT(updateItem()));

//  setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
//  setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
//  setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
//  setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);

  QSettings settings("config.ini", QSettings::IniFormat);

  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());

  ui->actionLock->activate(QAction::Trigger);

  //Initialisation des listes d'items
  m_modelList = new ModelList(this);
  m_entityList = new EntityList(this);
  ui->modelTableView->setModel(m_modelList);
  ui->entityTableView->setModel(m_entityList);

  m_entityList->addEntity(Entity("root"));
  m_ogreWidget->setCurrentEntity(&m_entityList->getList()[0]);
  m_currentEntity = &m_entityList->getList()[0];

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
    {
      m_ogreWidget->addItem(m_modelList->getList().at(idx.row()));
      delete ui->currentEntityListView->model();
      ui->currentEntityListView->setModel(new CurrentEntityModel(m_currentEntity, this));
    }
}

void  MainWindow::on_actionInstEntity_triggered()
{
  QModelIndex idx = ui->entityTableView->selectionModel()->currentIndex();

  if (idx.isValid())
    {
      m_ogreWidget->addItem(m_entityList->getList()[idx.row()]);
      delete ui->currentEntityListView->model();
      ui->currentEntityListView->setModel(new CurrentEntityModel(m_currentEntity, this));
    }
}

void  MainWindow::on_actionAddEntity_triggered()
{
  EditEntityDialog  dialog(this);

  if (dialog.exec() == QDialog::Accepted)
    {
      m_entityList->insertRow(0);
      m_entityList->setData(m_entityList->index(0, 0), dialog.getName(), Qt::EditRole);
      m_entityList->setData(m_entityList->index(0, 1), dialog.getComposed(), Qt::EditRole);
    }
}

void  MainWindow::on_actionEditEntity_triggered()
{
  EditEntityDialog  dialog(this);
  QModelIndex       idx = ui->entityTableView->selectionModel()->currentIndex();

  if (idx.isValid())
    {
      dialog.setName(m_entityList->getList().at(idx.row()).getName());
      dialog.setComposed(m_entityList->getList().at(idx.row()).isComposed());
    }
  if (dialog.exec() == QDialog::Accepted)
    {
      m_entityList->setData(m_entityList->index(idx.row(), 0), dialog.getName(), Qt::EditRole);
      m_entityList->setData(m_entityList->index(idx.row(), 1), dialog.getComposed(), Qt::EditRole);
    }
}

void  MainWindow::on_actionLoadEntity_triggered()
{
  QModelIndex     idx = ui->entityTableView->selectionModel()->currentIndex();

  if (idx.isValid())
    {
      m_currentEntity = &m_entityList->getList()[idx.row()];
      m_ogreWidget->setCurrentEntity(m_currentEntity);

      delete ui->currentEntityListView->model();
      ui->currentEntityListView->setModel(new CurrentEntityModel(m_currentEntity, this));
    }
}

void  MainWindow::on_actionRefresh_triggered()
{
  refreshData();
}

void  MainWindow::itemSelected(bool selected)
{
  (void)selected;
  itemMoved();
  //ui->infoWidgetContents->setEnabled(selected);

}

void  MainWindow::itemMoved()
{
  ui->pXDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getPosition().x);
  ui->pYDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getPosition().y);
  ui->pZDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getPosition().z);
  ui->oWDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getOrientation().w);
  ui->oXDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getOrientation().x);
  ui->oYDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getOrientation().y);
  ui->oZDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getOrientation().z);
  ui->sXDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getScale().x);
  ui->sYDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getScale().y);
  ui->sZDoubleSpinBox->setValue(m_ogreWidget->getSelection()->getScale().z);
}

void  MainWindow::updateItem()
{
  m_ogreWidget->getSelection()->setPosition(ui->pXDoubleSpinBox->value(),
                                            ui->pYDoubleSpinBox->value(),
                                            ui->pZDoubleSpinBox->value());
  m_ogreWidget->getSelection()->setOrientation(ui->oWDoubleSpinBox->value(),
                                               ui->oXDoubleSpinBox->value(),
                                               ui->oYDoubleSpinBox->value(),
                                               ui->oZDoubleSpinBox->value());
  m_ogreWidget->getSelection()->setScale(ui->sXDoubleSpinBox->value(),
                                         ui->sYDoubleSpinBox->value(),
                                         ui->sZDoubleSpinBox->value());
  m_ogreWidget->update();
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

void  MainWindow::on_currentEntityListView_activated(QModelIndex const & index)
{
  EditInstItemDialog  dialog(this);
  InstItem * item = m_currentEntity->getItem(index.row());
  int deltaProba;

  dialog.setInstanciationProbability(item->getInstanciationProbability());
  dialog.setMaxProbability(m_currentEntity->getInstNothingProbability());
  if (dialog.exec() == QDialog::Accepted)
    {
      deltaProba = dialog.getInstanciationProbability() - item->getInstanciationProbability();
      item->setInstanciationProbability(dialog.getInstanciationProbability());
      m_currentEntity->setInstNothingProbability(m_currentEntity->getInstNothingProbability() - deltaProba);
    }
}

void  MainWindow::on_modelTableView_customContextMenuRequested(const QPoint & pos)
{
  QMenu * menu = new QMenu(this);

  menu->addAction(ui->actionInstModel);
  menu->popup(QCursor::pos());
}

void  MainWindow::on_entityTableView_customContextMenuRequested(const QPoint & pos)
{
  QMenu * menu = new QMenu(this);

  menu->addAction(ui->actionInstEntity);
  menu->addAction(ui->actionLoadEntity);
  menu->addAction(ui->actionAddEntity);
  menu->addAction(ui->actionEditEntity);
  menu->addAction(ui->actionDeleteEntity);
  menu->popup(QCursor::pos());
}

void  MainWindow::on_actionChangeBackgroundColor_triggered()
{
  QColor color = QColorDialog::getColor(Qt::white, this);

  if (color.isValid())
    m_ogreWidget->setBackgroundColor(color);
}

void  MainWindow::on_nextToolButton_clicked()
{
  int index = ui->transformationStackedWidget->currentIndex();

  if (index == 0)
    {
      ui->currentInfo->setText(tr("Rotation"));
      index++;
    }
  else if (index == 1)
    {
      ui->currentInfo->setText(tr("Scale"));
      index++;
    }
  else if (index == 2)
    {
      ui->currentInfo->setText(tr("Position"));
      index = 0;
    }
  ui->transformationStackedWidget->setCurrentIndex(index);
}

void  MainWindow::on_prevToolButton_clicked()
{
  int index = ui->transformationStackedWidget->currentIndex();

  if (index == 0)
    {
      ui->currentInfo->setText(tr("Scale"));
      index = 2;
    }
  else if (index == 1)
    {
      ui->currentInfo->setText(tr("Position"));
      index--;
    }
  else if (index == 2)
    {
      ui->currentInfo->setText(tr("Rotation"));
      index--;
    }
  ui->transformationStackedWidget->setCurrentIndex(index);
}

void  MainWindow::on_currentEntityListView_clicked(const QModelIndex &index)
{
  m_ogreWidget->selectItem(m_currentEntity->getItem(index.row()));
}

void MainWindow::on_actionDeleteEntity_triggered()
{
  QModelIndex       idx = ui->entityTableView->selectionModel()->currentIndex();

  if (idx.isValid() && &m_entityList->getList().at(idx.row()) != m_currentEntity)
    {
      m_entityList->getList().takeAt(idx.row());
      ui->entityTableView->setModel(0);
      ui->entityTableView->setModel(m_entityList);
    }
  else
    ui->statusBar->showMessage("Cannot delete entity already loaded");
}
