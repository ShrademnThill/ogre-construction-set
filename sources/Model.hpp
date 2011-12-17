#ifndef MODEL_HPP
#define MODEL_HPP

#include <QString>

class Model
{
private:
  QString m_name;
  QString m_path;

public:
  Model(QString const & path, QString const & name = "");
  ~Model(void);

  void setName(QString const & name);
  void setPath(QString const & path);

  QString const & getName(void) const;
  QString const & getPath(void) const;
};

#endif // MODEL_HPP
