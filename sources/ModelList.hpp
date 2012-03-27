#ifndef MODELLIST_HPP
#define MODELLIST_HPP

#include <QAbstractTableModel>
#include <QList>
#include "Model.hpp"

class ModelList : public QAbstractTableModel
{
public:
  ModelList(QObject * parent = 0);
  ModelList(QStringList const & paths, QObject * parent = 0);
  ~ModelList(void);

  int           rowCount(QModelIndex const & parent) const;
  int           columnCount(QModelIndex const & parent) const;
  QVariant      data(QModelIndex const & index, int role) const;
  QVariant      headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(QModelIndex const & index) const;
  bool          setData(QModelIndex const & index, const QVariant &value, int role = Qt::EditRole);
  bool          insertRows(int row, int count, QModelIndex const & index = QModelIndex());
  bool          removeRows(int row, int count, QModelIndex const & index = QModelIndex());

  QList<Model> const &  getList(void) const;

  void  build(QString const & path, bool rec);
  void  clearList();

private:
  QList<Model> m_list;

  bool  isModel(QString const & path) const;
};

#endif // MODELLIST_HPP
