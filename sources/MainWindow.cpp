#include <QColorDialog>
#include <QInputDialog>
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "DataManager.hpp"
#include "EditEntityDialog.hpp"
#include "EditInstItemDialog.hpp"
#include "EntityModelItem.hpp"
#include "SceneXML.hpp"

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
  //Ajout du widget de rendu
  m_ogreWidget = new OgreWidget(this);
  m_ogreWidget->setObjectName("ogreWidget");
  ui->setupUi(this);
  setCentralWidget(m_ogreWidget);

  //Ajout du widget de settings
  m_confWidget = new ConfWidget(this);

  //Connection des signaux
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

  //Restauration de la configuration
  setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
  setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
  setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
  setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);

  QSettings settings("config.ini", QSettings::IniFormat);

  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());

  ui->actionLock->activate(QAction::Trigger);

  //Initialisation du model des models
  m_fileSystemModel = new QFileSystemModel(this);

  m_fileSystemModel->setRootPath(QDir::currentPath());
  m_fileSystemModel->setNameFilters(QStringList("*.mesh"));
  m_fileSystemModel->setNameFilterDisables(false);
  ui->modelTreeView->setModel(m_fileSystemModel);
  ui->modelTreeView->header()->hideSection(1);
  ui->modelTreeView->header()->hideSection(2);
  ui->modelTreeView->header()->hideSection(3);

  //Initialisation du model des entity
  m_entityModel = DataManager::getSingleton()->getEntityModel();
  ui->entityTreeView->setModel(m_entityModel);
  ui->entityTreeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);

  on_actionNewProject_triggered();
}

void  MainWindow::refreshData()
{
  ui->modelTreeView->setRootIndex(m_fileSystemModel->index(DataManager::getSingleton()->getModelPath()));
  ui->currentEntityLabel->setText(m_currentEntity->getName());
  m_ogreWidget->setCurrentEntity(m_currentEntity);
  delete ui->currentEntityListView->model();
  ui->currentEntityListView->setModel(new CurrentEntityModel(m_currentEntity, this));
}

void  MainWindow::closeEvent(QCloseEvent * event)
{
  QSettings settings("config.ini", QSettings::IniFormat);

  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());
  delete DataManager::getSingleton();

  QMainWindow::closeEvent(event);
}

