#ifndef MODEL_HPP
#define MODEL_HPP

#include <QString>
#include "Item.hpp"

class Model : public Item
{
public:
  Model(QString const & path = "", QString const & name = "");
  ~Model(void);

  void setName(QString const & name);
  void setPath(QString const & path);

  QString const & getName(void) const;
  QString const & getPath(void) const;

private:
  QString m_name;
  QString m_path;
};

#endif // MODEL_HPP
