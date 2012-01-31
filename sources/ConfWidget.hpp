#ifndef CONFWIDGET_HPP
#define CONFWIDGET_HPP

#include <QDialog>
#include <QListWidgetItem>
#include "RessourcesPathList.hpp"

namespace Ui {
class ConfWidget;
}

class ConfWidget : public QDialog
{
  Q_OBJECT

public:
  explicit ConfWidget(QWidget *parent = 0);
  ~ConfWidget();

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
  Ui::ConfWidget *  ui;

  RessourcesPathList  m_ressourcesPathModelList;
};

#endif // CONFWIDGET_HPP
