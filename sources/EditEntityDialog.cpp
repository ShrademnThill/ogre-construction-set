#include "EditEntityDialog.hpp"
#include "ui_EditEntityDialog.h"

EditEntityDialog::EditEntityDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::EditEntityDialog)
{
  ui->setupUi(this);
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

void  EditEntityDialog::setName(QString const & name)
{
  ui->nameLineEdit->setText(name);
}

void  EditEntityDialog::setComposed(bool composed)
{
  ui->composedCheckBox->setChecked(composed);
}
