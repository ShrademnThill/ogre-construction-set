#include "EditInstItemDialog.hpp"
#include "ui_EditInstItemDialog.h"

EditInstItemDialog::EditInstItemDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::EditInstItemDialog),
  m_max(100)
{
  ui->setupUi(this);
}

EditInstItemDialog::~EditInstItemDialog()
{
  delete ui;
}

int EditInstItemDialog::getInstanciationProbability(void) const
{
  return (ui->spinBox->value());
}

void  EditInstItemDialog::setMaxProbability(int value)
{
  m_max = value;
  ui->label->setText(QString("There is %1 percent of probability to instance nothing.").arg(m_max));

}

void  EditInstItemDialog::setInstanciationProbability(int value)
{
  m_value = value;
  ui->probaHorizontalSlider->setSliderPosition(value);
  ui->spinBox->setValue(value);
}

void  EditInstItemDialog::on_probaHorizontalSlider_valueChanged(int value)
{
  if (value >= m_max + m_value)
    ui->probaHorizontalSlider->setSliderPosition(m_max + m_value);
  ui->label->setText(QString("There is %1 percent of probability to instance nothing.").arg(m_max + (m_value - value)));
}
