#ifndef ENTITYGROUP_HPP
#define ENTITYGROUP_HPP

#include <QString>

class EntityGroup
{
public:
  EntityGroup(QString const & name);

  int getGroupIdx(void) const;

private:
  QString m_name;
  int     m_groupIdx;
};

#endif // ENTITYGROUP_HPP
