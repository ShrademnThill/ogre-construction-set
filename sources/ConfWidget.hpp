#ifndef CONFWIDGET_HPP
#define CONFWIDGET_HPP

#include <QDialog>

namespace Ui {
  class ConfWidget;
}

class ConfWidget : public QDialog
{
  Q_OBJECT

public:
  explicit ConfWidget(QWidget *parent = 0);
  ~ConfWidget();

private:
  Ui::ConfWidget *ui;
};

#endif // CONFWIDGET_HPP