QMenu * MainWindow::createPopupMenu()
{
  QMenu * customMenu = QMainWindow::createPopupMenu();

  customMenu->addSeparator();
  customMenu->addAction(ui->actionLock);
  return (customMenu);
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

void  MainWindow::on_actionRefresh_triggered()
{
  refreshData();
}

void  MainWindow::on_actionChangeBackgroundColor_triggered()
{
  QColor color = QColorDialog::getColor(Qt::white, this);

  if (color.isValid())
    m_ogreWidget->setBackgroundColor(color);
}

void  MainWindow::on_actionInstModel_triggered()
{
  QModelIndex idx = ui->modelTreeView->selectionModel()->currentIndex();

  if (idx.isValid())
    {
      Model model(m_fileSystemModel->fileName(idx), m_fileSystemModel->fileName(idx));

      m_ogreWidget->addItem(model);
      delete ui->currentEntityListView->model();
      ui->currentEntityListView->setModel(new CurrentEntityModel(m_currentEntity, this));
    }
}

void  MainWindow::on_actionInstEntity_triggered()
{
  QModelIndex idx = ui->entityTreeView->selectionModel()->currentIndex();

  if (idx.isValid())
    {
      Entity * entity = static_cast<EntityModelItem *>(m_entityModel->itemFromIndex(idx))->getEntity();

      if (!entity)
        return ;
      if (!m_currentEntity->containEntity(entity))
        {
          m_ogreWidget->addItem(*entity);
          delete ui->currentEntityListView->model();
          ui->currentEntityListView->setModel(new CurrentEntityModel(m_currentEntity, this));
        }
      else
        ui->statusBar->showMessage(tr("Cannot instanciate Entity containing the current entity because infinited loop"));
    }
}

void  MainWindow::on_actionAddEntity_triggered()
{
  EditEntityDialog  dialog(this);
  QModelIndex       idx = ui->entityTreeView->selectionModel()->currentIndex();
  EntityModelItem * item = 0;

  if (!idx.isValid())
    return ;
  item = static_cast<EntityModelItem *>(m_entityModel->itemFromIndex(idx));
  if (dialog.exec() == QDialog::Accepted)
    {
      Entity * entity = new Entity;

      entity->setName(dialog.getName());
      entity->setComposed(dialog.getComposed());
      entity->setTags(dialog.getFilter());
      DataManager::getSingleton()->getEntityList().append(entity);
      if (!item->getEntity())
        item->appendRow(QList<QStandardItem *>() << new EntityModelItem(entity) << new EntityModelItem(entity));
      else
        m_entityModel->appendRow(QList<QStandardItem *>() << new EntityModelItem(entity) << new EntityModelItem(entity));
    }
}

void  MainWindow::on_actionLoadEntity_triggered()
{
  QModelIndex     idx = ui->entityTreeView->selectionModel()->currentIndex();

  if (idx.isValid())
    {
      Entity * entity = static_cast<EntityModelItem *>(m_entityModel->itemFromIndex(idx))->getEntity();

      if (!entity)
        return ;
      m_currentEntity = entity;
      m_ogreWidget->setCurrentEntity(m_currentEntity);

      delete ui->currentEntityListView->model();
      ui->currentEntityListView->setModel(new CurrentEntityModel(m_currentEntity, this));
      ui->currentEntityLabel->setText(m_currentEntity->getName());
    }
}

void  MainWindow::on_actionEditEntity_triggered()
{
  EditEntityDialog  dialog(this);
  QModelIndex       idx = ui->entityTreeView->selectionModel()->currentIndex();
  Entity *          entity = 0;

  if (!idx.isValid())
    return ;
  entity = static_cast<EntityModelItem *>(m_entityModel->itemFromIndex(idx))->getEntity();
  if (!entity || entity->isDeleted())
    return ;
  dialog.setName(entity->getName());
  dialog.setComposed(entity->isComposed());
  dialog.setFilter(entity->getTags());
  if (dialog.exec() == QDialog::Accepted)
    {
      entity->setTags(dialog.getFilter());
      m_entityModel->setData(idx, dialog.getName());
      m_entityModel->setData(idx.sibling(idx.row(), 1), dialog.getComposed());
    }
}

void  MainWindow::on_actionDeleteEntity_triggered()
{
  QModelIndex       idx = ui->entityTreeView->selectionModel()->currentIndex();
  Entity *          entity = 0;

  if (!idx.isValid())
    return ;
  entity = static_cast<EntityModelItem *>(m_entityModel->itemFromIndex(idx))->getEntity();
  if (!entity)
    return ;
  if (entity != m_currentEntity && !entity->isDeleted())
    {
      entity->setDeleted(true);
      m_entityModel->removeRow(idx.row(), idx.parent());
    }
  else
    ui->statusBar->showMessage("Cannot delete entity already loaded");
}

void  MainWindow::on_actionAddGroup_triggered()
{
  QString str = QInputDialog::getText(this, tr("Add Group"), tr("Group name:"));

  if (!str.isEmpty())
    m_entityModel->appendRow(QList<QStandardItem *>() << new EntityModelItem(0, str) << new EntityModelItem(0));
}

void  MainWindow::on_actionRenameGroup_triggered()
{
  QModelIndex       idx = ui->entityTreeView->selectionModel()->currentIndex();

  if (!idx.isValid())
    return ;

  QString str = QInputDialog::getText(this, tr("Rename Group"), tr("Group name:"));

  if (!str.isEmpty())
    m_entityModel->setData(idx.sibling(idx.row(), 0), str);
}

void  MainWindow::on_actionDeleteGroup_triggered()
{
  QModelIndex idx = ui->entityTreeView->selectionModel()->currentIndex();

  if (!idx.isValid())
    return ;
  if (!m_entityModel->rowCount(idx))
    m_entityModel->removeRow(idx.row(), idx.parent());
}

void  MainWindow::on_actionSnapToGrid_triggered(bool checked)
{
  m_ogreWidget->snapToGrid(checked);
}

void  MainWindow::on_actionSnapToAngle_triggered(bool checked)
{
  m_ogreWidget->snapToAngle(checked);
}

void  MainWindow::on_actionConstraintX_triggered(void)
{
  m_ogreWidget->constraintX(true);
  m_ogreWidget->constraintY(false);
  m_ogreWidget->constraintZ(false);
  ui->actionConstraintX->setChecked(true);
  ui->actionConstraintY->setChecked(false);
  ui->actionConstraintZ->setChecked(false);
}

void  MainWindow::on_actionConstraintY_triggered(void)
{
  m_ogreWidget->constraintX(false);
  m_ogreWidget->constraintY(true);
  m_ogreWidget->constraintZ(false);
  ui->actionConstraintX->setChecked(false);
  ui->actionConstraintY->setChecked(true);
  ui->actionConstraintZ->setChecked(false);
}

void  MainWindow::on_actionConstraintZ_triggered(void)
{
  m_ogreWidget->constraintX(false);
  m_ogreWidget->constraintY(false);
  m_ogreWidget->constraintZ(true);
  ui->actionConstraintX->setChecked(false);
  ui->actionConstraintY->setChecked(false);
  ui->actionConstraintZ->setChecked(true);
}

void  MainWindow::on_actionResetCamera_triggered()
{
  m_ogreWidget->resetCamera();
}

void  MainWindow::on_actionNewProject_triggered()
{
  Entity * entity;

  if (m_ogreWidget->isVisible())
    {
      m_ogreWidget->getSelection()->clearSelection();
      m_ogreWidget->resetCamera();
    }
  m_entityModel->removeRows(0, m_entityModel->rowCount());
  entity = new Entity("root");
  m_entityModel->appendRow(QList<QStandardItem *>() << new EntityModelItem(entity) << new EntityModelItem(entity));
  m_currentEntity = entity;
  refreshData();
  for (int i = 0; i < DataManager::getSingleton()->getEntityList().size(); ++i)
    delete DataManager::getSingleton()->getEntityList().takeAt(i);
  DataManager::getSingleton()->getEntityList().append(entity);
  setWindowFilePath("");
}

void  MainWindow::on_actionOpen_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("OCS files (*.ocs)"));

  if (!fileName.isEmpty())
    {
      DataManager::getSingleton()->openProject(fileName);
      m_currentEntity = DataManager::getSingleton()->getEntityList()[0];
      refreshData();
      setWindowFilePath(fileName);
      ui->statusBar->showMessage(fileName + " opened.");
    }
}

