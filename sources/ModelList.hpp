#ifndef MODELLIST_HPP
#define MODELLIST_HPP

#include <QAbstractTableModel>
#include <QList>
#include "Model.hpp"

class ModelList : public QAbstractTableModel
{

public:
  ModelList(QString const & path = "", QObject *parent = 0);
  ~ModelList(void);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  bool insertRows(int row, int count, const QModelIndex &index = QModelIndex());
  bool removeRows(int row, int count, const QModelIndex &index = QModelIndex());
  QList<Model> const & getList(void) const;

private:
  QList<Model> m_list;

  bool isModel(QString const & path) const;
  void build(QString const & path);
};

#endif // MODELLIST_HPP
