#include "ConfWidget.hpp"
#include "ui_confwidget.h"

ConfWidget::ConfWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ConfWidget),
  m_ressourcesPathModelList(parent)
{
  ui->setupUi(this);
  connect(ui->optionsList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
          this, SLOT(changePage(QListWidgetItem*, QListWidgetItem*)));
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
