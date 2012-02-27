#ifndef EDITINSTITEMDIALOG_HPP
#define EDITINSTITEMDIALOG_HPP

#include <QDialog>

namespace Ui {
  class EditInstItemDialog;
}

class EditInstItemDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit EditInstItemDialog(QWidget *parent = 0);
  ~EditInstItemDialog();

  int getInstanciationProbability(void) const;

  void  setMaxProbability(int value);
  void  setInstanciationProbability(int value);
  
private slots:
  void  on_probaHorizontalSlider_valueChanged(int value);

private:
  Ui::EditInstItemDialog *  ui;
  int                       m_max;
  int                       m_value;
};

#endif // EDITINSTITEM_HPP
