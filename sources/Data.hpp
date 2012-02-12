#ifndef DATA_HPP
#define DATA_HPP

#include <QStringList>
#include <QList>
#include "RessourcesPathList.hpp"
#include "Model.hpp"
//#include "ModelList.hpp"

class Data
{
public:
  static Data * getSingleton();

  QList<RessourcesPath>   getRessourcesPathList() const;

  void  setRessourcesPathList(QList<RessourcesPath> const &);

private:
  Data();
  Data(Data const &);
  Data &  operator=(Data const &);

  static Data * m_instance;

  QList<RessourcesPath> m_ressourcesPathList;
};

#endif // DATA_HPP
