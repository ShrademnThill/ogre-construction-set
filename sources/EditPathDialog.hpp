#ifndef EDITPATHDIALOG_HPP
#define EDITPATHDIALOG_HPP

#include <QDialog>

namespace Ui {
  class EditPathDialog;
}

class EditPathDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit EditPathDialog(QWidget *parent = 0);
  ~EditPathDialog();

  void  setRessourcePath(QString const & path);
  void  setRecursivity(bool recursivity);

  QString getRessourcePath(void) const;
  bool    getRecursivity(void) const;
  
private slots:
  void  on_seclectDirButton_clicked();

private:
  Ui::EditPathDialog * ui;
};

#endif // ADDPATHDIALOG_HPP