void  MainWindow::on_actionSave_triggered()
{
  if (windowFilePath().isEmpty())
    on_actionSaveAs_triggered();
  else
    {
      DataManager::getSingleton()->saveProject(windowFilePath());
      ui->statusBar->showMessage("Save done.");
    }
}

void  MainWindow::on_actionSaveAs_triggered()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), tr("OCS files (*.ocs)"));

  if (!fileName.isEmpty())
    {
      refreshData();
      DataManager::getSingleton()->saveProject(fileName);
      setWindowFilePath(fileName);
      ui->statusBar->showMessage(fileName + " saved.");
    }
}

void  MainWindow::on_actionExportScene_triggered()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), tr("OgreDotScene files (*.scene)"));

  if (!fileName.isEmpty())
    {
      QFile         file(fileName);
      QTextStream   out(&file);

      if (!file.open(QFile::ReadWrite|QFile::Truncate))
        return ;

      SceneXML scene(m_ogreWidget->getScene());

      scene.save(out);
      file.close();
    }
}

void  MainWindow::on_actionImportProject_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Import File"), QDir::homePath(), tr("OCS files (*.ocs)"));

  if (!fileName.isEmpty())
    {
      DataManager::getSingleton()->importProject(fileName);
      refreshData();
      ui->statusBar->showMessage(fileName + " imported.");
    }
}

void  MainWindow::on_modelTreeView_customContextMenuRequested(const QPoint &)
{
  QMenu * menu = new QMenu(this);

  menu->addAction(ui->actionInstModel);
  menu->popup(QCursor::pos());
}

void  MainWindow::on_entityTreeView_customContextMenuRequested(const QPoint &)
{
  QModelIndex idx = ui->entityTreeView->selectionModel()->currentIndex();
  QMenu *     menu = new QMenu(this);
  Entity *    entity = 0;

  menu->addAction(ui->actionInstEntity);
  menu->addAction(ui->actionLoadEntity);
  menu->addAction(ui->actionAddEntity);
  menu->addAction(ui->actionEditEntity);
  menu->addAction(ui->actionDeleteEntity);
  menu->addSeparator();
  menu->addAction(ui->actionAddGroup);
  menu->addAction(ui->actionRenameGroup);
  menu->addAction(ui->actionDeleteGroup);
  ui->actionInstEntity->setEnabled(false);
  ui->actionLoadEntity->setEnabled(false);
  ui->actionEditEntity->setEnabled(false);
  ui->actionDeleteEntity->setEnabled(false);
  ui->actionRenameGroup->setEnabled(false);
  ui->actionDeleteGroup->setEnabled(false);
  if (idx.isValid())
    {
      entity = static_cast<EntityModelItem *>(m_entityModel->itemFromIndex(idx))->getEntity();
      if (entity)
        {
          ui->actionInstEntity->setEnabled(true);
          ui->actionLoadEntity->setEnabled(true);
          ui->actionEditEntity->setEnabled(true);
          ui->actionDeleteEntity->setEnabled(true);
        }
      else
        {
          ui->actionRenameGroup->setEnabled(true);
          if (!m_entityModel->rowCount(idx))
            ui->actionDeleteGroup->setEnabled(true);
        }
    }
  menu->popup(QCursor::pos());
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

void  MainWindow::on_currentEntityListView_doubleClicked(QModelIndex const & index)
{
  EditInstItemDialog  dialog(this);
  InstItem * item = m_currentEntity->getItem(index.row());
  int deltaProba;

  dialog.setInstanciationProbability(item->getInstanciationProbability());
  dialog.setMaxProbability(m_currentEntity->getInstNothingProbability());
  dialog.setTags(item->getTags());
  if (dialog.exec() == QDialog::Accepted)
    {
      deltaProba = dialog.getInstanciationProbability() - item->getInstanciationProbability();
      item->setInstanciationProbability(dialog.getInstanciationProbability());
      m_currentEntity->setInstNothingProbability(m_currentEntity->getInstNothingProbability() - deltaProba);
      item->setTags(dialog.getTags());
    }
}

void  MainWindow::on_ogreWidget_itemSelected()
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

void  MainWindow::on_ogreWidget_itemUnselected()
{
  ;
}

void  MainWindow::on_ogreWidget_itemMoved()
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

void  MainWindow::on_ogreWidget_itemDoubleClicked()
{
  ui->statusBar->showMessage("Item double clicked!");
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

