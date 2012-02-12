#include "ConfWidget.hpp"
#include "ui_ConfWidget.h"
#include "Data.hpp"
#include "EditPathDialog.hpp"

ConfWidget::ConfWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ConfWidget)
{
  ui->setupUi(this);
  connect(ui->optionsList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
          this, SLOT(changePage(QListWidgetItem*, QListWidgetItem*)));

  m_ressourcesPathModelList = new RessourcesPathList(Data::getSingleton()->getRessourcesPathList(), this);
  ui->pathTableView->setModel(m_ressourcesPathModelList);
}

ConfWidget::~ConfWidget()
{
  delete ui;
}

void  ConfWidget::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
  if (!current)
    current = previous;
  ui->optionPages->setCurrentIndex(ui->optionsList->row(current));
}

void  ConfWidget::on_addPathButton_clicked()
{
  EditPathDialog  dialog(this);

  if (dialog.exec() == QDialog::Accepted)
    {
      m_ressourcesPathModelList->insertRow(0);
      m_ressourcesPathModelList->setData(m_ressourcesPathModelList->index(0, 0), dialog.getRessourcePath(), Qt::EditRole);
      m_ressourcesPathModelList->setData(m_ressourcesPathModelList->index(0, 1), dialog.getRecursivity(), Qt::EditRole);
    }
}

void ConfWidget::on_editPathButton_clicked()
{
  QModelIndex     idx = ui->pathTableView->selectionModel()->currentIndex();
  EditPathDialog  dialog(this);

  dialog.setRessourcePath(m_ressourcesPathModelList->data(m_ressourcesPathModelList->index(idx.row(), 0), Qt::DisplayRole).toString());
  dialog.setRecursivity(m_ressourcesPathModelList->data(m_ressourcesPathModelList->index(idx.row(), 1), Qt::DisplayRole).toInt());
  if (dialog.exec() == QDialog::Accepted)
    {
      m_ressourcesPathModelList->setData(m_ressourcesPathModelList->index(idx.row(), 0), dialog.getRessourcePath(), Qt::EditRole);
      m_ressourcesPathModelList->setData(m_ressourcesPathModelList->index(idx.row(), 1), dialog.getRecursivity(), Qt::EditRole);
    }
}

void  ConfWidget::on_deletePathButton_clicked()
{
  QModelIndex idx = ui->pathTableView->selectionModel()->currentIndex();

  if (idx.isValid())
    m_ressourcesPathModelList->removeRow(idx.row());
}

void  ConfWidget::on_buttonBox_accepted()
{
  Data::getSingleton()->setRessourcesPathList(m_ressourcesPathModelList->getList());
}

void  ConfWidget::on_buttonBox_rejected()
{
  //Reinitialisation
  m_ressourcesPathModelList->setList(Data::getSingleton()->getRessourcesPathList());
  ui->pathTableView->setModel(0);
  ui->pathTableView->setModel(m_ressourcesPathModelList);
}
