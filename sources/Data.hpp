#ifndef DATA_HPP
#define DATA_HPP

#include <QStringList>

class Data
{
public:
  static Data * getSingleton();

  QStringList & getRessourcesPath();
  QStringList & getModelsPath();

private:
  Data();
  Data(Data const &);
  Data &  operator=(Data const &);

  static Data * m_instance;

  QStringList   m_ressourcesPath;
  QStringList   m_modelsPath;
};

#endif // DATA_HPP
