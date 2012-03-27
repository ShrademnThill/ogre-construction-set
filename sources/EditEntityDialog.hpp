#ifndef EDITENTITYDIALOG_HPP
#define EDITENTITYDIALOG_HPP

#include <QDialog>
#include <QStringListModel>

namespace Ui {
  class EditEntityDialog;
}

class EditEntityDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit EditEntityDialog(QWidget *parent = 0);
  ~EditEntityDialog();

  QString     getName(void) const;
  bool        getComposed(void) const;
  QStringList getFilter(void) const;

  void  setName(QString const & name);
  void  setComposed(bool composed);
  void  setFilter(QStringList const & filter);

private slots:
  void  on_addFilterButton_clicked(void);
  void  on_deleteFilterButton_clicked();

private:
  Ui::EditEntityDialog *  ui;
  QStringListModel        m_filter;
};

#endif // EDITENTITYDIALOG_HPP
