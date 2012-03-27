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
    void  changePage(QListWidgetItem * current, QListWidgetItem * previous);

private slots:
    void  on_addPathButton_clicked();
    void  on_deletePathButton_clicked();
    void  on_editPathButton_clicked();
    void  on_buttonBox_accepted();
    void  on_buttonBox_rejected();
    void  on_selectDirButton_clicked();

private:
  Ui::ConfWidget *  ui;

  RessourcesPathList *  m_ressourcesPathModelList;
};

#endif // CONFWIDGET_HPP
