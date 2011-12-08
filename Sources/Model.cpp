#include "Model.hpp"

Model::Model(QString const & path, QString const & name) :
  m_path(path),
  m_name(name)
{
}

Model::~Model(void)
{
}


void Model::setName(QString const & name)
{
  m_name = name;
}

void Model::setPath(QString const & path)
{
  m_path = path;
}

QString const & Model::getName(void) const
{
  return (m_name);
}

QString const & Model::getPath(void) const
{
  return (m_path);
}
