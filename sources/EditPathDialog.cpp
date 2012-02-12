#include <QFileDialog>
#include "EditPathDialog.hpp"
#include "ui_EditPathDialog.h"

EditPathDialog::EditPathDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::EditPathDialog)
{
  ui->setupUi(this);
}

EditPathDialog::~EditPathDialog()
{
  delete ui;
}

void EditPathDialog::on_seclectDirButton_clicked()
{
  QString dir = QFileDialog::getExistingDirectory(this);

  if (!dir.isEmpty())
    ui->ressourcePathLineEdit->setText(dir);
}

void  EditPathDialog::setRessourcePath(QString const & path)
{
  ui->ressourcePathLineEdit->setText(path);
}

void  EditPathDialog::setRecursivity(bool recursivity)
{
  ui->recursivityCheckBox->setChecked(recursivity);
}

QString EditPathDialog::getRessourcePath(void) const
{
  return (ui->ressourcePathLineEdit->text());
}

bool  EditPathDialog::getRecursivity(void) const
{
  return (ui->recursivityCheckBox->isChecked());
}
