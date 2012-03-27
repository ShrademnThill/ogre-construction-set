#ifndef EDITINSTITEMDIALOG_HPP
#define EDITINSTITEMDIALOG_HPP

#include <QDialog>
#include <QStringListModel>

namespace Ui {
  class EditInstItemDialog;
}

class EditInstItemDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit EditInstItemDialog(QWidget *parent = 0);
  ~EditInstItemDialog();

  int         getInstanciationProbability(void) const;
  QStringList getTags(void) const;

  void  setMaxProbability(int value);
  void  setInstanciationProbability(int value);
  void  setTags(QStringList const & tags);
  
private slots:
  void  on_probaHorizontalSlider_valueChanged(int value);
  void  on_addTagButton_clicked(void);
  void  on_deleteTagButton_clicked();

private:
  Ui::EditInstItemDialog *  ui;
  int                       m_max;
  int                       m_value;
  QStringListModel          m_tags;
};

#endif // EDITINSTITEM_HPP
