#include <QInputDialog>
#include "EditEntityDialog.hpp"
#include "ui_EditEntityDialog.h"

EditEntityDialog::EditEntityDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::EditEntityDialog)
{
  ui->setupUi(this);

  ui->listView->setModel(&m_filter);
}

EditEntityDialog::~EditEntityDialog()
{
  delete ui;
}

QString  EditEntityDialog::getName(void) const
{
  return (ui->nameLineEdit->text());
}

bool  EditEntityDialog::getComposed(void) const
{
  return (ui->composedCheckBox->isChecked());
}

QStringList EditEntityDialog::getFilter(void) const
{
  return (m_filter.stringList());
}

void  EditEntityDialog::setName(QString const & name)
{
  ui->nameLineEdit->setText(name);
}

void  EditEntityDialog::setComposed(bool composed)
{
  ui->composedCheckBox->setChecked(composed);
}


void  EditEntityDialog::setFilter(QStringList const & filter)
{
  m_filter.setStringList(filter);
}

void  EditEntityDialog::on_addFilterButton_clicked(void)
{
  QString str = QInputDialog::getText(this, tr("Filter Name"), tr("Name"));

  if (!str.isEmpty())
    {
      m_filter.insertRow(0);
      m_filter.setData(m_filter.index(0), str);
    }
}

void  EditEntityDialog::on_deleteFilterButton_clicked()
{
  QModelIndex idx = ui->listView->selectionModel()->currentIndex();

  if (idx.isValid())
    m_filter.removeRow(idx.row());
}
