#ifndef EDITENTITYDIALOG_HPP
#define EDITENTITYDIALOG_HPP

#include <QDialog>

namespace Ui {
  class EditEntityDialog;
}

class EditEntityDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit EditEntityDialog(QWidget *parent = 0);
  ~EditEntityDialog();

  QString getEntityName() const;
  
private:
  Ui::EditEntityDialog *ui;
};

#endif // EDITENTITYDIALOG_HPP
