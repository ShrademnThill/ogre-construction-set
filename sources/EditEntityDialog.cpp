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

QString  EditEntityDialog::getEntityName() const
{
  return (ui->nameLineEdit->text());
}
