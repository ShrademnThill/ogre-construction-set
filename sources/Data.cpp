#include "Data.hpp"

Data * Data::m_instance = 0;

Data::Data()
{
}

Data * Data::getSingleton()
{
  if (!m_instance)
    m_instance = new Data;
  return (m_instance);
}

QStringList & Data::getRessourcesPath()
{
  return (m_ressourcesPath);
}

QStringList & Data::getModelsPath()
{
  return (m_modelsPath);
}
