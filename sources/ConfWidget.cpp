#include "ConfWidget.hpp"
#include "ui_confwidget.h"

ConfWidget::ConfWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ConfWidget)
{
  ui->setupUi(this);
}

ConfWidget::~ConfWidget()
{
  delete ui;
}
