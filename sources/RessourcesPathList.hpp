#ifndef RESSOURCESPATHLIST_HPP
#define RESSOURCESPATHLIST_HPP

#include <QAbstractTableModel>
#include <QList>

struct RessourcesPath
{
  RessourcesPath(){}
  QString path;
  bool    rec;
};

class RessourcesPathList : public QAbstractTableModel
{
public:
  RessourcesPathList(QObject * parent);
  ~RessourcesPathList(void);

  int           rowCount(const QModelIndex &parent) const;
  int           columnCount(const QModelIndex &parent) const;
  QVariant      data(const QModelIndex &index, int role) const;
  QVariant      headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool          setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  bool          insertRows(int row, int count, const QModelIndex &index = QModelIndex());
  bool          removeRows(int row, int count, const QModelIndex &index = QModelIndex());

  QList<RessourcesPath> const & getList(void) const;

private:
  QList<RessourcesPath> m_list;
};

#endif // RESSOURCESPATHLIST_HPP
