#include <QInputDialog>
#include "EditInstItemDialog.hpp"
#include "ui_EditInstItemDialog.h"

EditInstItemDialog::EditInstItemDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::EditInstItemDialog),
  m_max(100)
{
  ui->setupUi(this);

  ui->tagListView->setModel(&m_tags);
}

EditInstItemDialog::~EditInstItemDialog()
{
  delete ui;
}

int EditInstItemDialog::getInstanciationProbability(void) const
{
  return (ui->spinBox->value());
}

QStringList EditInstItemDialog::getTags(void) const
{
  return (m_tags.stringList());
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

void  EditInstItemDialog::setTags(QStringList const & tags)
{
  m_tags.setStringList(tags);
}

void  EditInstItemDialog::on_probaHorizontalSlider_valueChanged(int value)
{
  if (value >= m_max + m_value)
    ui->probaHorizontalSlider->setSliderPosition(m_max + m_value);
  ui->label->setText(QString("There is %1 percent of probability to instance nothing.").arg(m_max + (m_value - value)));
}

void  EditInstItemDialog::on_addTagButton_clicked(void)
{
  QString str = QInputDialog::getText(this, tr("Tag Name"), tr("Name"));

  if (!str.isEmpty())
    {
      m_tags.insertRow(0);
      m_tags.setData(m_tags.index(0), str);
    }
}

void  EditInstItemDialog::on_deleteTagButton_clicked()
{
  QModelIndex idx = ui->tagListView->selectionModel()->currentIndex();

  if (idx.isValid())
    m_tags.removeRow(idx.row());
}
