#include "Data.hpp"

Data * Data::m_instance = 0;

Data::Data()
{
  RessourcesPath  ressourcesPath;

  ressourcesPath.path = "../data/models/";
  m_ressourcesPathList.append(ressourcesPath);
}

Data * Data::getSingleton()
{
  if (!m_instance)
    m_instance = new Data;
  return (m_instance);
}

QList<RessourcesPath> Data::getRessourcesPathList() const
{
  return (m_ressourcesPathList);
}

void  Data::setRessourcesPathList(QList<RessourcesPath> const & ressourcesPathList)
{
  m_ressourcesPathList = ressourcesPathList;
}
